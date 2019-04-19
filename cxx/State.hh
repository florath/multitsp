#ifndef MULTI_TSP_STATE_HH
#define MULTI_TSP_STATE_HH

#include "DistMatrix.hh"
#include "Team.hh"
#include "Tour.hh"

namespace MultiTSP {

class State {
public:
  State(unsigned int tour_cnt, unsigned int spaces_per_tour_cnt,
        std::vector<Team> const &p_teams, DistMatrix const &p_dists);

  std::ostream & print(std::ostream & ostr) const;

private:
  std::vector<Team> const &teams;
  DistMatrix const &dists;

  unsigned int const tour_cnt;
  unsigned int const spaces_per_tour_cnt;
  std::vector<Tour> tours;
};

} // namespace MultiTSP

inline std::ostream &operator<<(std::ostream &stream,
                                MultiTSP::State const &state) {
  return state.print(stream);
}

#endif
