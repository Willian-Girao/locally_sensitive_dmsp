// Std libs.
#include <iostream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// CGAL - 
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>

// MPI - Message Passing Interface for parallel programming.
#include "mpi.h"

using namespace std;

/* Registering processes identifiers (sensor id) and number of processes (number of nodes) */
int myId, numOfProcs;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;

int main(int argc, char* argv[])
{
	/* Initializing MPI parallelism */
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);

	cout << "Process: " << myId << endl << endl;

	Point_2 points[5] = { Point_2(0,0), Point_2(10,0), Point_2(10,10), Point_2(6,5), Point_2(4,1) };
  	Point_2 result[5];

  	Point_2 *ptr = CGAL::convex_hull_2( points, points+5, result );

  	std::cout <<  ptr - result << " points on the convex hull:" << std::endl;

	for(int i = 0; i < ptr - result; i++) {
		std::cout << result[i] << std::endl;
	}

	cout << endl;

	/* Finalize the MPI environment. */
	MPI_Finalize();

	/* Resuming exection */
	return 0;
}