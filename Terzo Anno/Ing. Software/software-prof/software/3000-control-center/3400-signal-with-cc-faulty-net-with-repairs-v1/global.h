#ifndef global_h
#define global_h

#include "main.h"

class Global {

public:

  Global();
  
  unsigned int seed;

  double TimeStep = 1.0;
  
  double get_random_double();

  int get_random_int_1();

  int get_random_int_2();

  
};



#endif
