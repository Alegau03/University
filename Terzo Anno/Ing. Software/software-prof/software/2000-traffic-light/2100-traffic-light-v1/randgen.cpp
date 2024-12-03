#include "main.h"

#include "randgen.h"


// Create a random device and use it to generate a random seed
random_device myRandomDevice;
unsigned int myseed = myRandomDevice();

// Initialize a default_random_engine with the seed
default_random_engine myRandomEngine(myseed);
 
// Initialize a uniform_int_distribution to produce values between -10 and 10
uniform_int_distribution<int> myUnifIntDist_1(-10, 10);

// Initialize a uniform_int_distribution to produce values between 60 and 120
uniform_int_distribution<int> myUnifIntDist_2(60, 120);

// Initialize a uniform_real_distribution to produce values between 0 and 1
uniform_real_distribution<double> myUnifRealDist_1(0.0, 1.0);

RandGen::RandGen() {
  
seed = myseed;
 
}


double RandGen::get_random_double() {

  /* update state */

  return(myUnifRealDist_1(myRandomEngine));
   
} // get_random_double()


int RandGen::get_random_int_1() {
  return(myUnifIntDist_1(myRandomEngine)); 
} // get_random_double()


int RandGen::get_random_int_2() {
  return(myUnifIntDist_2(myRandomEngine)); 
} // get_random_double()
