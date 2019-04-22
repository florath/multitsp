#include "State.hh"
#include "Util.hh"
#include "Value.hh"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

#include <boost/date_time/posix_time/posix_time.hpp>

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
             DistMatrix const &p_dists, std::mt19937 &p_rng,
             unsigned long p_random_seed, std::string const &p_host_id)
    : teams(p_teams), dists(p_dists), tour_cnt(p_tour_cnt),
      spaces_per_tour_cnt(p_spaces_per_tour_cnt), rating2value(p_rating2value),
      rng(p_rng), random_seed(p_random_seed), host_id(p_host_id) {
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

std::string State::as_json(std::string const &comment,
                           unsigned long round) const {
  Rating const rating(compute_rating());
  Value const value(rating * rating2value);
  std::hash<std::thread::id> hasher;

  boost::posix_time::ptime const timestamp(
      boost::posix_time::second_clock::universal_time());

  return std::string("{\"timestamp\": \"") + to_iso_extended_string(timestamp) +
         "\", \"rating\": " + rating.as_json() +
         ", \"value\": " + std::to_string(value) + ", \"comment\": \"" +
         comment + "\", \"round\": " + std::to_string(round) +
         ", \"thread-id\": " +
         std::to_string(hasher(std::this_thread::get_id())) +
         ", \"tour-cnt\": " + std::to_string(tour_cnt) +
         ", \"spaces\": " + std::to_string(spaces_per_tour_cnt) +
         ", \"random-seed\": " + std::to_string(random_seed) +
         ", \"host-id\": \"" + host_id + "\", \"tours\": [" +
         join<Tour>(tours.begin(), tours.end()) + "]}";
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
  std::uniform_int_distribution<std::mt19937::result_type> dist(
      0, tours.size() - 1);

  unsigned int const t1(dist(rng));
  unsigned int const t2(dist(rng));
  if (t1 == t2) {
    return false;
  }

  return tours[t1].try_swap(tours[t2], rng);
}

void State::swap() {
  while (try_swap()) {
  }
}

} // namespace MultiTSP
