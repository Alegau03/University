#ifndef randgen_h
#define randgen_h

#include "main.h"

class RandGen {

public:

  RandGen();
  
  unsigned int seed;
  
  double get_random_double();

  int get_random_int();

  
};



#endif
