#pragma once

#include "SensorNode.h"

using namespace std;

//Constructor.
SensorNode::SensorNode(string instanceFileName, int sensorId, bool shouldDebug) {
	//Starting debug properties
	debug = shouldDebug;
	mpiTimeDebuging = MPI_Wtime();

	//Progran execution starting now
	endExec = false;

	//Starting metrics associated variables
	startServeTime = 0.0;
	endServeTime = 0.0;
	startServedTime = 0.0;
	endServedTime = 0.0;
	startRequestTime = 0.0;
	endRequestTime = 0.0;
	maxTime = 0.0;
	steps = 0;

	//Messages counter variables
	cont_SEND_MULE = 0;
	cont_MSG_REQUEST = 0;
	cont_MSG_ENUMERNODES = 0;
	cont_MSG_SERVED = 0;
	cont_MSG_BEING_SERVED = 0;
	cont_ACK_SERVED = 0;
	cont_ACK_BEING_SERVED = 0;
	cont_TOTAL_MSGS_SENT = 0;

	//Starting node associated variables
	isMuleWithMe = false; //Mule has made no moves thus far
	nodeId = sensorId; //Saving process id (aka nodeId; aka sensorId)
	parentId = -1; //Parent thusfar is unkown
	u = -1; //No msgs sent thus far
	infoSent = 0; //No info sent thus far
	infoRec = 0; //No info received thus far

	//Opening file containing problem's instance in the form of a graph
	ifstream instance_file;

	instance_file.open(instanceFileName);

	if (instance_file.fail())
	{
		errorHasOccoured("Fail oppening the instance file");
	}

	//Parsing file lines as a vector in order to optimize the access to the neighbors information.
	vector<string> file_lines;

	for (string line; getline(instance_file, line); )
	{
		file_lines.push_back(line);
	}
	instance_file.close();

	totalNodes = stoi(file_lines[0]); //Setting total number of nodes (1st line of the file)

	//Utils local variables.
	string str;
	int nodeFileLine = sensorId + 1;
	int nodeNeighborsFileLine = sensorId + 3 + stoi(file_lines[0]); /* '+ 3' accounts for the for index shift
	caused by NODES_COUNT and space between nodes coordinates and neighbors information. */

	//Error check.
	if (nodeNeighborsFileLine > file_lines.size())
	{
		errorHasOccoured("Neighbors index file calculated wrong");
	}

	//Populating my_coords structure.
	vector<string> split_vector_nodeLine;

	istringstream isss(file_lines[nodeFileLine]);

	for (string s; isss >> s; ) {
		split_vector_nodeLine.push_back(s);
	}

	double dx = stod(split_vector_nodeLine[0]);
	double dy = stod(split_vector_nodeLine[1]);

	pair<double, double> my_xy(dx, dy);

	my_coordinates[sensorId] = my_xy;

	//Populating my_neighbors structure.
	string neighbors_count = "";
	vector<string> split_vector;

	istringstream iss(file_lines[nodeNeighborsFileLine - 1]);

	for (string s; iss >> s; ) {
		split_vector.push_back(s);
	}

	for (int i = 1; i < stoi(split_vector[0]) + 1; ++i)
	{
		//Saving neighbor id
		my_neighbors_ids.push_back(stoi(split_vector[i]));

		//Initializing flag buffers
		neighbors_ACK_BEING_SERVED_buffer[stoi(split_vector[i])] = false;
		neighbors_ACK_SERVED_buffer[stoi(split_vector[i])] = false;

		//Get neighbor coordinates
		int neighborFileLine = stoi(split_vector[i]) + 1;
		vector<string> neighbor_line_split;

		istringstream iss(file_lines[neighborFileLine]);

		for (string s; iss >> s; ) {
			neighbor_line_split.push_back(s);
		}

		double cx = stod(neighbor_line_split[0]);
		double cy = stod(neighbor_line_split[1]);

		pair<double, double> neighbor_coord(cx, cy);

		my_neighbors_xy[stoi(split_vector[i])] = neighbor_coord;
	}

	unattendedNeigbors = stoi(split_vector[0]); //Initially all neighbors are unattended

	//cout << "MY_ID: " << nodeId << " | " << "MY_COORDINATES: " << my_xy.first << ", " << my_xy.second << " | NEIGHBORS_LINE_INDEX: " << nodeNeighborsFileLine << endl;
	//cout << endl << endl;
}

//Destructor.
SensorNode::~SensorNode() {
	//Finalize the MPI environment.
	MPI_Finalize();

	cout << "\nNode #" << nodeId << " has ended.\n" << endl;
	//TODO - method to check weather the node has had any issues (like if it has not been served).
}


/* Methods - setters */
//Node.
//Execution.
//Metrics.
//Utils.


/* Methods - getters */
//Node.
//Execution.
//Metrics.
//Utils.
void SensorNode::pauseExec(void) {
	int a;
	cout << "\n - EXECUTION PAUSED...\n" << endl;
	cin >> a;
	return;
}

void SensorNode::errorHasOccoured(string msg) {
	cout << "\n\n > ERROR - " << msg << "." << endl;
	pauseExec();

	return;
}

void SensorNode::debugMesseging(int receiver, string msg) {
	if (debug)
	{
		double tf = MPI_Wtime();
		cout << "\n" << nodeId << " " << msg << " " << receiver << "	[ " << (tf - mpiTimeDebuging) << " ]" << endl;
	}

	return;
}

void SensorNode::resetNeigAckServedSentBackBuffer(void) {
	for (int i = 0; i < my_neighbors_ids.size(); ++i)
	{
		neighbors_ACK_SERVED_buffer[my_neighbors_ids[i]] = false;
	}
	return;
}

void SensorNode::resetNeigAckBeingServedSentBackBuffer(void) {
	for (int i = 0; i < my_neighbors_ids.size(); ++i)
	{
		if (my_neighbors_ids[i] != parentId)
		{
			neighbors_ACK_BEING_SERVED_buffer[my_neighbors_ids[i]] = false;
		}
	}
	return;
}

void SensorNode::updateNeighborFlagAckServedReceived() {

	return;
}

/* Private Methods */
void SensorNode::muleOn1stSensorStart(void) {
	parentId = 0; //1st node to be visited ("Base Station")
	served = true; //Mule is serving me right now
	isMuleWithMe = true; //Mule is with me
	unattendedNeigbors = 0; /* REVISION - Pablo set's it to '0' here but I believe it should
	be the actual number of neighbors for there are msgs to be exchanged in order to serve them.
	Correction: maybe this is due to the fact that 'no one will ask this node's number of unattended
	nodes' (this is set to '0' already) - but still there might be msgs to be accounted for here still - validate with prof. Uéverton */

	//Returns an elapsed time on the calling processor
	startServeTime = MPI_Wtime();

	//Reseting flag that marks the event of a N(u) having sent 'ACK_SERVED' back
	resetNeigAckServedSentBackBuffer();

	//Notifying N(u) that u is being served by the mule
	for (int i = 0; i < my_neighbors_ids.size(); i++)
	{
		//Debuging msg
		debugMesseging(my_neighbors_ids[i], "MSG_SERVED");

		//Let N(u) know I'm being served
		MPI_Send(&infoSent, 1, MPI_INT, my_neighbors_ids[i], MSG_SERVED, MPI_COMM_WORLD);

		//Updating metrics variables
		cont_TOTAL_MSGS_SENT++;
		cont_MSG_SERVED++;
	}
}

void SensorNode::msgServedReceived() {
	//In case I haven't been served yet
	if (!served)
	{
		parentId = u; //Sensor sending me msg is my parent
		
		//REVISE - maybe this should only be set after receiving MSG_BEING_SERVED
		served = true; //Mule can serve me

		//Mule was at the base station - (it has been served already in the case of the base station - 1st node)
		if (u == 0)
		{
			//Node 0 sensor is where the mule starts at
			unattendedNeigbors--;
		}

		//Only one neighbor (and he is the one who sent me the msg) - there's nobody to send MSG_BEING_SERVED to
		if (my_neighbors_ids.size() - 1 == 0)
		{
			//Debuging msg
			debugMesseging(u, "ACK_SERVED");

			//Let u know I recognize he has been served
			MPI_Send(&infoSent, 1, MPI_INT, u, ACK_SERVED, MPI_COMM_WORLD);

			//Updating metrics variables
			cont_TOTAL_MSGS_SENT++;
			cont_ACK_BEING_SERVED++;
		}
		else {
			//Returns an elapsed time on the calling processor
			startServeTime = MPI_Wtime(); //REVISE - should this line be here?

			//Reseting flag that marks the event of a N(v) having sent 'ACK_BEING_SERVED' back - except my parent node (mule is there)
			resetNeigAckBeingServedSentBackBuffer(); //Will only be finished when all my N(v) have sent me ACK_BEING_SERVED

			//Updating my neighbors about my status
			for (int i = 0; i < my_neighbors_ids.size(); i++)
			{
				//My parent sent me the message (no need to notify him)
				if (my_neighbors_ids[i] != parentId)
				{
					//Debuging msg
					debugMesseging(my_neighbors_ids[i], "MSG_BEING_SERVED");

					//Let my N(v) know I'm being served (my parent node is with the mule at this moment talking to me)
					MPI_Send(&infoSent, 1, MPI_INT, my_neighbors_ids[i], MSG_BEING_SERVED, MPI_COMM_WORLD);

					//Updating metrics variables
					cont_TOTAL_MSGS_SENT++;
					cont_MSG_BEING_SERVED++;
				}
			}
		}
	}
	else {
		//Then all my neighbors have already been notified by me - notify whoever sent me MSG_SERVED that I'm confirming acknowledgment

		//Debuging msg
		debugMesseging(u, "ACK_SERVED");

		//Let my N(v) know I'm being served (my parent node is with the mule at this moment talking to me)
		MPI_Send(&infoSent, 1, MPI_INT, u, ACK_SERVED, MPI_COMM_WORLD);

		//Updating metrics variables
		cont_TOTAL_MSGS_SENT++;
		cont_ACK_SERVED++;
	}
}

void SensorNode::msgBeingServedReceived() {
	//My neighbor is being served by the mule
	unattendedNeigbors--;

	//Error check
	if (unattendedNeigbors < 0)
	{
		errorHasOccoured("Number of neighbors remaining to be served went negative");
	}

	//Debuging msg
	debugMesseging(u, "ACK_BEING_SERVED");

	//Let who sent the message now I'm acknowledging that the mule is serving it
	MPI_Send(&infoSent, 1, MPI_INT, u, ACK_BEING_SERVED, MPI_COMM_WORLD);

	//Updating metrics variables
	cont_TOTAL_MSGS_SENT++;
	cont_ACK_BEING_SERVED++;

	return;
}

void SensorNode::msgAckServedReceived() {
	updateNeighborFlagAckServedReceived();

	return;
}

void SensorNode::initializeSensorNode(int id) {
	if (id == 0)
	{
		muleOn1stSensorStart();
	}

	int node = 0; //Reference to the id of the node sending the incoming msg

	while (!endExec) {
		/* Listen for the msgs here */
		MPI_Recv(&infoRec, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &statusReq);

		//Saving who has sent me the msg
		u = statusReq.MPI_SOURCE;

		//Make message_type-based action upon message arrival
		switch (statusReq.MPI_TAG)
		{
			case MSG_SERVED:
				msgServedReceived();
				break;
			case ACK_SERVED:
				//msgAckServedReceived();
				cout << "" << endl;
				break;
			case MSG_BEING_SERVED:
				msgBeingServedReceived();
				break;
			default:
				break;
		}
	}

	return;
}