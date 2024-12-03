#ifndef dtmc2_h
#define dtmc2_h

#include "main.h"


class DTMC2 : public VIRTUAL_DTMC
{
public:
  
  DTMC2(RandGen *p, const char *mysysname) :
  VIRTUAL_DTMC(p, mysysname){}
  
  DTMC2(int xsize, int usize, int ysize, RandGen *p, const char *mysysname) :
  VIRTUAL_DTMC(xsize, usize, ysize, p, mysysname){}
  
 // define initial state
  void init();

  // update state
  void next();
  
  // update output
  void output();
};


#endif
