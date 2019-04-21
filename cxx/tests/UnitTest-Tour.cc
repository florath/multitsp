#define BOOST_TEST_MODULE UnitTest_Tour
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "Rating2Value.hh"
#include "Tour.hh"
#include "State.hh"

class UnitTestTour {
public:
  UnitTestTour()
      : dists{{0, 1, 2, 3}, {5, 0, 7, 11}, {13, 17, 0, 19}, {23, 29, 31, 0}},
        rating2value(1.0, 1.0), teams({{MultiTSP::TeamId_t(1), 1, 47},
                                       {MultiTSP::TeamId_t(2), 1, 53},
                                       {MultiTSP::TeamId_t(3), 1, 59}}),
        tour(dists, rating2value, teams, 4) {}

  MultiTSP::DistMatrix const dists;
  MultiTSP::Rating2Value const rating2value;
  MultiTSP::TeamSet teams;
  MultiTSP::Tour tour;
};

BOOST_AUTO_TEST_CASE(tour_empty) {
  UnitTestTour const utt;
  MultiTSP::Rating const rating(utt.tour.compute_rating());
  BOOST_TEST(rating == MultiTSP::Rating(0, 0));
}

BOOST_AUTO_TEST_CASE(tour_one) {
  UnitTestTour utt;
  utt.tour.push_back(MultiTSP::TeamId_t(1));
  MultiTSP::Rating const rating(utt.tour.compute_rating());
  BOOST_TEST(rating == MultiTSP::Rating(52, 52));
}

BOOST_AUTO_TEST_CASE(tour_two) {
  UnitTestTour utt;
  utt.tour.push_back(MultiTSP::TeamId_t(1));
  utt.tour.push_back(MultiTSP::TeamId_t(2));
  MultiTSP::Rating const rating(utt.tour.compute_rating());
  BOOST_TEST(rating == MultiTSP::Rating(120, 186));
}

BOOST_AUTO_TEST_CASE(tour_three) {
  UnitTestTour utt;
  utt.tour.push_back(MultiTSP::TeamId_t(1));
  utt.tour.push_back(MultiTSP::TeamId_t(2));
  utt.tour.push_back(MultiTSP::TeamId_t(3));
  MultiTSP::Rating const rating(utt.tour.compute_rating());
  BOOST_TEST(rating == MultiTSP::Rating(208, 444));
}

BOOST_AUTO_TEST_CASE(tour_state_local_optimize) {
  UnitTestTour utt;
  MultiTSP::State state(1, 4, utt.rating2value, utt.teams, utt.dists);

  MultiTSP::Rating const rating_state_before(state.compute_rating());
  BOOST_TEST(rating_state_before == MultiTSP::Rating(208, 444));
  state.optimize_local();
  MultiTSP::Rating const rating_state_after(state.compute_rating());
  state.as_json(std::cerr);
  BOOST_TEST(rating_state_after == MultiTSP::Rating(212, 386));
}

