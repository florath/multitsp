#ifndef MULTI_TSP_CONFIG_HH
#define MULTI_TSP_CONFIG_HH

#include <vector>

#include "DistMatrix.hh"
#include "Rating2Value.hh"
#include "Team.hh"
#include "TeamSet.hh"

namespace MultiTSP {

class Config {
public:
  Config(int argc, char *argv[]);

  DistMatrix const &get_distances() const { return distances; }
  TeamSet const &get_teams() const { return teams; }
  unsigned int get_spaces_per_tour_cnt() const { return spaces_per_tour_cnt; }
  unsigned int get_tour_cnt() const { return tour_cnt; }
  Rating2Value const & get_rating2value() const { return rating2value; }
  std::string const & get_host_id() const { return host_id; }

private:
  TeamSet teams;
  DistMatrix distances;
  Rating2Value rating2value;
  unsigned int tour_cnt;
  unsigned int thread_cnt;
  unsigned int spaces_per_tour_cnt;
  std::string host_id;
};

} // namespace MultiTSP

#endif
