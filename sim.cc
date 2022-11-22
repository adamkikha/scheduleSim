#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include "sim.h"
using namespace std;

Queue::Queue(int size){
	array = (Process **) malloc( size * sizeof( Process * ) );
	head = 0;
	tail = 0;
	size = 0;
}

void Queue::push( Process * process ){
	array[tail++] = process;
	size++;
}

Process * Queue::pop(){
	if (size) {
		size--;
		return array[head++];
	}
	return NULL;
}

Simulation::Simulation(){
	noOfProcesses = 0;
	noOfAlgos = 0;
	avalAlgos = 8;
	algos = (Algorithm **) malloc( avalAlgos * sizeof( Algorithm * ) );
}

Process::Process(char* n,int a,int s,int i){
	strcpy(name,n);
	arrival = a;
	service = s;
	index = i;
}

void Simulation::insertProcess( Process * process ){
	processes[noOfProcesses] = process;
	noOfProcesses++;
}

void Simulation::insertAlgo( Algorithm * algo ){
	if ( avalAlgos == noOfAlgos ) {
		avalAlgos *= 2;
		algos = (Algorithm **) realloc( algos , avalAlgos * sizeof( Algorithm * ) );
	}
	algos[ noOfAlgos ] = algo;
	noOfAlgos++;
}










void FCFS(Simulation sim,int param){
	Process * ps = (Process *) malloc( sim.noOfProcesses * sizeof(Process) );
	for (int i = 0; i < sim.noOfProcesses; i++) ps[i] = *(sim.processes[i]);

	if(!sim.traceOrStats){
		float ** mat = new float *[sim.noOfProcesses];
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[sim.simLength]{};
		int start = 0;
		int clock = 0;
		Queue * readyQ = new Queue(sim.noOfProcesses+sim.simLength);
		Process * p;
		
		// Admission
		while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
		while (clock < sim.simLength)
		{
			
			// Selection
			p = readyQ->pop();

			// Service
			if (p){
				while(p->service){
					p->service--;
					mat[p->index][clock] = 2;
					for (int i = readyQ->head; i < readyQ->tail; i++)
					{
						mat[readyQ->array[i]->index][clock] = 1;
					}
					clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				}
			} else {
				if (start < sim.noOfProcesses){
					while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				} else clock = sim.simLength;
			}
		}
		sim.print("FCFS",ps,mat,sim.traceOrStats,sim.noOfProcesses,sim.simLength);
	}else{
		float ** mat = new float *[sim.noOfProcesses];
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[3]{};
		int start = 0;
		int clock = 0;
		Queue * readyQ = new Queue(sim.noOfProcesses+sim.simLength);
		Process * p;
		
		// Admission
		while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
		while (clock < sim.simLength)
		{
			
			// Selection
			p = readyQ->pop();

			// Service
			if (p){
				while(p->service){
					p->service--;
					clock++;
					if(!p->service){
						mat[p->index][0] = clock;
						mat[p->index][1] = clock - (p->arrival);
						mat[p->index][2] = mat[p->index][1] / ((float) ps[p->index].service) ;
					}
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				}
			} else {
				if (start < sim.noOfProcesses){
					while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				} else clock = sim.simLength;
			}
		}
		
		sim.print("FCFS",ps,mat,sim.traceOrStats,sim.noOfProcesses,3);
	}
}




void RR(Simulation sim,int param){
	Process * ps = (Process *) malloc( sim.noOfProcesses * sizeof(Process) );
	for (int i = 0; i < sim.noOfProcesses; i++) ps[i] = *(sim.processes[i]);

	if(!sim.traceOrStats){
		float ** mat = new float *[sim.noOfProcesses];
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[sim.simLength]{};
		int start = 0;
		int clock = 0;
		Queue * readyQ = new Queue(sim.noOfProcesses+sim.simLength);
		Process * p;
		
		// Admission
		while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
		while (clock < sim.simLength)
		{
			
			// Selection
			p = readyQ->pop();

			// Service
			if (p){
				for(int i=0;i < param && p->service;i++){
					p->service--;
					mat[p->index][clock] = 2;
					for (int i = readyQ->head; i < readyQ->tail; i++)
					{
						mat[readyQ->array[i]->index][clock] = 1;
					}
					clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				}
				if(p->service) readyQ->push(p);
			} else {
				if (start < sim.noOfProcesses){
					while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				} else clock = sim.simLength;
			}
		}
		char * name = (char *) malloc(5*sizeof(char));
		sprintf(name,"RR-%d",param);
		sim.print(name,ps,mat,sim.traceOrStats,sim.noOfProcesses,sim.simLength);
	}else{
		float ** mat = new float *[sim.noOfProcesses];
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[3]{};
		int start = 0;
		int clock = 0;
		Queue * readyQ = new Queue(sim.noOfProcesses+sim.simLength);
		Process * p;
		
		// Admission
		while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
		while (clock < sim.simLength)
		{
			
			// Selection
			p = readyQ->pop();

			// Service
			if (p){
				for(int i=0;i < param && p->service;i++){
					p->service--;
					clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				}
				if(p->service) readyQ->push(p);
				else {
					mat[p->index][0] = clock;
					mat[p->index][1] = clock - (p->arrival);
					mat[p->index][2] = mat[p->index][1] / ((float) ps[p->index].service) ;
				}
			} else {
				if (start < sim.noOfProcesses){
					while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				} else clock = sim.simLength;
			}
		}
		char * name = (char *) malloc(5*sizeof(char));
		sprintf(name,"RR-%d",param);
		sim.print(name,ps,mat,sim.traceOrStats,sim.noOfProcesses,3);
	}	
}



void SPN(Simulation sim,int param){
	Process * ps = (Process *) malloc( sim.noOfProcesses * sizeof(Process) );
	for (int i = 0; i < sim.noOfProcesses; i++) ps[i] = *(sim.processes[i]);

	if(!sim.traceOrStats){
		float ** mat = new float *[sim.noOfProcesses];
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[sim.simLength]{};
		int start = 0;
		int clock = 0;
		int minService;
		int minIndex = 0;
		Process * temp;
		Queue * readyQ = new Queue(sim.noOfProcesses+sim.simLength);
		Process * p;
		
		// Admission
		while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
		while (clock < sim.simLength)
		{
			// Selection
			minService = sim.simLength;
			for (int i = readyQ->head; i < readyQ->tail; i++)
			{
				if(readyQ->array[i]->service < minService){
					minService = readyQ->array[i]->service;
					minIndex = i;
				} else if (readyQ->array[i]->service == minService){
					if (readyQ->array[i]->index < readyQ->array[minIndex]->index) minIndex = i;
				}
			}
			temp = readyQ->array[readyQ->head];
			readyQ->array[readyQ->head] = readyQ->array[minIndex];
			readyQ->array[minIndex] = temp;

			p = readyQ->pop();

			// Service
			if (p){
				while(p->service){
					p->service--;
					mat[p->index][clock] = 2;
					for (int i = readyQ->head; i < readyQ->tail; i++)
					{
						mat[readyQ->array[i]->index][clock] = 1;
					}
					clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				}
			} else {
				if (start < sim.noOfProcesses){
					while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				} else clock = sim.simLength;
			}
		}
		sim.print("SPN",ps,mat,sim.traceOrStats,sim.noOfProcesses,sim.simLength);
	}else{
		float ** mat = new float *[sim.noOfProcesses];
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[3]{};
		int start = 0;
		int clock = 0;
		int minService;
		int minIndex = 0;
		Process * temp;
		Queue * readyQ = new Queue(sim.noOfProcesses+sim.simLength);
		Process * p;
		
		// Admission
		while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
		while (clock < sim.simLength)
		{
			
			// Selection
			minService = sim.simLength;
			for (int i = readyQ->head; i < readyQ->tail; i++)
			{
				if(readyQ->array[i]->service < minService){
					minService = readyQ->array[i]->service;
					minIndex = i;
				} else if (readyQ->array[i]->service == minService){
					if (readyQ->array[i]->index < readyQ->array[minIndex]->index) minIndex = i;
				}
			}
			temp = readyQ->array[readyQ->head];
			readyQ->array[readyQ->head] = readyQ->array[minIndex];
			readyQ->array[minIndex] = temp;

			p = readyQ->pop();

			// Service
			if (p){
				while(p->service){
					p->service--;
					clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				}
				mat[p->index][0] = clock;
				mat[p->index][1] = clock - (p->arrival);
				mat[p->index][2] = mat[p->index][1] / ((float) ps[p->index].service) ;
			} else {
				if (start < sim.noOfProcesses){
					while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				} else clock = sim.simLength;
			}
		}
		
		sim.print("SPN",ps,mat,sim.traceOrStats,sim.noOfProcesses,3);
	}
}





void SRT(Simulation sim,int param){
	Process * ps = (Process *) malloc( sim.noOfProcesses * sizeof(Process) );
	for (int i = 0; i < sim.noOfProcesses; i++) ps[i] = *(sim.processes[i]);

	if(!sim.traceOrStats){
		float ** mat = new float *[sim.noOfProcesses];
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[sim.simLength]{};
		int start = 0;
		int clock = 0;
		int minService;
		int minIndex = 0;
		Queue * readyQ = new Queue(sim.noOfProcesses+sim.simLength);
		Process * p;
		
		// Admission
		while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
		while (clock < sim.simLength)
		{
			
			// Selection
			minService = sim.simLength;
			for (int i = readyQ->head; i < readyQ->tail; i++)
			{
				if(readyQ->array[i]){
					if(readyQ->array[i]->service < minService){
						minService = readyQ->array[i]->service;
						minIndex = i;
					}
				}
			}
			
			p = readyQ->array[minIndex];
			readyQ->array[minIndex] = nullptr;
			// Service
			if (p){
				if (start < sim.noOfProcesses){
					while( sim.processes[start]->arrival != clock && p->service){
						p->service--;
						mat[p->index][clock] = 2;
						for (int i = 0; i < readyQ->tail; i++)
						{
							if(readyQ->array[i]) mat[readyQ->array[i]->index][clock] = 1;
						}
						clock++;
					}
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
					if(p->service) readyQ->push(p);
				} else {
					while(p->service){
						p->service--;
						mat[p->index][clock] = 2;
						for (int i = readyQ->head; i < readyQ->tail; i++)
						{
							if(readyQ->array[i]) mat[readyQ->array[i]->index][clock] = 1;
						}
						clock++;
					}
				}
			} else {
				if (start < sim.noOfProcesses){
					while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				} else clock = sim.simLength;
			}
		}

		sim.print("SRT",ps,mat,sim.traceOrStats,sim.noOfProcesses,sim.simLength);
	}else{
		float ** mat = new float *[sim.noOfProcesses];
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[3]{};
		int start = 0;
		int clock = 0;
		int minService;
		int minIndex = 0;
		Process * temp;
		Queue * readyQ = new Queue(sim.noOfProcesses+sim.simLength);
		Process * p;
		
		// Admission
		while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
		while (clock < sim.simLength)
		{
			
			// Selection
			minService = sim.simLength;
			for (int i = readyQ->head; i < readyQ->tail; i++)
			{
				if(readyQ->array[i]){
					if(readyQ->array[i]->service < minService){
						minService = readyQ->array[i]->service;
						minIndex = i;
					}
				}
			}
			
			p = readyQ->array[minIndex];
			readyQ->array[minIndex] = nullptr;
			// Service
			if (p){
				if (start < sim.noOfProcesses){
					while(sim.processes[start]->arrival != clock && p->service){
						p->service--;
						clock++;
					}
					if(!p->service) {
						mat[p->index][0] = clock;
						mat[p->index][1] = clock - (p->arrival);
						mat[p->index][2] = mat[p->index][1] / ((float) ps[p->index].service) ;
					}
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
					if(p->service) readyQ->push(p);
				} else {
					while(p->service){
						p->service--;
						clock++;
					}
					mat[p->index][0] = clock;
					mat[p->index][1] = clock - (p->arrival);
					mat[p->index][2] = mat[p->index][1] / ((float) ps[p->index].service) ;
				}
			} else {
				if (start < sim.noOfProcesses){
					while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				} else clock = sim.simLength;
			}
		}

		sim.print("SRT",ps,mat,sim.traceOrStats,sim.noOfProcesses,3);
	}
}



void HRRN(Simulation sim,int param){
	Process * ps = (Process *) malloc( sim.noOfProcesses * sizeof(Process) );
	for (int i = 0; i < sim.noOfProcesses; i++) ps[i] = *(sim.processes[i]);

	if(!sim.traceOrStats){
		float ** mat = new float *[sim.noOfProcesses];
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[sim.simLength]{};
		int start = 0;
		int clock = 0;
		float ratio;
		float maxRatio;
		int maxIndex = 0;
		Process * temp;
		Queue * readyQ = new Queue(sim.noOfProcesses+sim.simLength);
		Process * p;
		
		// Admission
		while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
		while (clock < sim.simLength)
		{
			// Selection
			maxRatio = 1;
			for (int i = readyQ->head; i < readyQ->tail; i++)
			{
				ratio = ( (float) (clock - readyQ->array[i]->arrival) / (float) readyQ->array[i]->service) + 1;
				if(ratio > maxRatio){
					maxRatio = ratio;
					maxIndex = i;
				} else if (ratio == maxRatio){
					if (readyQ->array[i]->index < readyQ->array[maxIndex]->index) maxIndex = i;
				}
			}
			temp = readyQ->array[readyQ->head];
			readyQ->array[readyQ->head] = readyQ->array[maxIndex];
			readyQ->array[maxIndex] = temp;

			p = readyQ->pop();

			// Service
			if (p){
				while(p->service){
					p->service--;
					mat[p->index][clock] = 2;
					for (int i = readyQ->head; i < readyQ->tail; i++)
					{
						mat[readyQ->array[i]->index][clock] = 1;
					}
					clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				}
			} else {
				if (start < sim.noOfProcesses){
					while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				} else clock = sim.simLength;
			}
		}
		sim.print("HRRN",ps,mat,sim.traceOrStats,sim.noOfProcesses,sim.simLength);
	}else{
		float ** mat = new float *[sim.noOfProcesses];
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[3]{};
		int start = 0;
		int clock = 0;
		float ratio;
		float maxRatio;
		int maxIndex = 0;
		Process * temp;
		Queue * readyQ = new Queue(sim.noOfProcesses+sim.simLength);
		Process * p;
		
		// Admission
		while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
		while (clock < sim.simLength)
		{
			
			// Selection
			maxRatio = 1;
			for (int i = readyQ->head; i < readyQ->tail; i++)
			{
				ratio = ( (float) (clock - readyQ->array[i]->arrival) / (float) readyQ->array[i]->service) + 1;
				if(ratio > maxRatio){
					maxRatio = ratio;
					maxIndex = i;
				} else if (ratio == maxRatio){
					if (readyQ->array[i]->index < readyQ->array[maxIndex]->index) maxIndex = i;
				}
			}
			temp = readyQ->array[readyQ->head];
			readyQ->array[readyQ->head] = readyQ->array[maxIndex];
			readyQ->array[maxIndex] = temp;

			p = readyQ->pop();

			// Service
			if (p){
				while(p->service){
					p->service--;
					clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				}
				mat[p->index][0] = clock;
				mat[p->index][1] = clock - (p->arrival);
				mat[p->index][2] = mat[p->index][1] / ((float) ps[p->index].service) ;
			} else {
				if (start < sim.noOfProcesses){
					while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQ->push(sim.processes[start++]);
				} else clock = sim.simLength;
			}
		}
		
		sim.print("HRRN",ps,mat,sim.traceOrStats,sim.noOfProcesses,3);
	}
}
void FB1(Simulation sim,int param){
	Process * ps = (Process *) malloc( sim.noOfProcesses * sizeof(Process) );
	for (int i = 0; i < sim.noOfProcesses; i++) ps[i] = *(sim.processes[i]);
	float ** mat = new float *[sim.noOfProcesses];
	if(sim.traceOrStats){
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[3]{};
	} else for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[sim.simLength]{};
	Queue *readyQs[5] = {new Queue(sim.noOfProcesses+sim.simLength),new Queue(sim.noOfProcesses+sim.simLength),new Queue(sim.noOfProcesses+sim.simLength),new Queue(sim.noOfProcesses+sim.simLength),new Queue(sim.noOfProcesses+sim.simLength)};
	int start = 0;
	int clock = 0;
	int selected = 0;
	int count = 0;
	Process * p;

	// Admission
	while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) {
		readyQs[0]->push(sim.processes[start++]);
		count++;
	}
	while (clock < sim.simLength){

		selected = 0;
		for (int i = 0; i < 5; i++)
		{
			if(readyQs[i]->size){
				selected = 1;
				p = readyQs[i]->pop();
				count--;
				p->service--;
				if (!sim.traceOrStats)
				{
					mat[p->index][clock] = 2;
					for (int q = 0; q < 5; q++)
					{
						for (int j = readyQs[q]->head; j < readyQs[q]->tail; j++)
						{
							mat[readyQs[q]->array[j]->index][clock] = 1;
						}
					}
					clock++;
				} else {
					clock++;
					if(!p->service){
						mat[p->index][0] = clock;
						mat[p->index][1] = clock - (p->arrival);
						mat[p->index][2] = mat[p->index][1] / ((float) ps[p->index].service) ;
					}
				}
				while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) {
					readyQs[0]->push(sim.processes[start++]);
					count++;
				}
				if(p->service){
					if(count) readyQs[(i == 4 ? i : i+1)]->push(p);
					else readyQs[i]->push(p);
				}
				break;
			}
		}
		if(!selected){
			if (start < sim.noOfProcesses){
				while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
				while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQs[0]->push(sim.processes[start++]);
			} else clock = sim.simLength;
		}
	}
	if(sim.traceOrStats) sim.print("FB-1",ps,mat,sim.traceOrStats,sim.noOfProcesses,3);
	else sim.print("FB-1",ps,mat,sim.traceOrStats,sim.noOfProcesses,sim.simLength);		
}


void FB2(Simulation sim,int param){
	Process * ps = (Process *) malloc( sim.noOfProcesses * sizeof(Process) );
	for (int i = 0; i < sim.noOfProcesses; i++) ps[i] = *(sim.processes[i]);
	float ** mat = new float *[sim.noOfProcesses];
	if(sim.traceOrStats){
		for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[3]{};
	} else for (int i = 0; i < sim.noOfProcesses; ++i) mat[i] = new float[sim.simLength]{};
	Queue *readyQs[5] = {new Queue(sim.noOfProcesses+sim.simLength),new Queue(sim.noOfProcesses+sim.simLength),new Queue(sim.noOfProcesses+sim.simLength),new Queue(sim.noOfProcesses+sim.simLength),new Queue(sim.noOfProcesses+sim.simLength)};
	int start = 0;
	int clock = 0;
	int selected = 0;
	int count = 0;
	int quant[5] = {1,2,4,8,16};
	Process * p;

	// Admission
	while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) {
		readyQs[0]->push(sim.processes[start++]);
		count++;
	}
	while (clock < sim.simLength){

		selected = 0;
		for (int i = 0; i < 5; i++)
		{
			if(readyQs[i]->size){
				selected = 1;
				p = readyQs[i]->pop();
				count--;
				for(int c=0;c < quant[i] && p->service;c++){
					p->service--;
					if (!sim.traceOrStats)
					{
						mat[p->index][clock] = 2;
						for (int q = 0; q < 5; q++)
						{
							for (int j = readyQs[q]->head; j < readyQs[q]->tail; j++)
							{
								mat[readyQs[q]->array[j]->index][clock] = 1;
							}
						}
						clock++;
					} else {
						clock++;
						if(!p->service){
							mat[p->index][0] = clock;
							mat[p->index][1] = clock - (p->arrival);
							mat[p->index][2] = mat[p->index][1] / ((float) ps[p->index].service) ;
						}
					}
					while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) {
						readyQs[0]->push(sim.processes[start++]);
						count++;
					}
				}
				if(p->service){
					if(count) readyQs[(i == 4 ? i : i+1)]->push(p);
					else readyQs[i]->push(p);
				}
				break;
			}
		}
		if(!selected){
			if (start < sim.noOfProcesses){
				while (sim.processes[start]->arrival != clock && clock < sim.simLength) clock++;
				while (start < sim.noOfProcesses && sim.processes[start]->arrival == clock) readyQs[0]->push(sim.processes[start++]);
			} else clock = sim.simLength;
		}
	}
	if(sim.traceOrStats) sim.print("FB-2i",ps,mat,sim.traceOrStats,sim.noOfProcesses,3);
	else sim.print("FB-2i",ps,mat,sim.traceOrStats,sim.noOfProcesses,sim.simLength);

}
void AGE(Simulation sim,int param){
	if(sim.traceOrStats){
		
	}else{
		
	}
}












void (*schedulers[8])(Simulation sim,int p) = {FCFS,RR,SPN,SRT,HRRN,FB1,FB2,AGE};
Simulation Simulation::currentSim;
Process * Simulation::currentProcess;
Algorithm * Simulation::currentAlgo;

void Simulation::print(char * algo,Process * processes,float ** mat,int traceOrStats,int rows,int columns){
	if(!traceOrStats){
		printf("%s", algo);
		for (int i = 0; i < 6-strlen(algo); i++) printf(" ");
		for (int i = 0; i <= columns; i++) printf("%d ",i%10);
		printf("\n");
		for (int i = 0; i < (columns*2)+8; i++) printf("-");
		printf("\n");
		for (int i = 0; i < rows; i++)
		{
			printf("%s     |",processes[i].name);
			for (int j = 0; j < columns; j++)
			{
				printf("%s|",mat[i][j] == 0? " " : (mat[i][j] == 1? ".":"*"));
			}
			printf(" \n");
		}
		for (int i = 0; i < (columns*2)+8; i++) printf("-");
		printf("\n");
		printf("\n");
	} else{
		printf("%s\n", algo);
		printf("Process    |");
		for (int i = 0; i < rows; i++) printf("  %s  |",processes[i].name);
		printf("\n");
		
		printf("Arrival    |");
		for (int i = 0; i < rows; i++) printf(" %s%d  |",processes[i].arrival > 9? "":" ",processes[i].arrival);
		printf("\n");
		
		printf("Service    |");
		for (int i = 0; i < rows; i++) printf(" %s%d  |",processes[i].service > 9? "":" ",processes[i].service);
		printf(" Mean|\n");
		
		printf("Finish     |");
		for (int i = 0; i < rows; i++) printf(" %s%.0f  |",mat[i][0] > 9? "":" ",mat[i][0]);
		printf("-----|\n");
		
		printf("Turnaround |");
		for (int i = 0; i < rows; i++) printf(" %s%.0f  |",mat[i][1] > 9? "":" ",mat[i][1]);
		float sumT = 0;
		for (int i = 0; i < rows; i++) sumT+= mat[i][1];
		sumT /= rows;
		printf("%s%.2f|\n",sumT >= 10? "":" ",sumT);
		
		printf("NormTurn   |");
		for (int i = 0; i < rows; i++) printf("%s%.2f|",mat[i][2] >= 10? "":" ",mat[i][2]);
		float sumNT = 0;
		for (int i = 0; i < rows; i++) sumNT+= mat[i][2];
		sumNT /= rows;
		printf("%s%.2f|\n",sumNT >= 10? "":" ",sumNT);
		
		printf("\n");
	}
}

void parse(){
	char line[50];
	char * tok;
	
	cin.getline(line,50);
	Simulation::currentSim.traceOrStats = (strcmp(line,"trace") != 0);

	cin.getline(line,50);
	tok = strtok(line,",-");
	while (tok != NULL){
		Simulation::currentAlgo = new Algorithm();
		Simulation::currentAlgo->type = atoi(tok);
		if (!strcmp(tok,"2") || !strcmp(tok,"8")){
			tok = strtok(NULL,",");
			Simulation::currentAlgo->parameter = atoi(tok);
		}
		Simulation::currentSim.insertAlgo( Simulation::currentAlgo );
		tok = strtok(NULL,",-");
	}
	
	cin.getline(line,50);
	Simulation::currentSim.simLength = atoi(line);
	
	cin.getline(line,50);
	int noOflines = atoi(line);
	
	Simulation::currentSim.processes = (Process **) malloc( noOflines * sizeof( Process * ) );
	for (int i = 0; i < noOflines; i++){
		cin.getline(line,50);
		char *name = strtok(line,",");
		int arrival = atoi(strtok(NULL,","));
		int service = atoi(strtok(NULL,","));
		Simulation::currentProcess = new Process(name,arrival,service,Simulation::currentSim.noOfProcesses);
		Simulation::currentSim.insertProcess( Simulation::currentProcess );
	}

}

void Simulation::simulate(){
	int service[noOfProcesses];
	for (int i = 0; i < noOfProcesses; i++) service[i] = processes[i]->service;
	for(int k = 0 ; k < noOfAlgos ; k++){
		void (*schedule)(Simulation sim,int p) = ::schedulers[(algos[k]->type)-1];
		int param = algos[k]->parameter;
		
		schedule(Simulation::currentSim,param);

		for (int i = 0; i < noOfProcesses; i++) processes[i]->service = service[i];
	}
}

int main(){
	parse();
	Simulation::currentSim.simulate();
}