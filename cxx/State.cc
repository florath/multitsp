#include "State.hh"

namespace MultiTSP {

State::State(unsigned int p_tour_cnt, unsigned int p_spaces_per_tour_cnt,
             std::vector<Team> const &p_teams, DistMatrix const &p_dists)
    : teams(p_teams), dists(p_dists), tour_cnt(p_tour_cnt),
      spaces_per_tour_cnt(p_spaces_per_tour_cnt) {
  tours.reserve(tour_cnt);
}
} // namespace MultiTSP
