#include "main.h"

void Simulator::run(double T)
{

  Global p;
  
  ControlCenter ctr(&p, "ctr");
  TrafficLight mc1(&p, "trl");
  Network ntw(&p, "ntw");
  Monitor mon(&p, "mon");
  
  double t;
  int offset;
  FILE *fp;
  
  printf("seed: %u\n", p.seed);
  printf("initialization: t = %lf\n", t);

  ctr.init(); mc1.init(); ntw.init(); mon.init();
  
  fp = fopen("logfile.csv", "w");
  /* print first row log file */
  fprintf(fp, "col1-time ");
  offset = 2;
  offset = ctr.fprintf_first_line(fp, offset);
  offset = ntw.fprintf_first_line(fp, offset);
  offset = mc1.fprintf_first_line(fp, offset);
  offset = mon.fprintf_first_line(fp, offset);
   fprintf(fp, "\n");
    
  for (t = 0; t <= T; t = t + (p.TimeStep))
  {
    mc1.output(); 
    ctr.output(); 
    ntw.output(); 
    mon.output(); 

    /* connections  */
    ntw.u = ctr.color;
    mc1.u = (TrafficLight::Color) ntw.y;
    mon.u_from_ctr = ctr.color;
    mon.u_from_trl = mc1.color;
    
     /* print data log file */
    fprintf(fp, "%lf ", t);
    ctr.fprintflog(fp);
    ntw.fprintflog(fp);
    mc1.fprintflog(fp);
    mon.fprintflog(fp);
     fprintf(fp, "\n");

    /* update state */
    ctr.next();
    mc1.next();
    ntw.next();
    mon.next();
    
  }   // for ()

  fclose(fp);
  
}  //  run()




