.DEFAULT_GOAL := all
# Simple makefile create executable(.out) from every .c file
CC = g++
LIB = libutility.so

# SRC will hold an array of all the .c files
SRCS := $(subst ./,,$(shell find . -name "*.cpp"))

# OBJS will hold an array of the corresponding .out to the .c files
OBJS := $(filter-out utility.out, $(patsubst %.cpp,%.out,$(SRCS)))

$(info $(OBJS))

# all will be the default TARGET and will create all the object files
.PHONY: all
all: $(OBJS) $(LIB)

$(LIB): utility.cpp	
	$(CC) -fPIC -shared utility.cpp -o $(LIB) 

# This rule will be executed for every .out file present in the target "all"
%.out: %.cpp $(LIB)
	$(CC) $< $(LIB) -o $@

# Easy way to remove all binaries
.PHONY: clean
clean:
	find . -name "*.out" -exec rm -rf {} \;
	find . -name "*.so" -exec rm -rf {} \;