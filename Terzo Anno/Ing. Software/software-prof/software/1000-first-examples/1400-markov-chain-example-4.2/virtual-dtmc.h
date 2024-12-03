#ifndef virtual_dtmc_h
#define virtual_dtmc_h

#include "main.h"

class VIRTUAL_DTMC {

private:
  
  void dtmc_body(RandGen *p, const char *mysysname);

public:

  RandGen *p;  // pointer to random generator class

  const char *sysname;

  int xsize = 2; // state size
  int usize = 2; // input size
  int ysize = 2; // output size
  
  double *x;  // present state
  double *u;  // input
  double *y;  // output

  VIRTUAL_DTMC(RandGen *p, const char *mysysname);
  VIRTUAL_DTMC(int xsize, int usize, int ysize, RandGen *p, const char *mysysname);


  int fprintf_first_line(FILE *fp, int offset);
  void fprintflog(FILE *fp);
    
  // define initial state
  virtual void init();

  // update state
  virtual void next();
  
  // update output
  virtual void output();

  
};


#endif
