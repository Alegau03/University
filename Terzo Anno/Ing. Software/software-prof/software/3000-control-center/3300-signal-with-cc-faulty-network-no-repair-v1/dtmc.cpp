#include "main.h"



/* initial state */
void TrafficLight::init() {

  x[semaphore] = red;

}  // init()



void TrafficLight::next() {

  if (u[cmd] != nop)
    // command arrived
    {x[semaphore] = u[cmd];}
  else //network fault
    {x[semaphore] = red;}
  
} // next()


void TrafficLight::output() {

    y[color] = x[semaphore];

}

