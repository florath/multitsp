#include "Tour.hh"

namespace MultiTSP
{
Tour::Tour(std::vector<Team> const &p_teams, unsigned int p_max_size)
    : size(0), max_size(p_max_size), teams(p_teams) {}
}
