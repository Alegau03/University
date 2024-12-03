#include "main.h"

void Simulator::run(int T)
{

  RandGen p;
  
  DTMC mc1(&p, "mc1");
  DTMC2 mc2(&p, "mc2");
  
  int t = 0;
  int offset;
  FILE *fp;
  
  printf("seed: %u\n", p.seed);
  printf("initialization: t = %d\n", t);

  mc1.init(); mc2.init();
  
  fp = fopen("logfile.csv", "w");
  /* print first row log file */
  fprintf(fp, "col1-time ");
  offset = 2;
  offset = mc1.fprintf_first_line(fp, offset);
  offset = mc2.fprintf_first_line(fp, offset);
  fprintf(fp, "\n");
    
  for (t = 0; t <= T; t++)
  {
    mc1.output(); mc2.output();

    /* connections  */
    mc1.u[0] = mc2.y[0];  mc1.u[1] = mc2.y[1];
    mc2.u[0] = mc1.y[0];  mc2.u[1] = mc1.y[1];

    
     /* print data log file */
    fprintf(fp, "%d ", t);
    mc1.fprintflog(fp);
    mc2.fprintflog(fp);
    fprintf(fp, "\n");

    /* update state */
    mc1.next();
    mc2.next();

    
  }   // for ()

  fclose(fp);
  
}  //  run()





