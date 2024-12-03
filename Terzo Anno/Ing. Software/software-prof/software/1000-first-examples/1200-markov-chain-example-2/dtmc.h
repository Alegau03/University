#ifndef dtmc_h
#define dtmc_h

#include "main.h"


class DTMC  : public VIRTUAL_DTMC
{

public:
  
  DTMC(RandGen *p, const char *mysysname) :
  VIRTUAL_DTMC(p, mysysname){}
  
  DTMC(int xsize, int usize, int ysize, RandGen *p, const char *mysysname) :
  VIRTUAL_DTMC(xsize, usize, ysize, p, mysysname){}

  
 // define initial state
  void init();

  // update state
  void next();
  
  // update output
  void output();
};


#endif
