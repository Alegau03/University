
#ifndef main_h
#define main_h

using namespace std;

#include <iostream>
#include <random>

#define DEBUG 1000

#include "global.h"
#include "virtual-dtmc.h"

#include "control-center.h"
#include "dtmc.h"
#include "network.h"
#include "monitor.h"

#include "simulator.h"


#define HORIZON 10000   // horizon in seconds 

 /* Types for input and output values  */
  enum Color {nop, green, orange, red};

#endif
