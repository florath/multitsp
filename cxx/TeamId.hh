#ifndef MULTI_TSP_TEAM_ID_HH
#define MULTI_TSP_TEAM_ID_HH

#include <string>
#include <iostream>

#include "TypeDefinition.hh"

namespace MultiTSP {

// This is a dedicated type which starts counting at 1
// e.g. the first team is team 1
// (This is historical, because entry 0 was the target place.)
using TeamId_t = TypeDefinition<unsigned int, struct Team_Id>;

inline bool operator<(TeamId_t const a, TeamId_t const b) {
  return a.get() < b.get();
}

inline std::ostream &operator<<(std::ostream &out, TeamId_t const id) {
  out << id.get();
  return out;
}

} // namespace MultiTSP

namespace std {
inline std::string to_string(MultiTSP::TeamId_t const id) {
  return std::to_string(id.get());
}
} // namespace std

#endif
