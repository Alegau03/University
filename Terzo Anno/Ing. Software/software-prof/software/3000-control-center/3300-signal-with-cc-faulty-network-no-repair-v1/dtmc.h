#ifndef dtmc_h
#define dtmc_h

#include "main.h"


class TrafficLight : public VIRTUAL_DTMC
{

public:

  enum OutputVar {color, output_size};
  enum InputVar {cmd, input_size};

  #include "varsize-public.h"
 
 
  TrafficLight(Global *p, const char *mysysname) :
  VIRTUAL_DTMC(p, mysysname){}
    
 // define initial state
  void init();

  // update state
  void next();
  
  // update output
  void output();

private:
  
  enum StateVar {semaphore, state_size};

  #include "varsize-private.h"
};


#endif
