
#ifndef sim_h
#define sim_h

struct Process {
	char name[1];
	int arrival;
	int service;
	Process(char* n,int a,int s);
};

struct Algorithm {
	int type;
	int parameter;
};

struct Simulation {
	int traceOrStats;
	int simLength;
	int avalAlgos;
	int noOfAlgos;
	int noOfProcesses;
	Process ** processes;
	Algorithm ** algos;

	void print(int ** traces);
	void simulate();
	void insertProcess( Process * process );
	void insertAlgo( Algorithm * algo);

	Simulation();

	static Simulation currentSim;
	static Process *currentProcess;
	static Algorithm *currentAlgo;
};

struct Queue {
	Process ** array;
	int head;
	int tail;
	int size;

	void push( Process * process );
	Process * pop();
	Process ** inspect();

	Queue(int size);
};

#endif
