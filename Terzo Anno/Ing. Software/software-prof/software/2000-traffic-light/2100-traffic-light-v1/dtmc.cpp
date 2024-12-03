#include "main.h"


void DTMC::init() {

  /* initial state */

  int i;

  x[timer] = (p -> get_random_int_2());
  x[semaphore] = red;

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

  /* update timer*/
  if (x[timer] > 0)
    {
      x[timer] = x[timer] - 1;
    }
  else /* update */
    {
      x[semaphore] = (x[semaphore] >= red) ? green : (x[semaphore] + 1) ;
      x[timer] = (p -> get_random_int_2());   // re-init
    }
 
} // next()


void DTMC::output() {

  /* update state */

  int i;

    y[color] = x[semaphore];

  /*  
  for (i = 0; i < ysize; i++)
    {
      y[i] = x[i];
    }
  */
}

