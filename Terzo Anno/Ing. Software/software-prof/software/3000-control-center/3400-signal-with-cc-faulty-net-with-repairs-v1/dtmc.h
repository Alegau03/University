#ifndef dtmc_h
#define dtmc_h

#include "main.h"


class TrafficLight  
{

public:

    Global *p;  // pointer to global utilities

  const char *sysname;


  /* Types for input and output  */
  enum Color {red, green, orange};

  
  /* input vars */
  int usize = 1; // input size

  Color u;

  /* output vars */
  int ysize = 1; // num output vars

  Color color;
  
  
  TrafficLight(Global *p, const char *mysysname);
    
 // define initial state
  void init();

  // update state
  void next();
  
  // update output
  void output();

  int fprintf_first_line(FILE *fp, int offset);
  void fprintflog(FILE *fp);
  
private:
  
  int xsize = 1; // num of state vars

  /* state vars  */
  Color semaphore;
  
};


#endif
