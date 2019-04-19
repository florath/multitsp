#ifndef MULTI_TSP_CONFIG_HH
#define MULTI_TSP_CONFIG_HH

#include <vector>

#include "DistMatrix.hh"
#include "Team.hh"

namespace MultiTSP {

class Config {
public:
  Config(int argc, char *argv[]);

  DistMatrix const &get_distances() const { return distances; }
  std::vector<Team> const &get_teams() const { return teams; }
  unsigned int get_spaces_per_tour_cnt() const { return spaces_per_tour_cnt; }
  unsigned int get_tour_cnt() const { return tour_cnt; }

private:
  std::vector<Team> teams;
  DistMatrix distances;

  float weight_x;
  float weight_y;
  unsigned int tour_cnt;
  unsigned int thread_cnt;
  unsigned int spaces_per_tour_cnt;
};

} // namespace MultiTSP

#endif
