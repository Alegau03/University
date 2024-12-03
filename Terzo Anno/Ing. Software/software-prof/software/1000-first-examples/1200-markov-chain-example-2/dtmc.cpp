#include "main.h"


void DTMC::init() {

  /* initial state */

  int i;

  for (i = 0; i < xsize; i++)
    {
      x[i] = 0;
    }
  

}  // init()



void DTMC::next() {

  /* update state */

  int i;

  for (i = 0; i < xsize; i++)
    {
      x[i] = u[i]*(p -> get_random_double());
    }
  

} // next()


void DTMC::output() {

  /* update state */

  int i;

  for (i = 0; i < ysize; i++)
    {
      y[i] = x[i];
    }
}

