#ifndef MULTI_TSP_RATING2VALUE_HH
#define MULTI_TSP_RATING2VALUE_HH

namespace MultiTSP {

class Rating2Value {
public:
  // Sets everything to 0.
  Rating2Value(double p_weight_tour_length = 0.0,
               double p_weight_length_of_stay = 0.0)
      : weight_tour_length(p_weight_tour_length),
        weight_length_of_stay(p_weight_length_of_stay) {}

  double get_weight_tour_length() const { return weight_tour_length; }
  double get_weight_length_of_stay() const { return weight_length_of_stay; }

  void set_weight_tour_length(double wtl) { this->weight_tour_length = wtl; }
  void set_weight_length_of_stay(double wlos) {
    this->weight_length_of_stay = wlos;
  }

private:
  double weight_tour_length;
  double weight_length_of_stay;
};

} // namespace MultiTSP

#endif
