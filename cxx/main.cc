#include "Config.hh"
#include "State.hh"
#include <cmath>
#include <ctime>
#include <iostream>

using namespace MultiTSP;

namespace {

inline double drnadom() { return ((double)random()) / RAND_MAX; }

int sa(Config const &config, unsigned long random_seed) {

  std::cerr << "T1" << std::endl;
  
  std::mt19937 rng(random_seed);

  std::cerr << "T2" << std::endl;
  DistMatrix const &dists(config.get_distances());
  State state(config.get_tour_cnt(), config.get_spaces_per_tour_cnt(),
              config.get_rating2value(), config.get_teams(), dists, rng,
              random_seed, config.get_host_id());
  std::cout << state.as_json("random") << std::endl;
  state.optimize_local();
  std::cout << state.as_json("random local optimized") << std::endl;

  // This algorithm uses three different state / result pairs:
  // 1. the optimal state
  //    This is the really optimal state found during the
  //    algorithm. Because it can get worse, this might not
  //    be the one from the end of the algorithm.
  // 2. the current state
  //    This is the last accepted state
  // 3. the new state
  //    This is the generated new state - but not (yet)
  //    accepted by the

  Rating2Value const rating2value(config.get_rating2value());

  State current_state(state);
  Rating current_rating(current_state.compute_rating());
  Value current_value(current_rating * rating2value);

  State opt_state(state);
  Rating opt_rating(opt_state.compute_rating());
  Value opt_value(opt_rating * rating2value);

  unsigned long sa_round_idx(0);
  unsigned long sa_round_with_same_opt_cnt(0);

  double temp(1200.0);
  State new_state(state);
  // Simulated Annealing
  while (true) {
    // Homogenous chain
    for (int hmcl(0); hmcl < 10000; ++hmcl) {
      // ResultType const cres(tspst.value());
      // double const crval = cres.normalize();

      State new_state(current_state);
      new_state.swap();
      new_state.optimize_local();
      Rating const new_rating(new_state.compute_rating());
      Value const new_value(new_rating * rating2value);

      double const diff(new_value - current_value);
      // Always accept if it's getting better or
      // limited depending on the current temperature.
      if (diff < 0 || drnadom() < exp(-((double)diff) / temp)) {
        current_state = new_state;
        current_rating = new_rating;
        current_value = new_value;
      }

      if (opt_value > new_value) {
        opt_state = new_state;
        opt_rating = new_rating;
        opt_value = new_value;

        std::cout << opt_state.as_json("intermediate", sa_round_idx)
                  << std::endl;

        sa_round_with_same_opt_cnt = 0;
      }

      ++sa_round_idx;
      ++sa_round_with_same_opt_cnt;
    }

    if (sa_round_idx % 50000 == 0)
      temp *= 1.02;
    else
      temp *= 0.99;

    // Exit algorithm if 5e6 times no better state was
    // accepted (~1h CPU time)
    if (sa_round_with_same_opt_cnt >= 5000000) {
      std::cout << opt_state.as_json("final", sa_round_idx) << std::endl;
      return 0;
    }
  }
}

} // namespace

int main(int argc, char *argv[]) {

  time_t const tseed(time(0));
  srandom(tseed);

  Config const config(argc, argv);

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(
      0, std::numeric_limits<unsigned long>::max());

  sa(config, dist(rng));

  return 0;
}
