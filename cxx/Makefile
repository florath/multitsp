CC=gcc
CXX=g++
DEBUG=-ggdb -O0 -march=native
CFLAGS:=$(DEBUG) -W -Wall -Wextra -Wpedantic -pedantic -ansi
LDLIBS:=-l:libboost_program_options.so.1.67.0
OUTPUT_OPTION=-MMD -MP -o $@

SRC:=$(wildcard *.cc)
OBJ:=$(SRC:.cc=.o)
DEP:=$(SRC:.cc=.d)

all: multitsp

-include $(DEP)

.PHONY: clean

multitsp: main.o Config.o
	${CXX} $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	-rm -f $(OBJ) $(DEP) multitsp
