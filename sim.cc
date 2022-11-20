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

Process ** Queue::inspect(){
	return array;
}

Simulation::Simulation(){
	noOfProcesses = 0;
	noOfAlgos = 0;
	avalAlgos = 8;
	algos = (Algorithm **) malloc( avalAlgos * sizeof( Algorithm * ) );
}

Process::Process(char* n,int a,int s){
	strcpy(name,n);
	arrival = a;
	service = s;
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

int FCFS(Simulation sim,int p){
	return 1;
}
int RR(Simulation sim,int p){
	return 2;	
}
int SPN(Simulation sim,int p){
	return 3;
}
int SRT(Simulation sim,int p){
	return 4;
}
int HRRN(Simulation sim,int p){
	return 5;
}
int FB1(Simulation sim,int p){
	return 6;
}
int FB2(Simulation sim,int p){
	return 7;
}
int AGE(Simulation sim,int p){
	return 8;
}

int (*schedulers[8])(Simulation sim,int p) = {FCFS,RR,SPN,SRT,HRRN,FB1,FB2,AGE};
Simulation Simulation::currentSim;
Process * Simulation::currentProcess;
Algorithm * Simulation::currentAlgo;

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
		Simulation::currentProcess = new Process(name,arrival,service);
		Simulation::currentSim.insertProcess( Simulation::currentProcess );
	}
	/*printf("traceOrStats= %d\nsimLength= %d\nnoOfAlgos= %d\nnoOfProcesses= %d\n",Simulation::currentSim.traceOrStats,Simulation::currentSim.simLength,Simulation::currentSim.noOfAlgos,Simulation::currentSim.noOfProcesses);
	printf("\nAlgos:\ntype parameter\n");
	for (int i = 0; i < Simulation::currentSim.noOfAlgos; ++i)
	{
		Algorithm * a = Simulation::currentSim.algos[i];
		printf("%d    %d\n", a->type,a->parameter);
	}
	printf("\nprocesses:\nname arrival service\n");
	for (int i = 0; i < Simulation::currentSim.noOfProcesses; ++i)
	{
		Process * p = Simulation::currentSim.processes[i];
		printf("%s    %d       %d\n", p->name,p->arrival,p->service);
	}*/
}

void Simulation::simulate(){
	int service[noOfProcesses];
	for (int i = 0; i < noOfProcesses; i++) service[i] = processes[i]->service;
	for(int k = 0 ; k < noOfAlgos ; k++){
		int (*schedule)(Simulation sim,int p) = ::schedulers[(algos[k]->type)-1];
		int param = algos[k]->parameter;
		
		schedule(Simulation::currentSim,param);

		for (int i = 0; i < noOfProcesses; i++) processes[i]->service = service[i];
	}
}

int main(){
	parse();
	Simulation::currentSim.simulate();
}