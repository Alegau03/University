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
  
  /* state names */
  int timer = 0;
  int semaphore = 1;

  /* input names */
  
  /* output names */
  int color = 0;


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
