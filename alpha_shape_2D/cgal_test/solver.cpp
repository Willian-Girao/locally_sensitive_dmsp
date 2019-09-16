// Std libs.
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <map>
#include <time.h>
#include <stdlib.h>
#include <array>
#include <ctime>

// MPI - Message Passing Interface for parallel programming.
#include "mpi.h"

// User defined headers.
#include "SensorNode.h"

using namespace std;

/* Registering processes identifiers (sensor id) and number of processes (number of nodes) */
int myId, numOfProcs;

int main(int argc, char* argv[])
{
	/* Debug variables */
	//string s = argv[4];
	//string z = argv[5];

	string s = "yes";
	string z = "2";

	bool shouldDebug = (s == "yes" ? true : false);
	int shouldDebugLevel = stoi(z);

	/* Initializing MPI parallelism */
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);

	/* Consuming instance file and initializing network nodes */
	string instanceFileName = "./instances/pcb442.dat";

	/* Constructor of the class consumes the instance and initializes the sensor accordingly */
	SensorNode u(instanceFileName, myId, shouldDebug, shouldDebugLevel);

	/* Finalize the MPI environment. */
	MPI_Finalize();

	/* Resuming exection */
	return 0;
}