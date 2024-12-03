
#include "main.h"


int main()
{

  Simulator q;
  
#if 0
  random_device myRandomDevice;
  unsigned int seed = myRandomDevice();
    
// Initialize a default_random_engine with the seed
  default_random_engine myRandomEngine(seed);

  // Initialize a uniform_int_distribution to produce values between -10 and 10
  uniform_int_distribution<int> myUnifIntDist(-10, 10);

// Initialize a uniform_real_distribution to produce values between 0 and 1
  uniform_real_distribution<double> myUnifRealDist(0.0, 1.0);
#endif
  
  /*  prg  */
  
#if (DEBUG > 0)
  setvbuf(stdout, (char*) NULL, _IONBF, 0);
  setvbuf(stderr, (char*) NULL, _IONBF, 0);
#endif


  q.run(HORIZON);
   
}  /*  main()  */

