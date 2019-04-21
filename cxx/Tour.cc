#include <algorithm>

#include "Tour.hh"
#include "Util.hh"

namespace MultiTSP {
Tour::Tour(DistMatrix const &p_dists, Rating2Value const &p_rating2value,
           TeamSet const &p_teams, unsigned int p_max_places)
    : max_places(p_max_places), dists(p_dists), rating2value(p_rating2value),
      teams(p_teams), places_used(0) {

  assert(dists.size() > 0);
  assert(dists[0].size() == dists.size());
}

std::string Tour::as_json() const {
  Rating const rating(compute_rating());
  Value const value(rating * rating2value);
  std::string res("{\"places\": " + std::to_string(places_used) +
                  ", \"max\": " + std::to_string(max_places) +
                  ", \"rating\": " + rating.as_json() +
                  ", \"value\": " + std::to_string(value) + ", \"ids\": [");
  res += join<TeamId_t>(ids.begin(), ids.end()) + "]}";
  return res;
}

namespace {

unsigned int add_to_rating(Rating &rating, unsigned int spaces_used,
                           DistMatrix const &dists, Team const &current_team,
                           TeamId_t const to_id) {
  unsigned int const entry_time(current_team.get_entry_time());
  unsigned int const drive_time_to_next(
      // -1????
      dists[current_team.get_id().get()][to_id.get()]);
  std::cerr << "{ RAW DIST: " << drive_time_to_next << " - " << entry_time
            << "} ";
  // ToDo: As long as a Team contains only one person, a ++ is done here.
  // In the final release this must be adapted to the number of persons.
  //// spaces_used += current_team.get_size();
  ++spaces_used;

  rating.inc_tour_length(entry_time + drive_time_to_next);
  rating.inc_length_of_stay((entry_time + drive_time_to_next) * spaces_used);
  return spaces_used;
}

} // namespace

Rating Tour::compute_rating() const {

  // std::cerr << "Compute rating Tour " << to_json() << " ";

  Rating rating;

  if (ids.empty()) {
    return rating;
  }

  unsigned int spaces_used(0);

  // Distances in between
  for (unsigned int i(0); i < ids.size() - 1; ++i) {
    std::cerr << "CR1 " << i << " " << rating.as_json() << std::endl;
    spaces_used = add_to_rating(rating, spaces_used, dists, teams[ids[i]],
                                teams[ids[i + 1]].get_id());
  }
  std::cerr << "CR2 " << rating.as_json() << std::endl;
  // Distances to school
  add_to_rating(rating, spaces_used, dists, teams[ids.back()], TeamId_t(0));
  std::cerr << "CR3 " << rating.as_json() << std::endl;
  return rating;
}

void Tour::optimize() {
  std::vector<TeamId_t> opt(ids);
  // Use original data set to find optimal!
  std::sort(ids.begin(), ids.end(),
            [](TeamId_t ida, TeamId_t idb) { return ida.get() < idb.get(); });
  Value optimal_value(10e20);

  do {
    // ToDo: Optimization: Store Rating and value for further use.
    Rating const rating(compute_rating());
    Value const value(rating * rating2value);

    if (value < optimal_value) {
      // std::cout << "New Best " << value << std::endl;
      optimal_value = value;
      opt = ids;
    }
  } while (std::next_permutation(ids.begin(), ids.end()));

  ids = opt;
}

} // namespace MultiTSP
