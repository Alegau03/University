#include "main.h"

void Simulator::run(int T)
{

  RandGen p;
  
  DTMC mc1(2, 0, 1, &p, "mc1");
  int t = 0;
  int offset;
  FILE *fp;
  
  printf("seed: %u\n", p.seed);
  printf("initialization: t = %d\n", t);

  mc1.init(); 
  
  fp = fopen("logfile.csv", "w");
  /* print first row log file */
  fprintf(fp, "col1-time ");
  offset = 2;
  offset = mc1.fprintf_first_line(fp, offset);
  fprintf(fp, "\n");
    
  for (t = 0; t <= T; t++)
  {
    mc1.output(); 

    /* connections  */
    
     /* print data log file */
    fprintf(fp, "%d ", t);
    mc1.fprintflog(fp);
    fprintf(fp, "\n");

    /* update state */
    mc1.next();
    
  }   // for ()

  fclose(fp);
  
}  //  run()





