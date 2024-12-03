#include "main.h"

Network::Network(Global *myp, const char *mysysname) {
  p = myp;
  sysname = mysysname;
}



/* initial state */
void Network::init() {
  
  x = 0; faulty = 0;

}  // init()



void Network::next() {

  if ((p -> get_random_double()) <= 0.0009)
    {faulty = 1;}

  if ((p -> get_random_double()) <= 0.001)
    {faulty = 0;}
  
  if (!faulty)
    {x = u; }
  
} // next()


void Network::output() {
      y = x;

}

int Network::fprintf_first_line(FILE *fp, int offset) {
  //  FILE *fp;
  int i, k;

  k = offset;

  /* print state */
  fprintf(fp, "col%d-%s.x ", k++, sysname);
  fprintf(fp, "col%d-%s.faulty ", k++, sysname);

  /* print output */
  fprintf(fp, "col%d-%s.y ", k++, sysname);

  /* print input, if any */
  fprintf(fp, "col%d-%s.u ", k++, sysname);

  return(k);

}


void Network::fprintflog(FILE *fp) {


 /* print state */
 fprintf(fp, "%d ", x);
 fprintf(fp, "%d ", faulty);
 
 /* print output */
 fprintf(fp, "%d ", y);

 /* print input, if any */
 fprintf(fp, "%d ", u);

}
