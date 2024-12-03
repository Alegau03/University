#ifndef virtual_dtmc_h
#define virtual_dtmc_h

#include "main.h"

class VIRTUAL_DTMC {

private:
  
  void dtmc_body(Global *p, const char *mysysname);

  virtual int xsize();
  virtual int usize();
  virtual int ysize();

public:

  Global *p;  // pointer to random generator class

  const char *sysname;

  double *u;  // input
  double *y;  // output

  VIRTUAL_DTMC(Global *p, const char *mysysname);
 

  int fprintf_first_line(FILE *fp, int offset);
  void fprintflog(FILE *fp);
    
  // define initial state
  virtual void init();

  // update state
  virtual void next();
  
  // update output
  virtual void output();

protected:
  
    double *x;  // present state

};


#endif
