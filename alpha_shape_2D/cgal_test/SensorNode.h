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

using namespace std;

class SensorNode {
public:
	//Constructor of the class consumes the instance and initializes the sensor accordingly
	SensorNode(string instanceFileName, int sensorId, bool shouldDebug);
	~SensorNode();

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
	//TODO - maybe set 'isMuleWithMe' when mule gets out of a node and have another one to track where it is

	//Metrics related properties.
	//REVIEW - need to validate this
	double startServeTime;
	double endServeTime;

	double startServedTime;
	double endServedTime;

	double startRequestTime;
	double endRequestTime;

	double maxTime; //?

	int totalMsgsSent; //Counts how many messages have been sent to serve the whole network

	//Debuging
	bool debug;
	double mpiTimeDebuging;

	map<int, bool> neighbors_ACK_SERVED_buffer;
	map<int, bool> neighbors_ACK_BEING_SERVED_buffer;
	map<int, pair<bool, int>> neighbors_MSG_ENUMERNODES_buffer; //Stores answers (values) received in 'MSG_ENUMERNODES'
	map<int, bool> neighbors_ENDED_EXEC_buffer;

	//Botton two properties are used to calculate the Alpha-Shape of (N(u) U {u})
	map<int, pair<double, double>> my_coordinates;
	vector<int> my_neighbors_ids;

	//TODO - Change this for a map of 'int' and 'pair'.
	//TODO - 'pair' has to become a Point for the CGLA lib.
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
		UPDATE_MSG_TOTAL_COUNT // 9# Msg utilize to update sensor 0 (id 0) track of total messages sent
	};
	
	//Methods utilized to select node where the mule will be sent to
	enum selection_method {
		GREEDY,
		CONVEX_HULL,
		ALPHA_SHAPE
	};

	//Elected selection method
	selection_method selected;

	MPI_Status statusReq;
	MPI_Request statusSend;

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

	int methodOfChoice(void);
	int makeGreedySelection(void);

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
	void debugMesseging(int receiver, string msg);
	void errorHasOccoured(string msg);
	void printSolutionMetrics(void);
};

#endif