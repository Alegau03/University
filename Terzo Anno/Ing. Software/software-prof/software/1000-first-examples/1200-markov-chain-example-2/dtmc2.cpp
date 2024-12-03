#include "main.h"


void DTMC2::init() {

  /* initial state */

  int i;

  for (i = 0; i < xsize; i++)
    {
      x[i] = 0;
    }
  

}  // init()



void DTMC2::next() {

  /* update state */

  int i;

  for (i = 0; i < xsize; i++)
    {
      x[i] = u[i] + (p -> get_random_double());
    }
  

} // next()


void DTMC2::output() {

  /* update state */

  int i;

  for (i = 0; i < ysize; i++)
    {
      y[i] = x[i];
    }
}





