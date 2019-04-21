#ifndef MULTI_TSP_TOUR_HH
#define MULTI_TSP_TOUR_HH

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "DistMatrix.hh"
#include "Rating.hh"
#include "Team.hh"
#include "TeamId.hh"
#include "TeamSet.hh"

namespace MultiTSP {

class Tour {
public:
  Tour(DistMatrix const &dists, Rating2Value const &rating2value,
       TeamSet const &teams, unsigned int max_places);

  bool fit(unsigned int tsize) const {
    return places_used + tsize <= max_places;
  }
  void push_back(TeamId_t const id) {
    ids.push_back(id);
    // ???? ToDo:    assert(teams[id].get_id() == id);
    places_used += teams[id].get_size();
    assert(places_used <= max_places);
  }

  Tour &operator=(Tour const &other) {
    places_used = other.places_used;
    ids = other.ids;
    return *this;
  }

  std::string as_json() const;
  void optimize();

  Rating compute_rating() const;

  //  std::ostream &print(std::ostream &ostr) const;

  bool try_swap(Tour & other);

private:
  unsigned int const max_places;
  DistMatrix const &dists;
  Rating2Value const &rating2value;
  TeamSet const &teams;

  unsigned int places_used;
  std::vector<TeamId_t> ids;
};

} // namespace MultiTSP

namespace std {
inline std::string to_string(MultiTSP::Tour const &t) { return t.as_json(); }
} // namespace std

#endif
