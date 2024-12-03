#!/bin/bash
# You should use Makefiles instead ;)

OPT=-O3
FLAGS=-DERR_CHK

gcc -Wall -g ${OPT} ${FLAGS} -o inputgen inputgen.c -lm
gcc -Wall -g ${OPT} ${FLAGS} -o blackscholes blackscholes.c -lm
gcc-14 -Wall -g ${OPT} ${FLAGS} -o blackscholes_omp solution/blackscholes_omp.c -lm -fopenmp
gcc -Wall -g ${OPT} ${FLAGS} -o blackscholes_pthreads solution/blackscholes_pthreads.c -lm -pthread

