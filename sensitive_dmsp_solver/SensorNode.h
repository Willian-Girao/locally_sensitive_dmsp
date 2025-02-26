#pragma once
#ifndef SENSORNODE_H
#define SENSORNODE_H

#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iomanip>
#include <sstream>
#include <mpi.h>

// CGAL - Graphic library headers.
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Alpha_shape_2.h>
#include <CGAL/Alpha_shape_vertex_base_2.h>
#include <CGAL/Alpha_shape_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/algorithm.h>
#include <CGAL/assertions.h>

using namespace std;

/* CGAL lib required */
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef std::vector<Point_2> Points;
typedef K::Segment_2                                         Segment;
typedef K::FT                                                FT;
typedef CGAL::Alpha_shape_vertex_base_2<K>                   Vb;
typedef CGAL::Alpha_shape_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>          Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                Triangulation_2;
typedef CGAL::Alpha_shape_2<Triangulation_2>                 Alpha_shape_2;
typedef Alpha_shape_2::Alpha_shape_edges_iterator            Alpha_shape_edges_iterator;

class SensorNode {
public:
	//Constructor of the class consumes the instance and initializes the sensor accordingly
	SensorNode(string instanceFileName, int sensorId, bool shouldDebug, int shouldDebugLevel);
	~SensorNode();

	/* Getters */

	/* Setters */
	void initializeSensorNode(void);
private:
	/* Properties */
	//Node associated properties.
	int nodeId; //Graph's node id (sensor within the network)
	int unattendedNeigbors; //Number of neighbors yet to be served
	int u; //Identifies which node has send me a msg
	bool served; //Flagged once the mule has passed by the node

	//Execution controll associated properties.
	int totalNodes; //Total number of sensors whithin the network
	int parentId; //The parent of the node within the mules path on the network
	int steps; //# of movements made by the mule thus far

	bool endExec; //Flags the end of the execiton
	bool isMuleWithMe; //Checks whether or not the mule is at the current node
	bool hasMuleBeenWithMe; //Checks whether or not the mule has passed by the node at some point - INTRODUCED TO FIX ERROR_001 AND ERROR_001.1
	bool backtracking;

	//Metrics related properties.

	//Utils metrics
	string instanceName;

	//Debuging
	bool debug;
	int debugLevel;
	double mpiTimeDebuging;

	map<int, bool> neighbors_ACK_SERVED_buffer;
	map<int, bool> neighbors_ACK_BEING_SERVED_buffer;
	map<int, pair<bool, int>> neighbors_MSG_ENUMERNODES_buffer; //Stores answers (values) received in 'MSG_ENUMERNODES'
	map<int, bool> neighbors_MSGING_COUNT_buffer; //Marks wich neighbors have sent me their updated # of sent msg before sending the mule;

	//Botton two properties are used to calculate the Alpha-Shape of (N(u) U {u})
	map<int, pair<double, double>> my_coordinates;
	vector<int> my_neighbors_ids;

	//TODO - Change this for a map of 'int' and 'pair'.
	//TODO - 'pair' has to become a Point_2 for the CGLA lib.
	map<int, pair<double, double>> my_neighbors_xy;

	//MPI associated properties.
	int infoSent; //Stores sent information
	int infoRec; //Stores received information

	//Messages exchanged by mule and nodes ('u' - parent | 'v' - chield)
	enum tags {
		SEND_MULE, // 1# Sends the mule to another node
		MSG_REQUEST, // 2# Requests the number of uncovered neighboors
		MSG_ENUMERNODES, // 3# Sends a msg with the number of yet to be served sensors I have //TODO - change name of this propertie later
		MSG_SERVED, // 4# Informs that the node has been served by the mule
		MSG_BEING_SERVED,  // 5# Used by a node v (in N(u)) to let all N(v) know the mule is serving it
		ACK_SERVED, // 6# Msg acknowledging that node v knows that u has been served (sent only when all N(v) have sent 'ACK_BEING_SERVED')
		ACK_BEING_SERVED, // 7# Msg acknowledging that node a node in N(v) knows that v has been served
		SEND_END, // 8# Msg breadcasted to the process in order to let 'em know the exectuion has to terminate
		SAVE_MSGING_COUNT // 9# Let's the base station (node 0) knows that it must update the counter holding the total count of msgs sent (up to the point when the mule was last moved)
	};

	int totalMsgsSent, will;
	
	//Methods utilized to select node where the mule will be sent to
	enum selection_method {
		GREEDY,
		CONVEX_HULL,
		ALPHA_SHAPE
	};

	//CGAL related
	Points pointsCGAL;
	Points resultCGAL;

	double alphaValue;

	//Elected selection method
	selection_method selected;

	MPI_Status statusReq;
	MPI_Request statusSend;
	ofstream resultsOutFile;

	/* Methods */
	void muleOn1stSensorStart(void);
	void resetNeigAckServedSentBackBuffer(void); //Resets the flags that tell us if a N(u) has sent 'ACK_SERVED' back
	void resetNeigAckBeingServedSentBackBuffer(void);
	void resetNeigEnumernodesBuffer(void); //Resets the number of yet to be served neighbors of each of my neighbors
	void updateNeighborFlagAckServedReceived(void); //Marks that a neighbor have sent 'ACK_SERVED' back

	bool checkAllNeighborFlagAckServedReceived(void); //Marks that a neighbor have sent 'ACK_SERVED' back
	bool checkAllNeighborFlagAckBeingServedReceived(void); //Marks that a neighbor have sent 'ACK_BEING_SERVED' back
	bool checkAllMsgEnumernodesReceived(void); //Checks whether or not all of my N(u) have sent me their updated # of neighbors remaining to be attended by the mule
	bool workToBeDoneStill(void); //Checks if there is any neighbor with its own neighbors waiting to be served
	bool allUpdateMsgCountReivedBack(void);

	int coordinateToId(double x, double y);
	int methodOfChoice(void);
	int makeGreedySelection(void);
	int makeConvexHullSelection(void);
	int makeAlphaShapeSelection(void);
	/* Method for the Convex-Hull */
	/* Method for the Alpha-Shape */

	//Upon Receiving exchanged messages
	void msgServedReceived(void); /* Mule is with my parent and I need to update my neighbors | Received: 'MSG_SERVED'; Sent ahead: 'MSG_BEING_SERVED') */
	void msgAckServedReceived(void); /* My one of my N(u) have updated of its neighbors | Received: 'ACK_SERVED' */
	void msgBeingServedReceived(void); /* One of my N(u) is in contact with the mule because one of his neighbors has it | Received: 'MSG_BEING_SERVED' | Sent back: 'ACK_BEING_SERVED' */
	void msgAckBeingServedReceived(void); /* One of my N(v) have acknowledged that he knows that who has sent me the msg has the mule */
	void msgRequestReceived(void); /* My parent is requesting the number of yet to be served neighbors I have */
	void msgEnumernodesReceived(void);
	void msgSendMuleReceived(void);
	void broadcastProgramTermination(void);

	//Utils.
	void pauseExec(void);
	void debugMesseging(int receiver, string msg, int payload);
	void errorHasOccoured(string msg);
	void printSolutionMetrics(void);
};

#endif