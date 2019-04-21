#ifndef MULTI_TSP_STATE_HH
#define MULTI_TSP_STATE_HH

#include "DistMatrix.hh"
#include "Rating.hh"
#include "Rating2Value.hh"
#include "Team.hh"
#include "TeamSet.hh"
#include "Tour.hh"

namespace MultiTSP {

class State {
public:
  State(unsigned int tour_cnt, unsigned int spaces_per_tour_cnt,
        Rating2Value const &rating2value, TeamSet const &p_teams,
        DistMatrix const &p_dists);

  State &operator=(State const &other) {
    this->tours = other.tours;
    return *this;
  }

  Rating compute_rating() const;
  void optimize_local();
  void swap();

  std::ostream &as_json(std::ostream &ostr, std::string const & comment) const;

private:
  bool try_swap();
  
  TeamSet const &teams;
  DistMatrix const &dists;
  unsigned int const tour_cnt;
  unsigned int const spaces_per_tour_cnt;
  Rating2Value const &rating2value;

  std::vector<Tour> tours;
};

} // namespace MultiTSP

/*
inline std::ostream &operator<<(std::ostream &stream,
                                MultiTSP::State const &state) {
  return state.print(stream);
}
*/

#endif
