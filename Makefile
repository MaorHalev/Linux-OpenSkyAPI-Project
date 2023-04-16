.DEFAULT_GOAL := all
CC = g++
LIB = libutility.so

# SRC will hold an array of all the .cpp files
SRCS := $(subst ./,,$(shell find . -name "*.cpp"))

# OBJS will hold an array of the corresponding .out to the .cpp files (without utility.cpp)
OBJS := $(filter-out utility.out, $(patsubst %.cpp,%.out,$(SRCS)))

# all will be the default TARGET and will create all the object files and so library.
.PHONY: all
all: $(OBJS) $(LIB)

# This rule will be executed for utility.cpp utility.h a .os file present in the target "all" as LIB
$(LIB): utility.cpp	utility.h
	$(CC) -fPIC $< -o $@ -shared

# This rule will be executed for every .out file present in the target "all" and link them to .os LIB
%.out: %.cpp $(LIB)
	$(CC) $< $(LIB) -o $@

.PHONY: clean
clean:
	find . -name "*.out" -exec rm -rf {} \;
	find . -name "*.so" -exec rm -rf {} \;