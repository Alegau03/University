#include "main.h"

#include "dtmc.h"


typedef double state_type;
typedef double input_type;
typedef double output_type;

void VIRTUAL_DTMC::dtmc_body(RandGen *myp, const char *mysysname) {

  p = myp;
  sysname = mysysname;
  
  if (xsize <= 0)
    {
     fprintf(stderr, "DTMC(): Error: state size cannot be 0\n");
     exit(1);
    }
  
x = (state_type *) malloc(sizeof(state_type)*xsize);

 if (x == NULL)
   {
     fprintf(stderr, "DTMC(): memory allocation for state failed\n");
     exit(1);
   }


  if (usize <= 0)
    {
      u = NULL;
    }
  else
    {
 u = (input_type *) malloc(sizeof(input_type)*usize);

 if (u == NULL)
   {
     fprintf(stderr, "DTMC(): memory allocation for input failed\n");
     exit(1);
   }
    } // if (usize <= 0)
 
  if (ysize <= 0)
    {
     fprintf(stderr, "DTMC(): Error: output size cannot be 0\n");
     exit(1);
    }

  y = (output_type *) malloc(sizeof(output_type)*ysize);

 if (y == NULL)
   {
     fprintf(stderr, "DTMC(): memory allocation for output failed\n");
     exit(1);
   }


 
}



VIRTUAL_DTMC::VIRTUAL_DTMC(RandGen *myp, const char *mysysname)  {
  dtmc_body(myp, mysysname);
}


VIRTUAL_DTMC::VIRTUAL_DTMC(int myxsize, int myusize, int myysize, RandGen *myp, const char *mysysname)  {

  xsize = myxsize;
  usize = myusize;
  ysize = myysize;

  dtmc_body(myp, mysysname);
  
}

int VIRTUAL_DTMC::fprintf_first_line(FILE *fp, int offset) {
  //  FILE *fp;
  int i, k;
  
  /*  prg  */

  //  fp = fopen(filename, "w");

  //  fprintf(fp, "%d ", t);

  k = offset;
  
  for (i = 0; i < xsize; i++)
    {
      fprintf(fp, "col%d-%s.x[%d] ", k++, sysname, i);
    }
  

  for (i = 0; i < ysize; i++)
    {
      fprintf(fp, "col%d-%s.y[%d] ", k++, sysname, i);
    }

    for (i = 0; i < usize; i++)
    {
      fprintf(fp, "col%d-%s.u[%d] ", k++, sysname, i);
    }

    return(k);
    
}  // fprintf_first_line()



void VIRTUAL_DTMC::fprintflog(FILE *fp) {

  /* update state */

  //  FILE *fp;
  int i;
  
  /*  prg  */

  //  fp = fopen(filename, "w");

  //  fprintf(fp, "%d ", t);

  for (i = 0; i < xsize; i++)
    {
      fprintf(fp, "%lf ", x[i]);
    }
  

  for (i = 0; i < ysize; i++)
    {
     fprintf(fp, "%lf ", y[i]);
    }

    for (i = 0; i < usize; i++)
    {
     fprintf(fp, "%lf ", u[i]);
    }

    //    fprintf(fp, "\n");
   
} // next()




void VIRTUAL_DTMC::init() {}  // init()

void VIRTUAL_DTMC::next() {} // next()

void VIRTUAL_DTMC::output() {}  // output




