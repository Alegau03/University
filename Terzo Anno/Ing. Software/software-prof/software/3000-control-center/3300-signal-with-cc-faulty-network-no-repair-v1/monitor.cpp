#include "main.h"



/* initial state */
void Monitor::init() {
  
  x[xvalue] = 0; x[counter] = 0;

}  // init()



void Monitor::next() {

  if ((u[u_from_ctr] == red) && (u[u_from_trl] != red) && (x[counter] < 3))
  { x[counter] = x[counter] + 1; }

  if (x[counter] >= 3)
    {x[xvalue] = 1;}
    
  if ((u[u_from_ctr] == red) && (u[u_from_trl] == red) && (x[counter] < 3))
    { x[counter] = 0; }
  
} // next()


void Monitor::output() {
      y[ymon] = x[xvalue];

}

