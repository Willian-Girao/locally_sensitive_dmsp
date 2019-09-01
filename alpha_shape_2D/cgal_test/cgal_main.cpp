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

// CGAL - Graphic library headers.
//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Alpha_shape_2.h>
//#include <CGAL/Alpha_shape_vertex_base_2.h>
//#include <CGAL/Alpha_shape_face_base_2.h>
//#include <CGAL/Delaunay_triangulation_2.h>
//#include <CGAL/algorithm.h>
//#include <CGAL/assertions.h>

// MPI - Message Passing Interface for parallel programming.
#include "mpi.h"

// User defined headers.
#include "utils.h"
#include "SensorNode.h"

using namespace std;

/* Registering processes identifiers (sensor id) and number of processes (number of nodes) */
int myId, numOfProcs;

const static int tag = 1;

int main(int argc, char* argv[])
{
	//Debug variables
	bool shouldDebug = false;

	/* Initializing MPI parallelism */
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);

	//cout << "Hello from process # " << myId << '\n';

	/* Consuming instance file and initializing network nodes */
	string instanceFileName = "./test_instances/instance_input_test00.dat"; //Input file

	/* Creating 'myId' sensor */
	SensorNode u(instanceFileName, myId, shouldDebug);
	
	/* Start creating solution */
	u.initializeSensorNode(myId);

	//Finalize the MPI environment.
	MPI_Finalize();

	return 0;
}