#include "Tour.hh"
#include "Util.hh"

namespace MultiTSP {
Tour::Tour(std::vector<Team> const &p_teams, unsigned int p_max_size)
    : size(0), max_size(p_max_size), teams(p_teams) {}

std::string Tour::to_string() const {
  std::string res("{\"size\": " + std::to_string(size) +
                  ", \"max\": " + std::to_string(max_size) + ", \"ids\": [");
  res += join<unsigned int>(ids.begin(), ids.end()) + "]}";
  return res;
}

std::ostream &Tour::print(std::ostream &ostr) const {
  ostr << "{\"size\": " << size << ", \"max\": " << max_size << ", \"ids\": [";
  for (auto id : ids) {
    ostr << id;
  }
  ostr << "] }";

  return ostr;
}

} // namespace MultiTSP
