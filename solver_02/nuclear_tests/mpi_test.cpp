// Std libs.
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// MPI - Message Passing Interface for parallel programming.
#include "mpi.h"

using namespace std;

/* Registering processes identifiers (sensor id) and number of processes (number of nodes) */
int myId, numOfProcs;

int main(int argc, char* argv[])
{
	/* Initializing MPI parallelism */
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);

	cout << "Process: " << myId << endl;

	/* Finalize the MPI environment. */
	MPI_Finalize();

	/* Resuming exection */
	return 0;
}