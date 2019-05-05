#ifndef MULTI_TSP_TOUR_HH
#define MULTI_TSP_TOUR_HH

#include <cassert>
#include <iostream>
#include <random>
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
    places_used += teams[id].get_size();
    assert(places_used <= max_places);
    rating_is_valid = false;
  }

  Tour &operator=(Tour const &other) {
    places_used = other.places_used;
    ids = other.ids;
    rating_is_valid = other.rating_is_valid;
    rating_cached = other.rating_cached;
    return *this;
  }

  void clear() {
    places_used = 0;
    ids.clear();
    rating_is_valid = false;
  }

  std::string as_json() const;
  void optimize();
  Rating compute_rating() const;
  bool try_swap(Tour &other, std::mt19937 &rng);

private:
  // If the rating is not valid, compute it.
  Rating internal_compute_rating() const;

  unsigned int const max_places;
  DistMatrix const &dists;
  Rating2Value const &rating2value;
  TeamSet const &teams;

  unsigned int places_used;
  std::vector<TeamId_t> ids;

  // Cache the rating
  mutable bool rating_is_valid;
  mutable Rating rating_cached;
};

} // namespace MultiTSP

namespace std {

inline std::string to_string(MultiTSP::Tour const &t) { return t.as_json(); }
} // namespace std

#endif
