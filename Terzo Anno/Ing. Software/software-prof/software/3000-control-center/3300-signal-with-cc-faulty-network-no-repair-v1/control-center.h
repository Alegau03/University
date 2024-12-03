#ifndef control_center_h
#define control_center_h

#include "main.h"


class ControlCenter : public VIRTUAL_DTMC
{

public:

  enum OutputVar {color, output_size};
  enum InputVar {input_size};

  #include "varsize-public.h"
      
 // define initial state
  void init();

  // update state
  void next();
  
  // update output
  void output();

  ControlCenter(Global *p, const char *mysysname) :
  VIRTUAL_DTMC(p, mysysname){}
    
private:
  
  enum StateVar {timer, semaphore, state_size};

  #include "varsize-private.h"
  
};


#endif
