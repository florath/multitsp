#include "Config.hh"
#include "State.hh"
#include <iostream>

using namespace MultiTSP;

namespace {

unsigned int compute_absolut_minimum(DistMatrix const &dists) {
  unsigned int res(0);
  for (unsigned int i(0); i < dists.size(); ++i) {
    res += dists[i][0];
  }
  return res;
}

} // namespace

int main(int argc, char *argv[]) {

  Config const config(argc, argv);

  DistMatrix const &dists(config.get_distances());

  std::cout << "Absolut minimum: " << compute_absolut_minimum(dists)
            << std::endl;

  State state(config.get_tour_cnt(), config.get_spaces_per_tour_cnt(),
              config.get_teams(), dists);

  return 0;
}
