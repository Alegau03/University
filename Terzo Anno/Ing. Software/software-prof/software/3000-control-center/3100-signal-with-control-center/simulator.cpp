#include "main.h"

void Simulator::run(int T)
{

  RandGen p;
  
  DTMC plant(1, 1, 1, &p, "plant");
  DTMC ctr(2, 0, 1, &p, "ctr");
  int t = 0;
  int offset;
  FILE *fp;
  
  printf("seed: %u\n", p.seed);
  printf("initialization: t = %d\n", t);

  plant.init(); ctr.init(); 
  
  fp = fopen("logfile.csv", "w");
  /* print first row log file */
  fprintf(fp, "col1-time ");
  offset = 2;
  offset = plant.fprintf_first_line(fp, offset);
  offset = ctr.fprintf_first_line(fp, offset);
  fprintf(fp, "\n");
    
  for (t = 0; t <= T; t++)
  {
    plant.output();   ctr.output(); 

    /* connections  */
    plant.u[plant.from_control_center] = ctr.y[ctr.semaphore];
      
     /* print data log file */
    fprintf(fp, "%d ", t);
    plant.fprintflog(fp);
    ctr.fprintflog(fp);
    fprintf(fp, "\n");

    /* update state */
    plant.next(); ctr.next();
    
  }   // for ()

  fclose(fp);
  
}  //  run()





