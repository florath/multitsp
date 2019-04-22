CC=gcc
CXX=g++
DEBUG=-ggdb -O0 -march=native
OPT=-O3
CFLAGS:=$(DEBUG) $(OPT) -W -Wall -Wextra -Wpedantic -pedantic
CXXFLAGS:=-std=c++17 $(DEBUG) $(OPT) -W -Wall -Wextra -Wpedantic -pedantic -I.
LDLIBS:=-l:libboost_program_options.so.1.67.0 
OUTPUT_OPTION=-MMD -MP -o $@

SRC:=$(wildcard *.cc)
OBJ:=$(SRC:.cc=.o)
DEP:=$(SRC:.cc=.d)

all: multitsp check_input tests/UnitTest-Tour

-include $(DEP)

.PHONY: clean

multitsp: main.o Config.o Team.o State.o Tour.o Rating.o
	${CXX} $^ $(LOADLIBES) $(LDLIBS) -o $@

check_input: check_input.o Config.o Team.o
	${CXX} $^ $(LOADLIBES) $(LDLIBS) -o $@

tests/UnitTest-Tour: tests/UnitTest-Tour.o Tour.o Team.o Rating.o State.o
	${CXX} $^ $(LOADLIBES) -l:libboost_unit_test_framework.so.1.67.0 -o $@

clean:
	-rm -f $(OBJ) $(DEP) multitsp tests/UnitTest-Tour check_input

tests: all
	tests/UnitTest-Tour
