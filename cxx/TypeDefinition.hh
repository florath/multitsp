#ifndef MULTI_TSP_TYPE_DEFINITION_HH
#define MULTI_TSP_TYPE_DEFINITION_HH

#include <utility>

template <typename T, typename Parameter> class TypeDefinition {
public:
  explicit TypeDefinition(T const &pv) : v(pv) {}
  explicit TypeDefinition(T &&pv) : v(std::move(pv)) {}
  T &get() { return v; }
  T const &get() const { return v; }

private:
  T v;
};

#endif
