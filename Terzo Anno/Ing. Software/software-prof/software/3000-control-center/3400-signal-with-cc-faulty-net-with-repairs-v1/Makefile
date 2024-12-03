
CC=g++
#CFLAGS=-std=c++11 -lm -I. -I/usr/include/postgresql -lpq 
#CFLAGS=-std=c++11 -I. -I/usr/include/postgresql -lpq -lm 
CFLAGS=-std=c++11 -I.  -lm 


DEPS = $(wildcard *.h)
objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

%.o:	%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: 	$(objects)
	$(CC) -o main $(objects) $(CFLAGS)


clear:
	rm *.o 


clearall:
	rm main ; rm *.o 
