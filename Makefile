
CC = g++ -g

all: lab4

sim.o: sim.cc
	$(CC) -c sim.cc

lab4: sim.o
	$(CC) -o lab4 sim.o -ll

test.o: test.cc
	$(CC) -c test.cc

test: test.o
	$(CC) -o test test.o -ll	

clean:
	rm -f test lab4 *.o