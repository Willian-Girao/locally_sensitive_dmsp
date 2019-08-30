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

// Messages exchanged by mule and nodes ('u' - parent | 'v' - chield).
enum tags {
	SEND_MULE, //Sends the mule to another node
	MSG_REQUEST, //Requests the number of uncovered neighboors
	MSG_ENUMERNODES, //Sends a msg requiering the number of uncovered nodes of a node v in N(u)
	MSG_SERVED, //Informs that the node has been served by the mule
	MSG_BEING_SERVED,  //Used by a node v (in N(u)) to let all N(v) know the mule is serving it
	ACK_SERVED, //Msg acknowledging that node v knows that u has been served (sent only when all N(v) have sent 'ACK_BEING_SERVED')
	ACK_BEING_SERVED, //Msg acknowledging that node a node in N(v) knows that v has been served
	SEND_END //Msg breadcasted to the process in order to let 'em know the exectuion has to terminate
};

/* Registering processes identifiers (sensor id) and number of processes (number of nodes) */
int myId, numOfProcs;

MPI_Status status;
const static int tag = 1;

int main(int argc, char* argv[])
{
	/* Initializing MPI parallelism */
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);

	cout << "Hello from Process # " << myId << '\n';

	/* Consuming instance file and initializing network nodes */
	string instanceFileName = "./test_instances/instance_input_test00.dat"; //Input file

	/* Creating 'myId' sensor */
	SensorNode u(instanceFileName, myId);
	//u.initializeSensorNode();

	//Finalize the MPI environment.
	MPI_Finalize();

	return 0;
}