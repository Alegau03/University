#include "main.h"


/* initial state */
void Network::init() {
  
  x[xvalue] = 0; x[faulty] = 0;

}  // init()



void Network::next() {

  if ((p -> get_random_double()) <= 0.001)
    {x[faulty] = 1;}

#if 0
  // repair
  if ((p -> get_random_double()) <= 0.001)
    {x[faulty] = 0;}
#endif
  
  if (x[faulty] <= 0)
    {x[xvalue] = u[uvalue]; }
  else // fault
    {x[xvalue] = 0; }
   
  
} // next()


void Network::output() {
      y[yvalue] = x[xvalue];

}


