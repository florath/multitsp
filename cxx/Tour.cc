#include <algorithm>

#include "Tour.hh"
#include "Util.hh"

#undef TRACE_TOUR

namespace MultiTSP {
Tour::Tour(DistMatrix const &p_dists, Rating2Value const &p_rating2value,
           TeamSet const &p_teams, unsigned int p_max_places)
    : max_places(p_max_places), dists(p_dists), rating2value(p_rating2value),
      teams(p_teams), places_used(0), rating_is_valid(false) {
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
  if (!ids.empty()) {
    res += join<TeamId_t>(ids.begin(), ids.end());
  }
  res += "]}";
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
#ifdef TRACE_TOUR
  std::cerr << "{ RAW DIST: " << drive_time_to_next << " - " << entry_time
            << "} ";
#endif
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
  if (!rating_is_valid) {
    rating_cached = internal_compute_rating();
    rating_is_valid = true;
  }
  return rating_cached;
}

Rating Tour::internal_compute_rating() const {
#ifdef TRACE_TOUR
  std::cerr << "Compute rating Tour " << to_json() << " ";
#endif

  Rating rating;

  if (ids.empty()) {
    return rating;
  }

  unsigned int spaces_used(0);

  // Distances in between
  for (unsigned int i(0); i < ids.size() - 1; ++i) {
#ifdef TRACE_TOUR
    std::cerr << "CR1 " << i << " " << rating.as_json() << std::endl;
#endif
    spaces_used = add_to_rating(rating, spaces_used, dists, teams[ids[i]],
                                teams[ids[i + 1]].get_id());
  }
#ifdef TRACE_TOUR
  std::cerr << "CR2 " << rating.as_json() << std::endl;
#endif
  // Distances to school
  add_to_rating(rating, spaces_used, dists, teams[ids.back()], TeamId_t(0));
#ifdef TRACE_TOUR
  std::cerr << "CR3 " << rating.as_json() << std::endl;
#endif
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
    Rating const rating(internal_compute_rating());
    Value const value(rating * rating2value);

    if (value < optimal_value) {
#ifdef TRACE_TOUR
      std::cout << "New Best " << value << std::endl;
#endif
      optimal_value = value;
      opt = ids;
      rating_cached = rating;
    }
  } while (std::next_permutation(ids.begin(), ids.end()));

  ids = opt;
}

bool Tour::try_swap(Tour &other) {
  std::vector<TeamId_t> both_ids(ids);
  both_ids.insert(std::end(both_ids), std::begin(other.ids),
                  std::end(other.ids));
#ifdef TRACE_TOUR
  std::cerr << "BothIds " << join<TeamId_t>(both_ids.begin(), both_ids.end())
            << std::endl;
#endif
  random_shuffle(std::begin(both_ids), std::end(both_ids));
#ifdef TRACE_TOUR
  std::cerr << "BothIds shuffle"
            << join<TeamId_t>(both_ids.begin(), both_ids.end()) << std::endl;
#endif

  Tour n1(dists, rating2value, teams, max_places);
  Tour n2(dists, rating2value, teams, max_places);

  bool skip_first_n1(true);
  for (auto const bid : both_ids) {
    unsigned int bid_size(teams[bid].get_size());
#ifdef TRACE_TOUR
    std::cerr << "bid " << bid.get() << std::endl;
    std::cerr << "Check size " << bid_size << std::endl;
    std::cerr << "N1 " << n1.as_json() << std::endl;
    std::cerr << "N2 " << n2.as_json() << std::endl;
#endif

    skip_first_n1 = !skip_first_n1;

    if (!skip_first_n1) {
      if (n1.fit(bid_size)) {
        n1.push_back(bid);
        continue;
      }
    }
    if (n2.fit(bid_size)) {
      n2.push_back(bid);
      continue;
    }
    if (n1.fit(bid_size)) {
      n1.push_back(bid);
      continue;
    }
    return false;
  }

#ifdef TRACE_TOUR
  std::cerr << "N1 final " << n1.as_json() << std::endl;
  std::cerr << "N2 final " << n2.as_json() << std::endl;
#endif

  places_used = n1.places_used;
  ids = n1.ids;
  rating_is_valid = false;

  other.places_used = n2.places_used;
  other.ids = n2.ids;
  other.rating_is_valid = false;

  return true;
}

} // namespace MultiTSP
