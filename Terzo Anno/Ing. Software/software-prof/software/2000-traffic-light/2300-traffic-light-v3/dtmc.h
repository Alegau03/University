#ifndef dtmc_h
#define dtmc_h

#include "main.h"


class DTMC  : public VIRTUAL_DTMC
{

public:

  
  /* colors  */
  double green = 0;
  double orange = 1;
  double red = 2;
  

  DTMC(RandGen *p, const char *mysysname) :
  VIRTUAL_DTMC(p, mysysname){}
  
  
 // define initial state
  void init();

  // update state
  void next();
  
  // update output
  void output();

private:

 enum StateVar {timer, semaphore, state_size};
 enum OutputVar {color, output_size};
 enum InputVar {input_size};

  
 int xsize();
 int usize();
 int ysize();
  
  
};


#endif
