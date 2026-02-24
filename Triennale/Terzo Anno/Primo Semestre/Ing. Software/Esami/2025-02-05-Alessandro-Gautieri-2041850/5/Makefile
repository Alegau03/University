CC := g++
CFLAGS := -lm -O3 -std=c++11

MOCC_SRC := $(wildcard ../mocc/*.cpp) 
MOCC := $(MOCC_SRC:.cpp=.o) 

LOCAL_SRC := $(wildcard *.cpp) 
LOCAL := $(LOCAL_SRC:.cpp=.o)


main: $(MOCC) $(LOCAL)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean 

clean:
	rm main  *.o ../mocc/*.o