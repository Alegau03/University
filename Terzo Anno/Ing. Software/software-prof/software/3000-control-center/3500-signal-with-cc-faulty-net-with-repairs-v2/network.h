#ifndef network_h
#define network_h

#include "main.h"


class Network : public VIRTUAL_DTMC
{

public:

  enum OutputVar {yvalue, output_size};
  enum InputVar {uvalue, input_size};

 // define initial state
  void init();

  // update state
  void next();
  
  // update output
  void output();

  Network(Global *p, const char *mysysname) :
  VIRTUAL_DTMC(p, mysysname){}

  
private:
  
  enum StateVar {xvalue, faulty, state_size};
 
  int xsize(){return(state_size);};
  int usize(){return(input_size);};
  int ysize(){return(output_size);};
  
};


#endif
