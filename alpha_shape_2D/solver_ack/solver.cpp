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
int myId, numOfProcs, provided;

int main(int argc, char* argv[])
{
	/* Consuming instance file and initializing network nodes */
	/*string instanceFileName = "./instances/kroD100.dat";*/
	//string instanceFileName = "./instances/dsj1000.dat";
	/*string instanceFileName = "./test/graph_0.dat";*/
	/*string instanceFileName = "./test/graph_3.dat";*/
	string instanceFileName = "./test/graph_0.dat";

	/* Node radius */
	double alphaValue = 7.798; //kroD100 radius
	//alphaValue = 5.78; //rat195 radius
	//alphaValue = 20; //team2_201 radius
	//alphaValue = 82.93; //team3_301 radius
	//alphaValue = 82.68; //lin318 radius
	//alphaValue = 19.910618; //rd400 radius
	//alphaValue = 7.6; //pcb442 radius
	//alphaValue = 27; //team6_501 radius
	//double alphaValue = 24.29218; //dsj1000 radius
	//alphaValue = 12; //bonus1000 radius

	/* Debug variables */
	//string s = argv[4];
	//string z = argv[5];

	string s = "yes";
	string z = "2";

	bool shouldDebug = (s == "yes" ? true : false);
	bool noAckAwait = true;
	int shouldDebugLevel = stoi(z);

	/* Initializing MPI parallelism */
	//MPI_Init(&argc, &argv);
	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);

	/* Constructor of the class consumes the instance and initializes the sensor accordingly */
	SensorNode u(instanceFileName, myId, shouldDebug, shouldDebugLevel, noAckAwait, alphaValue);

	/* Finalize the MPI environment. */
	MPI_Finalize();

	/* Resuming exection */
	return 0;
}