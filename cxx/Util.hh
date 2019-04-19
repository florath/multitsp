#ifndef MULTI_TSP_UTIL_HH
#define MULTI_TSP_UTIL_HH

#include <numeric>
#include <string>
#include <vector>

namespace MultiTSP {

template <typename ValueType, typename InputIt>
inline std::string join(InputIt begin, InputIt end) {
  InputIt second(begin);
  ++second;
  return std::accumulate(second, end, std::to_string(*begin),
                         [](std::string const &a, ValueType b) {
                           return a + ", " + std::to_string(b);
                         });
}

} // namespace MultiTSP

#endif
