#ifndef dtmc_h
#define dtmc_h

#include "main.h"


class TrafficLight : public VIRTUAL_DTMC
{

public:

  enum OutputVar {color, output_size};
  enum InputVar {cmd, input_size};


  /* Types for input and output  */
  enum Color {red, green, orange};


  
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
 
  int xsize(){return(state_size);};
  int usize(){return(input_size);};
  int ysize(){return(output_size);};
  
  
};


#endif
