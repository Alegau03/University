#ifndef control_center_h
#define control_center_h

#include "main.h"


class Control_Center  : public VIRTUAL_DTMC
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


  Control_Center(RandGen *p, const char *mysysname) :
  VIRTUAL_DTMC(p, mysysname){}
  
  Control_Center(int xsize, int usize, int ysize, RandGen *p, const char *mysysname) :
  VIRTUAL_DTMC(xsize, usize, ysize, p, mysysname){}

  
 // define initial state
  void init();

  // update state
  void next();
  
  // update output
  void output();
};


#endif
