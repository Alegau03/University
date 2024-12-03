#ifndef network_h
#define network_h

#include "main.h"


class Network 
{

public:

    Global *p;  // pointer to global utilities

  const char *sysname;


  /* input vars */
  int usize = 1; // input size

  int u;

  /* output vars */
  int ysize = 1; // num output vars

  int y;
    
  Network(Global *p, const char *mysysname);
    
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
  int x;
  
};


#endif
