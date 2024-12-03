#include "main.h"

void Simulator::run(int T)
{

  RandGen p;
  
  DTMC mc1(&p);
  
  int t = 0;
  FILE *fp;
  
  printf("seed: %u\n", p.seed);
  printf("initialization: t = %d\n", t);

  mc1.init();
  
  fp = fopen("logfile.csv", "w");
 
  for (t = 0; t <= T; t++)
  {
    mc1.output();
    mc1.u[0] = t;  mc1.u[1] = mc1.y[1] + 1;
    mc1.printlog(t);
    mc1.fprintflog(fp, t);

    mc1.next(); 

#if 0
   // body of system here
    printf("get state of system at time %d\n", t);
    printf("get output of system at time %d\n", t);

    printf("update inputs with outputs at time %d\n", t);
    printf("update state of system with inputs at time %d\n\n", t);
#endif
    
  }   // for ()

  fclose(fp);
  
}  //  run()





