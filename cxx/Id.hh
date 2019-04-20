#ifndef MULTI_TSP_TYPE_ID_HH
#define MULTI_TSP_TYPE_ID_HH

#include "TypeDefinition.hh"

namespace MultiTSP {

  // This is a dedicated type which starts counting at 1
  // e.g. the first team is team 1
  // (This is historical, because entry 0 was the target place.)
  using TeamId_t = TypeDefinition<unsigned int, struct Team_Id>;

  
}

#endif
