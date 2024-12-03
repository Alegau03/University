#include "main.h"

void Simulator::run(double T)
{

  Global p;
  
  TrafficLight mc1(&p, "mc1");
  double t;
  int offset;
  FILE *fp;
  
  printf("seed: %u\n", p.seed);
  printf("initialization: t = %lf\n", t);

  mc1.init(); 
  
  fp = fopen("logfile.csv", "w");
  /* print first row log file */
  fprintf(fp, "col1-time ");
  offset = 2;
  offset = mc1.fprintf_first_line(fp, offset);
  fprintf(fp, "\n");
    
  for (t = 0; t <= T; t = t + (p.TimeStep))
  {
    mc1.output(); 

    /* connections  */
    
     /* print data log file */
    fprintf(fp, "%lf ", t);
    mc1.fprintflog(fp);
    fprintf(fp, "\n");

    /* update state */
    mc1.next();
    
  }   // for ()

  fclose(fp);
  
}  //  run()





