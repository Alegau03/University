#ifndef dtmc_h
#define dtmc_h

#include "main.h"


class DTMC  : public VIRTUAL_DTMC
{

public:

  /* state names */
  int pippo = 0;
  int pluto = 1;

  /* input names */
  int paperino = 0;
  int paperoga = 1;
  
  /* output names */
  int qui = 0;
  int quo = 1;


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
