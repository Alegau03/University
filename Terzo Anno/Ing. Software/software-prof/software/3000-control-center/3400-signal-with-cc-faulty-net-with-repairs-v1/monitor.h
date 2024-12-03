#ifndef monitor_h
#define monitor_h

#include "main.h"


class Monitor
{

public:

    Global *p;  // pointer to global utilities

  const char *sysname;

 /* Types for input and output  */
  enum Color {red, green, orange};

  
  /* input vars */
  int usize = 2; // input size

  int u_from_ctr;
  int u_from_trl;

  /* output vars */
  int ysize = 1; // num output vars

  int y;
    
  Monitor(Global *p, const char *mysysname);
    
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
  int x;
  int counter;
};


#endif
