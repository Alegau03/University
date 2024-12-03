#include "main.h"


void DTMC::init() {

  /* initial state */

  int i;

  x[pippo] = 2;
  x[pluto] = 2;

  /* 
  for (i = 0; i < xsize; i++)
    {
      x[i] = 2;
    }
  */

}  // init()



void DTMC::next() {

  /* update state */

  int i;

  x[pippo] = 0.7*x[pippo] + 0.7*x[pluto];
  x[pluto] = -0.7*x[pippo] + 0.7*x[pluto];
  
  
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

    y[qui] = x[pippo];
    y[quo] = x[pluto];

  /*  
  for (i = 0; i < ysize; i++)
    {
      y[i] = x[i];
    }
  */
}

