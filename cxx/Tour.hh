#ifndef MULTI_TSP_TOUR_HH
#define MULTI_TSP_TOUR_HH

#include <cassert>
#include <iostream>
#include <vector>

#include "Team.hh"

namespace MultiTSP {

class Tour {
public:
  Tour(std::vector<Team> const &teams, unsigned int max_size);

  bool fit(unsigned int tsize) const {
    return size + tsize <= max_size; }
  void push_back(unsigned int id) {
    ids.push_back(id);
    assert(teams[id-1].get_id() == id);
    size += teams[id-1].get_size();
    assert(size <= max_size);
  }

  std::ostream &print(std::ostream &ostr) const {
    ostr << "[Tour size [" << size << "] max [" << max_size << "] [";
    for (auto id : ids) {
      ostr << id << " ";
    }
    ostr << "]";

    return ostr;
  }

private:
  unsigned int size;
  unsigned int const max_size;
  std::vector<Team> const &teams;
  std::vector<unsigned int> ids;
};
} // namespace MultiTSP

inline std::ostream &operator<<(std::ostream &stream,
                                MultiTSP::Tour const &tour) {
  return tour.print(stream);
}

#endif
