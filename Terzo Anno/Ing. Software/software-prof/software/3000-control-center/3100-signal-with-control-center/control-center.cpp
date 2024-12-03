#include "main.h"


void Control_Center::init() {

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



void Control_Center::next() {

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


void Control_Center::output() {

  /* update state */

  int i;

  /* there are network faults, so the update may or may not take place */

  if ((p -> get_random_double()) <= 0.9)
    {
       y[color] = x[semaphore];
    }


  /*  
  for (i = 0; i < ysize; i++)
    {
      y[i] = x[i];
    }
  */
}

