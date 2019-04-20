#ifndef MULTI_TSP_TEAM_HH
#define MULTI_TSP_TEAM_HH

#include <iostream>
#include "TeamId.hh"

namespace MultiTSP {

// One or more persons or items that will get in at the same address.
class Team {
public:
  Team(TeamId_t id, unsigned int size, unsigned int entry_time);

  std::string as_json() const;

  unsigned int get_size() const { return size; }
  TeamId_t get_id() const { return id; }
  unsigned int get_entry_time() const { return entry_time; }

private:
  TeamId_t id;
  unsigned int size;
  unsigned int entry_time;
};

inline std::ostream &operator<<(std::ostream &out, Team const team) {
  out << team.as_json();
  return out;
}
  
} // namespace MultiTSP

#endif
