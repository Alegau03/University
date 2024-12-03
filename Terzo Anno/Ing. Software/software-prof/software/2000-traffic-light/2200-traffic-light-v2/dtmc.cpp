#include "main.h"

TrafficLight::TrafficLight(Global *myp, const char *mysysname) {

  p = myp;
  sysname = mysysname;
}



/* initial state */
void TrafficLight::init() {
  
  timer = (p -> get_random_int_2());
  semaphore = red;

}  // init()



void TrafficLight::next() {

  /* update state */

  int i;

  /* update timer*/
  if (timer > 0)
    {
      timer = timer - (p -> TimeStep);
    }
  else /* update */
    {
      semaphore = (Color) ((((int) semaphore) +1)%3) ;
      timer = (p -> get_random_int_2());;   // re-init
    }
 
} // next()


void TrafficLight::output() {

    color = semaphore;

}

int TrafficLight::fprintf_first_line(FILE *fp, int offset) {
  //  FILE *fp;
  int i, k;

  k = offset;

  /* print state */
  fprintf(fp, "col%d-%s.timer ", k++, sysname);
  fprintf(fp, "col%d-%s.semaphore ", k++, sysname);

  /* print output */
  fprintf(fp, "col%d-%s.color ", k++, sysname);

  /* print input, if any */

  return(k);

}


void TrafficLight::fprintflog(FILE *fp) {


 /* print state */
 fprintf(fp, "%lf ", timer);
 fprintf(fp, "%d ", semaphore);

 
 /* print output */
 fprintf(fp, "%d ", color);

 
 /* print input, if any */

}
