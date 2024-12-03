#include "main.h"


void DTMC::init() {

  /* initial state */

  int i;

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

  /* get signal from control center */

  x[semaphore] = u[from_control_center];
 
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

