#ifndef monitor_h
#define monitor_h

#include "main.h"


class Monitor : public VIRTUAL_DTMC
{

public:

  enum OutputVar {ymon, output_size};
  enum InputVar {u_from_ctr, u_from_trl, input_size};

 /* Types for input and output  */
  enum Color {red, green, orange};

  
  Monitor(Global *p, const char *mysysname) :
  VIRTUAL_DTMC(p, mysysname){}
    
 // define initial state
  void init();

  // update state
  void next();
  
  // update output
  void output();

 
private:
  
  enum StateVar {xvalue, counter, state_size};
 
  int xsize(){return(state_size);};
  int usize(){return(input_size);};
  int ysize(){return(output_size);};


};


#endif
