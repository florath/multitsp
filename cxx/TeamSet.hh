#ifndef MULTI_TSP_TEAM_SET_HH
#define MULTI_TSP_TEAM_SET_HH

#include "Team.hh"
#include "TeamId.hh"
#include <cassert>
#include <vector>

#undef TEAM_SET_TRACE

#ifdef TEAM_SET_TRACE
#include <iostream>
#endif

namespace MultiTSP {

class TeamSet {
public:
  TeamSet() {
#ifdef TEAM_SET_TRACE
    std::cerr << "TeamSet constructor()" << std::endl;
#endif
  }
  TeamSet(std::vector<Team> const p_teams) : teams(p_teams) {
#ifdef TEAM_SET_TRACE
    std::cerr << "TeamSet constructor(teams) " << teams.size() << std::endl;
#endif
  }

  Team const &operator[](TeamId_t const id) const {
#ifdef TEAM_SET_TRACE
    std::cerr << "TeamSet operator[]() " << id << std::endl;
#endif
    assert(id.get() >= 1);
    assert(id.get() < teams.size() + 1);
    return teams[id.get() - 1];
  }

  void push_back(Team &&team) {
#ifdef TEAM_SET_TRACE
    std::cerr << "TeamSet push_back()" << team << std::endl;
#endif
    teams.push_back(team);
  }
  std::vector<Team>::iterator begin() {
#ifdef TEAM_SET_TRACE
    std::cerr << "TeamSet begin()" << std::endl;
#endif
    return teams.begin();
  }
  std::vector<Team>::iterator end() {
#ifdef TEAM_SET_TRACE
    std::cerr << "TeamSet end()" << std::endl;
#endif
    return teams.end();
  }

private:
  std::vector<Team> teams;
};

} // namespace MultiTSP

#endif
