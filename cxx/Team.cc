#include "Team.hh"

namespace MultiTSP {

Team::Team(TeamId_t p_id, unsigned int p_size, unsigned int p_entry_time)
    : id(p_id), size(p_size), entry_time(p_entry_time) {}

std::string Team::as_json() const {
  return std::string("{\"id\" :") + std::to_string(id) +
         ", \"size\" :" + std::to_string(size) +
         ", \"entry_time\" :" + std::to_string(entry_time) + "}";
}

} // namespace MultiTSP
