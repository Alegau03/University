
#ifndef main_h
#define main_h

using namespace std;

#include <iostream>
#include <random>

#include "randgen.h"
#include "dtmc.h"
#include "simulator.h"


#if 0
#include "clock.h"
#include "logger.h"
#include "gtime.h"
#include "timer.h"
#include "basic.h"
#include "system.h"

#include "ctr.h"
#include "plant.h"
#include "root.h"
#endif


#define DEBUG 1000

#define HORIZON 10

#if 0
extern Logger val2log;
extern GlobalTime gtime;
extern Clock ck;

extern random_device myRandomDevice;
extern unsigned int seed;
extern default_random_engine myRandomEngine;
extern uniform_int_distribution<int> myUnifIntDist;
extern uniform_real_distribution<double> myUnifRealDist;
#endif


#if 0
  random_device myRandomDevice;
  unsigned int seed = myRandomDevice();
    
// Initialize a default_random_engine with the seed
  default_random_engine myRandomEngine(seed);

// Initialize a uniform_int_distribution to produce values between -10 and 10
  uniform_int_distribution<int> myUnifIntDist(-10, 10);

// Initialize a uniform_real_distribution to produce values between 0 and 1
  uniform_real_distribution<double> myUnifRealDist(0.0, 1.0);
#endif



#endif
