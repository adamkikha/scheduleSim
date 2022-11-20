#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include "sim.h"

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

Simulation Simulation::currentSim;
Process * Simulation::currentProcess;
Algorithm * Simulation::currentAlgo;

void parse(){
	char line[50];
	char * tok;
	std::cin.getline(line,50);
	Simulation::currentSim.traceOrStats = (strcmp(line,"trace") != 0);
	std::cin.getline(line,50);
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
	std::cin.getline(line,50);
	Simulation::currentSim.simLength = atoi(line);
	std::cin.getline(line,50);
	int noOflines = atoi(line);
	Simulation::currentSim.processes = (Process **) malloc( noOflines * sizeof( Process * ) );
	for (int i = 0; i < noOflines; i++){
		std::cin.getline(line,50);
		char *name = strtok(line,",");
		int arrival = atoi(strtok(NULL,","));
		int service = atoi(strtok(NULL,","));
		Simulation::currentProcess = new Process(name,arrival,service);
		Simulation::currentSim.insertProcess( Simulation::currentProcess );
	}
	printf("traceOrStats= %d\nsimLength= %d\nnoOfAlgos= %d\nnoOfProcesses= %d\n",Simulation::currentSim.traceOrStats,Simulation::currentSim.simLength,Simulation::currentSim.noOfAlgos,Simulation::currentSim.noOfProcesses);
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
	}
}

int main(){
	parse();
}