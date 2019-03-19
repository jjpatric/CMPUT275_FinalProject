CFLAGS=-c -Wall -O2 -std=c++11
PROGS=map
OBJS=map.o

all: $(PROGS)

map: map.o digraph.o
	g++ map.o -o map

map.o: map.cpp building.h
	g++ map.cpp -o map.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	g++ digraph.cpp -o digraph.o $(CFLAGS)

clean:
	@rm $(PROGS) $(OBJS)