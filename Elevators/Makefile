CPP=g++
CC=gcc
CFLAGS=-g -I.
CPPFLAGS=-std=c++11 $(CFLAGS) 
LDFLAGS=

DEPS = 
SRC_DIR=.
OBJ_DIR=.
CSRC := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)
CPPSRC := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)
OBJ = $(filter %.o,$(CPPSRC:.cpp=.o) $(CSRC:.c=.o))

all: print elevators

print:
	@echo SRC $(CSRC) $(CPPSRC)
	@echo OBJ $(OBJ)
	@echo DEPS $(DEPS)
elevators: $(OBJ)
	$(CPP) $(LDFLAGS) -o $@ $^ 


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

clean:
	rm *.o elevators
