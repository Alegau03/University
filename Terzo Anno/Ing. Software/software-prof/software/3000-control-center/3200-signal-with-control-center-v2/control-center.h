#ifndef control_center_h
#define control_center_h

#include "main.h"


class ControlCenter
{

public:

    Global *p;  // pointer to global utilities

  const char *sysname;


  /* Types for input and output  */
  enum Color {red, green, orange};

  
  /* input vars */
  int usize = 0; // input size


  /* output vars */
  int ysize = 1; // num output vars

  Color color;
  
  
  ControlCenter(Global *p, const char *mysysname);
    
 // define initial state
  void init();

  // update state
  void next();
  
  // update output
  void output();

  int fprintf_first_line(FILE *fp, int offset);
  void fprintflog(FILE *fp);
  
private:
  
  int xsize = 2; // num of state vars

  /* state vars  */
  double timer;
  Color semaphore;
  
};


#endif
