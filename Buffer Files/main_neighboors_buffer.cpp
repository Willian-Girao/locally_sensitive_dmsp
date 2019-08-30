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

// CGAL - Graphic library headers.
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Alpha_shape_2.h>
#include <CGAL/Alpha_shape_vertex_base_2.h>
#include <CGAL/Alpha_shape_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/algorithm.h>
#include <CGAL/assertions.h>

// MPI - Message Passing Interface for parallel programming.
#include <mpi.h>

// User defined headers.
#include "utils.h"

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

int main(int argc, char* argv[])
{
	/* Staring to account elapsed execution time. */
	clock_t t_init = clock();

	/* Process metadata controll variables. */
	double startServeTime, endServeTime, startServedTime, endServedTime, startRequestTime, endRequestTime, maxTime = 0;

	/* Node control variables. */
	int nodeId, numNodes = 0; //Process Id, number of processes (one for each node)
	int parentId = -1; //Id of the parent (where the mule came from) of a sensor (node)
	int unattendedNeigbors; //# of yet to be served nodes in N(nodeId)	
	int steps = 0; //# of movements made by the mule thus far
	int u; //Identifies which node has send me the msg 'MSG_SERVED'

	bool ended = false; //Flags the termination of the program execution
	bool served = false; //Flagged once the mule has passed by the node
	bool isMuleWithMe = false; //Checks whether or not the mule is at the current node

	/**/
	

	// Initialize the MPI environment
	// MPI_Init(NULL, NULL);

	return 0;
}