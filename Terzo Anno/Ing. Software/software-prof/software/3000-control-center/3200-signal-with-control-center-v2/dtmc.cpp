#include "main.h"

TrafficLight::TrafficLight(Global *myp, const char *mysysname) {

  p = myp;
  sysname = mysysname;
}


/* initial state */
void TrafficLight::init() {

  semaphore = red;

}  // init()



void TrafficLight::next() {

  semaphore = u;
  
} // next()


void TrafficLight::output() {

    color = semaphore;

}

int TrafficLight::fprintf_first_line(FILE *fp, int offset) {
  //  FILE *fp;
  int i, k;

  k = offset;

  /* print state */
  fprintf(fp, "col%d-%s.semaphore ", k++, sysname);

  /* print output */
  fprintf(fp, "col%d-%s.color ", k++, sysname);

  /* print input, if any */
  fprintf(fp, "col%d-%s.u ", k++, sysname);

  return(k);

}


void TrafficLight::fprintflog(FILE *fp) {


 /* print state */
 fprintf(fp, "%d ", semaphore);

 
 /* print output */
 fprintf(fp, "%d ", color);

 
 /* print input, if any */
 fprintf(fp, "%d ", u);

}
