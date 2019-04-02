CC=g++
LFLAGS=
CFLAGS=-c -Wall -std=c++11 -O2
OBJS=digraph.o server.o serialport.o map.o
EXE=server

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LFLAGS)

server.o: server.cpp wdigraph.h map.h
	$(CC) server.cpp -o server.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

serialport.o: serialport.cpp serialport.h 
	$(CC) serialport.cpp -o serialport.o $(CFLAGS)

map.o: map.cpp map.h serialport.h wdigraph.h unit.h
	$(CC) map.cpp -o map.o $(CFLAGS)

clean:
	@rm -f $(OBJS) $(EXE)
