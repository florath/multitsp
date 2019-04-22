#ifndef MULTI_TSP_RATING_HH
#define MULTI_TSP_RATING_HH

#include <iostream>
#include <string>

#include "Rating2Value.hh"
#include "Value.hh"

namespace MultiTSP {

// The rating are the pure numbers which can be extracted
// from the state.
// Rating2Value class is the convertion, and Value (aka double) the
// value that can be compared.

class Rating {
public:
  Rating(unsigned int p_tour_length = 0, unsigned int p_length_of_stay = 0)
      : tour_length(p_tour_length), length_of_stay(p_length_of_stay) {}

  void clear() { tour_length = 0; length_of_stay = 0;
  }

  Value operator*(Rating2Value const &r2v) const {
    return tour_length * r2v.get_weight_tour_length() +
           length_of_stay * r2v.get_weight_length_of_stay();
  }

  Rating &operator+=(Rating const &other) {
    // std::cerr << "Rating " << as_json() << " Other " << other.as_json();
    tour_length += other.tour_length;
    length_of_stay += other.length_of_stay;
    // std::cerr << " -> " << as_json() << std::endl;
    return *this;
  }

  bool operator==(Rating const &other) const {
    return tour_length == other.tour_length &&
           length_of_stay == other.length_of_stay;
  }

  std::string as_json() const;

  void inc_tour_length(unsigned long v) { tour_length += v; }
  void inc_length_of_stay(unsigned long v) { length_of_stay += v; }

private:
  // Plain time the tour takes
  unsigned long tour_length;
  // Weighted sum of places * time on tour
  unsigned long length_of_stay;
};

inline std::ostream &operator<<(std::ostream &stream,
                                MultiTSP::Rating const &rating) {
  stream << rating.as_json();
  return stream;
}
} // namespace MultiTSP

#endif
