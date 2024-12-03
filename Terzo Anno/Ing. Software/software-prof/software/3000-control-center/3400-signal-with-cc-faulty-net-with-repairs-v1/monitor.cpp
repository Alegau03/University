#include "main.h"

Monitor::Monitor(Global *myp, const char *mysysname) {
  p = myp;
  sysname = mysysname;
}



/* initial state */
void Monitor::init() {
  
  x = 0; counter = 0;

}  // init()



void Monitor::next() {

  if ((u_from_ctr == red) && (u_from_trl != red) && (counter < 3))
  { counter++; }

  if (counter >= 3)
    {x = 1;}
    
  if ((u_from_ctr == red) && (u_from_trl == red) && (counter < 3))
    { counter = 0; }
  
} // next()


void Monitor::output() {
      y = x;

}

int Monitor::fprintf_first_line(FILE *fp, int offset) {
  //  FILE *fp;
  int i, k;

  k = offset;

  /* print state */
  fprintf(fp, "col%d-%s.x ", k++, sysname);
  fprintf(fp, "col%d-%s.counter ", k++, sysname);

  /* print output */
  fprintf(fp, "col%d-%s.y ", k++, sysname);

  /* print input, if any */
  fprintf(fp, "col%d-%s.u_from_ctr ", k++, sysname);
  fprintf(fp, "col%d-%s.u_from_trl ", k++, sysname);

  return(k);

}


void Monitor::fprintflog(FILE *fp) {


 /* print state */
 fprintf(fp, "%d ", x);
 fprintf(fp, "%d ", counter);
 
 /* print output */
 fprintf(fp, "%d ", y);

 /* print input, if any */
 fprintf(fp, "%d ", u_from_ctr);
 fprintf(fp, "%d ", u_from_trl);

}
