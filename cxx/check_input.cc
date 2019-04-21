#include "Config.hh"
#include "State.hh"
#include <iostream>

using namespace MultiTSP;

int main(int argc, char *argv[]) {
  Config const config(argc, argv);
  DistMatrix const &dists(config.get_distances());
  unsigned int const cnt(dists.size());

  for (unsigned int from(0); from < cnt; ++from) {
    for (unsigned int to(0); to < cnt; ++to) {
      for (unsigned int i(0); i < cnt; ++i) {
        unsigned int const dist_direct(dists[from][to]);
        unsigned int const dist_indirect(dists[from][i] + dists[i][to]);

        if (dist_direct > dist_indirect) {
          std::cerr << "ERROR in input data: " << from << ", " << to << ", "
                    << i << std::endl;
          std::cerr << "      direct: " << dist_direct
                    << " indirect: " << dist_indirect
                    << " difference: " << (dist_direct - dist_indirect)
                    << std::endl;
        }
      }
    }
  }
}
