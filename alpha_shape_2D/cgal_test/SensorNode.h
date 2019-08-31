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

//For debugging
#include <windows.h>
#include <wchar.h>

using namespace std;

class SensorNode {
public:
	SensorNode(string instanceFileName, int sensorId, bool shouldDebug);
	~SensorNode();

	/* Getters */
	//Node.
	//Execution.
	//Metrics.
	//Utils.


	/* Setters */
	//Node.
	void initializeSensorNode(int myId);

	//Execution.
	//Metrics.

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

	//Counters associated with experiment metrics.
	int	cont_SEND_MULE;
	int cont_MSG_REQUEST;
	int cont_TOTAL_MSGS_SENT;
	int cont_MSG_ENUMERNODES;

	int cont_MSG_SERVED;
	int cont_ACK_SERVED;

	int cont_MSG_BEING_SERVED;
	int cont_ACK_BEING_SERVED;


	bool endExec; //Flags the end of the execiton
	bool isMuleWithMe; //Checks whether or not the mule is at the current node

	//Metrics related properties.
	double startServeTime;
	double endServeTime;

	double startServedTime;
	double endServedTime;

	double startRequestTime;
	double endRequestTime;

	double maxTime;

	//Debuging
	bool debug;
	double mpiTimeDebuging;

	map<int, bool> neighbors_ACK_SERVED_buffer;
	map<int, bool> neighbors_ACK_BEING_SERVED_buffer;

	//Botton two properties are used to calculate the Alpha-Shape of (N(u) U {u})
	map<int, pair<double, double>> my_coordinates;
	vector<int> my_neighbors_ids;

	//TODO - Change this for a map of 'int' and 'pair'.
	//TODO - 'pair' has to become a Point for the CGLA lib.
	map<int, pair<double, double>> my_neighbors_xy;

	//MPI associated properties.
	int infoSent; //Stores sent information
	int infoRec; //Stores received information

	enum tags { //Messages exchanged by mule and nodes ('u' - parent | 'v' - chield)
		SEND_MULE, // 1# Sends the mule to another node
		MSG_REQUEST, // 2# Requests the number of uncovered neighboors
		MSG_ENUMERNODES, // 3# Sends a msg requiering the number of uncovered nodes of a node v in N(u)
		MSG_SERVED, // 4# Informs that the node has been served by the mule
		MSG_BEING_SERVED,  // 5# Used by a node v (in N(u)) to let all N(v) know the mule is serving it
		ACK_SERVED, // 6# Msg acknowledging that node v knows that u has been served (sent only when all N(v) have sent 'ACK_BEING_SERVED')
		ACK_BEING_SERVED, // 7# Msg acknowledging that node a node in N(v) knows that v has been served
		SEND_END // 8# Msg breadcasted to the process in order to let 'em know the exectuion has to terminate
	};

	MPI_Status statusReq;
	MPI_Request statusSend;

	/* Methods */
	void muleOn1stSensorStart(void);
	void resetNeigAckServedSentBackBuffer(void); //Resets the flags that tell us if a N(u) has sent 'ACK_SERVED' back
	void resetNeigAckBeingServedSentBackBuffer(void);
	void updateNeighborFlagAckServedReceived(void); //Marks that a neighbor have sent 'ACK_SERVED' back

	//Upon Receiving exchanged messages
	void msgServedReceived(void); /* Mule is with my parent and I need to update my neighbors | Received: 'MSG_SERVED'; Sent ahead: 'MSG_BEING_SERVED') */
	void msgAckServedReceived(void); /* My one of my N(u) have updated of its neighbors | Received: 'ACK_SERVED' */
	void msgBeingServedReceived(void); /* One of my N(u) is in contact with the mule because one of his neighbors has it | Received: 'MSG_BEING_SERVED' | Sent back: 'ACK_BEING_SERVED' */

	//Utils.
	void pauseExec(void);
	void debugMesseging(int receiver, string msg);
	void errorHasOccoured(string msg);
};

#endif