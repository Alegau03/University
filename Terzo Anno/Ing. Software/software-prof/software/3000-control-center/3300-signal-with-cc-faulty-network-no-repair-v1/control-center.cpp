#include "main.h"




/* initial state */
void ControlCenter::init() {
  
  x[timer] = (p -> get_random_int_2());
  x[semaphore] = red;

}  // init()



void ControlCenter::next() {

  /* update state */

  int i;

  /* update timer*/
  if (x[timer] > 0)
    {
      x[timer] = x[timer] - (p -> TimeStep);
    }
  else /* update */
    {
      x[semaphore] =  (x[semaphore] < 3) ? (x[semaphore] + 1) : green;
      x[timer] = (p -> get_random_int_2());   // re-init
    }
 
} // next()


void ControlCenter::output() {

    y[color] = x[semaphore];

}

