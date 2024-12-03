#include "main.h"


void DTMC::init() {

  /* initial state */

  int i;

  for (i = 0; i < xsize; i++)
    {
      x[i] = 2;
    }
  

}  // init()



void DTMC::next() {

  /* update state */

  int i;

  x[0] = 0.7*x[0] + 0.7*x[1];
  x[1] = -0.7*x[0] + 0.7*x[1];
  
  
  /*
  for (i = 0; i < xsize; i++)
    {
      x[i] = u[i]*(p -> get_random_double());
    }
  */ 

} // next()


void DTMC::output() {

  /* update state */

  int i;

  for (i = 0; i < ysize; i++)
    {
      y[i] = x[i];
    }
}

