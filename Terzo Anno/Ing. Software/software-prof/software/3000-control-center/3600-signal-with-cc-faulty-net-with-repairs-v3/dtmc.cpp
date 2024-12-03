#include "main.h"



/* initial state */
void TrafficLight::init() {

  x[semaphore] = red;

}  // init()



void TrafficLight::next() {

  x[semaphore] = u[cmd];
  
} // next()


void TrafficLight::output() {

    y[color] = x[semaphore];

}



#if 0
int DTMC::xsize(){return(state_size);}

int DTMC::usize(){return(input_size);}

int DTMC::ysize(){return(output_size);}
#endif  
