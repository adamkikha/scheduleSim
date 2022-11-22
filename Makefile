
CC = g++ -g

all: lab4

sim.o: sim.cc
	$(CC) -c sim.cc

lab4: sim.o
	$(CC) -o lab4 sim.o -ll	

clean:
	rm -f lab4 sim.o