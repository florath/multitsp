#include "State.hh"
#include "Util.hh"

#include <algorithm>
#include <iostream>

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
             std::vector<Team> const &p_teams, DistMatrix const &p_dists)
    : teams(p_teams), dists(p_dists), tour_cnt(p_tour_cnt),
      spaces_per_tour_cnt(p_spaces_per_tour_cnt) {
  tours.reserve(tour_cnt);
  for (unsigned int i(0); i < tour_cnt; ++i) {
    tours.push_back(Tour(teams, spaces_per_tour_cnt));
  }

  // Need to sort the teams: start with the teams with the most members.
  std::vector<Team> tc(teams);
  std::sort(tc.begin(), tc.end(), [](Team const &x, Team const &y) -> bool {
    return x.get_size() > y.get_size();
  });

  for (auto const &team : tc) {
    assign_team(team, tours);
  }

  std::cout << *this << std::endl;
}

std::ostream &State::print(std::ostream &ostr) const {
  ostr << "{\"cnt\": " << tour_cnt << ", \"spaces\": " << spaces_per_tour_cnt
       << ", \"tour\": [" << join<Tour>(tours.begin(), tours.end()) << "]}";
  return ostr;
}

} // namespace MultiTSP
