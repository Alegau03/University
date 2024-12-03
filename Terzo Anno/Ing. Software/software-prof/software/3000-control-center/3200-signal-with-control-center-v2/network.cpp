#include "main.h"

Network::Network(Global *myp, const char *mysysname) {
  p = myp;
  sysname = mysysname;
}



/* initial state */
void Network::init() {
  
  x = 0;

}  // init()



void Network::next() {

  x = u;
 
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

  /* print output */
  fprintf(fp, "col%d-%s.y ", k++, sysname);

  /* print input, if any */
  fprintf(fp, "col%d-%s.u ", k++, sysname);

  return(k);

}


void Network::fprintflog(FILE *fp) {


 /* print state */
 fprintf(fp, "%d ", x);
 
 /* print output */
 fprintf(fp, "%d ", y);

 /* print input, if any */
 fprintf(fp, "%d ", u);

}
