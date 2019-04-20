#include "Rating.hh"

namespace MultiTSP {

std::string Rating::as_json() const {
  return std::string("{\"tour-length\": ") + std::to_string(tour_length)
    + ", \"length-of-stay\": " + std::to_string(length_of_stay) + "}";
}

} // namespace MultiTSP
