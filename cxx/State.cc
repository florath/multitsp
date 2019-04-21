#include "State.hh"
#include "Util.hh"
#include "Value.hh"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>

#undef TRACE_STATE

namespace MultiTSP {

namespace {
void assign_team(Team const &team, std::vector<Tour> &tours) {
  for (auto &tour : tours) {
    if (tour.fit(team.get_size())) {
      tour.push_back(team.get_id());
      return;
    }
  }
  abort();
}
} // namespace

State::State(unsigned int p_tour_cnt, unsigned int p_spaces_per_tour_cnt,
             Rating2Value const &p_rating2value, TeamSet const &p_teams,
             DistMatrix const &p_dists)
    : teams(p_teams), dists(p_dists), tour_cnt(p_tour_cnt),
      spaces_per_tour_cnt(p_spaces_per_tour_cnt), rating2value(p_rating2value) {
  tours.reserve(tour_cnt);
  for (unsigned int i(0); i < tour_cnt; ++i) {
    tours.push_back(Tour(dists, rating2value, teams, spaces_per_tour_cnt));
  }

  // Need to sort the teams: start with the teams with the most members.
  TeamSet tc(teams);
  std::sort(tc.begin(), tc.end(), [](Team const &x, Team const &y) -> bool {
    return x.get_size() > y.get_size();
  });

  for (auto const &team : tc) {
    assign_team(team, tours);
  }
}

std::ostream &State::as_json(std::ostream &ostr,
                             std::string const &comment) const {
  Rating const rating(compute_rating());
  Value const value(rating * rating2value);

  std::chrono::time_point<std::chrono::system_clock> now =
      std::chrono::system_clock::now();
  time_t now_time = std::chrono::system_clock::to_time_t(now);
  auto gmt_time = gmtime(&now_time);
  auto timestamp = std::put_time(gmt_time, "%Y-%m-%d %H:%M:%S");

  ostr << "{\"timestamp\": \"" << timestamp
       << "\", \"rating\": " << rating.as_json() << ", \"value\": " << value
       << ", \"comment\": \"" << comment << "\", \"cnt\": " << tour_cnt
       << ", \"spaces\": " << spaces_per_tour_cnt << ", \"tour\": ["
       << join<Tour>(tours.begin(), tours.end()) << "]}";
  return ostr;
}

Rating State::compute_rating() const {
  Rating rating;
#ifdef TRACE_STATE
  std::cerr << "State::compute_rating start " << rating.as_json() << std::endl;
#endif
  for (auto tour : tours) {
    rating += tour.compute_rating();
#ifdef TRACE_STATE
    std::cerr << "State::compute_rating inc " << rating.as_json() << std::endl;
#endif
  }
#ifdef TRACE_STATE
  std::cerr << "State::compute_rating final " << rating.as_json() << std::endl;
#endif
  return rating;
}

void State::optimize_local() {
  for (auto &tour : tours) {
    tour.optimize();
  }
}

bool State::try_swap() {
  unsigned int const t1(random() % tours.size());
  unsigned int const t2(random() % tours.size());
  if (t1 == t2) {
    return false;
  }

  return tours[t1].try_swap(tours[t2]);
}

void State::swap() {
  while (try_swap()) {
  }
}

} // namespace MultiTSP
