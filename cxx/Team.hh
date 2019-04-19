#ifndef MULTI_TSP_TEAM_HH
#define MULTI_TSP_TEAM_HH

namespace MultiTSP {

// One or more persons or items that will get in at the same address.
class Team {
public:
  Team(unsigned int id, unsigned int size, unsigned int entry_time);

private:
  unsigned int const id;
  unsigned int const size;
  unsigned int const entry_time;
};

} // namespace MultiTSP

#endif
