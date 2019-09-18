#pragma once

#include "SensorNode.h"

using namespace std;

/* Public Methods */

//Sensor initializer
void SensorNode::initializeSensorNode(void) {
	if (nodeId == 0)
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
			msgAckServedReceived();
			break;
		case MSG_BEING_SERVED:
			msgBeingServedReceived();
			break;
		case ACK_BEING_SERVED:
			msgAckBeingServedReceived();
			break;
		case MSG_REQUEST:
			msgRequestReceived();
			break;
		case MSG_ENUMERNODES:
			msgEnumernodesReceived();
			break;
		case SEND_MULE:
			msgSendMuleReceived();
			break;
		case SAVE_MSGING_COUNT:
			if (nodeId == 0)
			{
				totalMsgsSent += infoRec;
			}
			break;
		case SEND_END:
			broadcastProgramTermination();
			break;
		default:
			errorHasOccoured("Message received and not processed");
			break;
		}
	}

	return;
}

//Constructor.
SensorNode::SensorNode(string instanceFileName, int sensorId, bool shouldDebug, int shouldDebugLevel) {
	//Starting debug properties
	debug = shouldDebug;
	debugLevel = shouldDebugLevel;
	mpiTimeDebuging = MPI_Wtime();

	//Saving neighbor selection method chosen
	// selected = GREEDY;
	selected = CONVEX_HULL;
	//selected = ALPHA_SHAPE;

	//alphaValue = 7.798; //rat195 radius
	//alphaValue = 20; //team2_201 radius
	//alphaValue = 7; //team3_301 radius
	//alphaValue = 82.68; //lin318 radius
	//alphaValue = 19.910618; //rd400 radius
	//alphaValue = 7.6; //pcb442 radius
	alphaValue = 27; //team6_501 radius
	//alphaValue = 24.29218; //dsj1000 radius

	//Progran execution starting now
	endExec = false;
	steps = 0; //Mule has made no moves yet

	//Starting node associated variables
	isMuleWithMe = false; //Mule has made no moves thus far

	hasMuleBeenWithMe = false; //INTRODUCED TO FIX ERROR_001 AND ERROR_001.1
	backtracking = false; //INTRODUCED TO FIX ERROR_001 AND ERROR_001.1

	nodeId = sensorId; //Saving process id (aka nodeId; aka sensorId)
	parentId = -1; //Parent thusfar is unkown
	u = -1; //No msgs sent thus far
	infoSent = 0; //No info sent thus far
	infoRec = 0; //No info received thus far

	totalMsgsSent = 0;
	will = 0;

	//Opening file containing problem's instance in the form of a graph
	ifstream instance_file;

	//Saving instance name
	instanceName = instanceFileName;

	instance_file.open(instanceFileName);

	if (instance_file.fail() || instance_file.bad())
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

	//Setting total number of nodes (1st line of the file)
	totalNodes = stoi(file_lines[0]);

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

	pointsCGAL.push_back(Point_2(dx, dy));

	pair<double, double> my_xy(dx, dy);

	my_coordinates[sensorId] = my_xy;

	//Populating my_neighbors structure.
	string neighbors_count = "";
	vector<string> split_vector;

	istringstream iss(file_lines[nodeNeighborsFileLine - 1]); //TODO - check why compiler doesnt like it

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
		neighbors_MSG_ENUMERNODES_buffer[stoi(split_vector[i])] = make_pair<bool, int>(false, -1);
		neighbors_MSGING_COUNT_buffer[stoi(split_vector[i])] = false;

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

		pointsCGAL.push_back(Point_2(cx, cy));
	}

	//Initially all neighbors are unattended
	unattendedNeigbors = stoi(split_vector[0]);

	//Calculating required shape - Convex-Hull | Alpha-Shape
	switch (selected)
	{
		case CONVEX_HULL:
			{
				//Calculating Convex-hull on {u} U N(u) and saving to 'resultCGAL'
				CGAL::convex_hull_2( pointsCGAL.begin(), pointsCGAL.end(), back_inserter(resultCGAL) );

				if (debug && debugLevel == 3) 
				{
					cout << "My id: " << nodeId << " | " << resultCGAL.size() << " points on the convex hull" << endl;
				}
				break;
			}
		case ALPHA_SHAPE:
			{
				//Calculating Alpha-shape on {u} U N(u)
				Alpha_shape_2 alphaShape(pointsCGAL.begin(), pointsCGAL.end(), FT(1000), Alpha_shape_2::GENERAL);

				//Alpha-shapes' alpha is set as the node transmission range
				alphaShape.set_alpha(alphaValue);

				//Saving alpha-shapes boundary vertices to 'resultCGAL'
				for (Alpha_shape_2::Alpha_shape_vertices_iterator it = alphaShape.Alpha_shape_vertices_begin(); it != alphaShape.Alpha_shape_vertices_end(); ++it)
				{
					Alpha_shape_2::Vertex_handle handle = *it;
					Point_2 p = handle->point();

					resultCGAL.push_back(p);
				}

				if (debug && debugLevel == 3)
				{
					// Returns an iterator pointing to the first element with α-value such that the alpha shape satisfies the following two properties:
					// 	- All data points are either on the boundary or in the interior of the regularized version of the alpha shape.
					// 	- The number of solid component of the alpha shape is equal to or smaller than nb_components.
					// If no such value is found, the iterator points to the first element with α-value such that the alpha shape satisfies the second property.
					cout << "My id: " << nodeId << " |  Alpha: " << alphaValue << endl;
				}

				break;
			}
		default:
			//Convex-hull by default
			CGAL::convex_hull_2( pointsCGAL.begin(), pointsCGAL.end(), back_inserter(resultCGAL) );

			if (debug && debugLevel == 3)
			{
				cout << "My id: " << nodeId << " | " << resultCGAL.size() << " points on the convex hull (default)" << endl;
			}
			break;
	}

	//Start sensor
	initializeSensorNode();
}

//Destructor.
SensorNode::~SensorNode() {
	//TODO - method to check weather the node has had any issues (like if it has not been served).
	//Closing last sensor on the network
	if (nodeId == 0) {
		//TODO - method to return the answer here.
		cout << "\n\n________________ SOLUTION FOUND ________________\n\n";
		cout << "Mule movements: " << steps << endl;
		cout << "Messages sent:  " << totalMsgsSent << endl;
		cout << "Nodes count:    " << totalNodes << endl;
		cout << "Instance:       " << instanceName << endl;
		switch (selected)
		{
			case GREEDY:
			{
				cout << "Method:         GREEDY" << instanceName << endl;
				break;
			}
			case CONVEX_HULL:
			{
				cout << "Method:         CONVEX-HULL" << instanceName << endl;
				break;
			}
			case ALPHA_SHAPE:
			{
				cout << "Method:         ALPHA-SHAPE" << instanceName << endl;
				break;
			}
			default:
			
				break;
		}
		cout << "________________________________________________\n\n";
	}
}

/* Private Methods */

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

void SensorNode::debugMesseging(int receiver, string msg, int payload) {
	if (debug)
	{
		double tf = MPI_Wtime();

		switch (debugLevel)
		{
		case 1:
			if (payload >= 0)
			{
				cout << "\n" << nodeId << " " << msg << " " << receiver << " [" << payload << "]	- " << (tf - mpiTimeDebuging) << "ms";
				cout << endl;
			}
			else if (msg == "SEND_MULE") {
				cout << "\n" << nodeId << " ->[ SENDING MULE ]-> " << receiver << endl;
			}
			else {
				cout << "\n" << nodeId << " " << msg << " " << receiver << "	- " << (tf - mpiTimeDebuging) << "ms" << endl;
			}

			break;
		case 2:
			if (msg == "SEND_MULE")
			{
				cout << "\n" << nodeId << " ->[ SENDING MULE ]-> " << receiver << endl;
			}

			break;
		default:
			break;
		}
	}

	return;
}

void SensorNode::printSolutionMetrics(void) {
	return;
}

//Sensor execution related
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

void SensorNode::resetNeigEnumernodesBuffer(void) {
	for (int i = 0; i < my_neighbors_ids.size(); ++i)
	{
		neighbors_MSG_ENUMERNODES_buffer[my_neighbors_ids[i]].first = false;
		neighbors_MSG_ENUMERNODES_buffer[my_neighbors_ids[i]].second = -1;
	}

	return;
}

void SensorNode::updateNeighborFlagAckServedReceived() {
	neighbors_ACK_SERVED_buffer[u] = true;

	return;
}

bool SensorNode::checkAllNeighborFlagAckServedReceived(void) {
	bool allReceived = true;

	for (int i = 0; i < my_neighbors_ids.size(); i++)
	{
		if (neighbors_ACK_SERVED_buffer[my_neighbors_ids[i]] == false && my_neighbors_ids[i] != parentId)
		{
			allReceived = false;
		}
	}

	return allReceived;
}

bool SensorNode::checkAllNeighborFlagAckBeingServedReceived(void) {
	bool allReceived = true;

	for (int i = 0; i < my_neighbors_ids.size(); i++)
	{
		if (neighbors_ACK_BEING_SERVED_buffer[my_neighbors_ids[i]] == false && my_neighbors_ids[i] != parentId)
		{
			allReceived = false;
		}
	}

	return allReceived;
}

bool SensorNode::checkAllMsgEnumernodesReceived(void) {
	bool allReceived = true;

	//Neighbor selection method has to influence here - who I'm waiting to reply my request
	switch (selected) 
	{
		case GREEDY:
			for (int i = 0; i < my_neighbors_ids.size(); i++)
			{
				if (backtracking)
				{
					if (neighbors_MSG_ENUMERNODES_buffer[my_neighbors_ids[i]].first == false)
					{
						allReceived = false;
					}
				}
				else {
					if (my_neighbors_ids[i] != parentId && neighbors_MSG_ENUMERNODES_buffer[my_neighbors_ids[i]].first == false)
					{
						allReceived = false;
					}
				}
			}
			break;
		case CONVEX_HULL:
			for (int i = 0; i < resultCGAL.size(); i++)
			{
				int idAux = coordinateToId(resultCGAL[i].x(), resultCGAL[i].y());

				if (backtracking)
				{
					if (idAux != nodeId && neighbors_MSG_ENUMERNODES_buffer[idAux].first == false)
					{
						allReceived = false;
					}
				}
				else {
					if (idAux != nodeId && idAux != parentId && neighbors_MSG_ENUMERNODES_buffer[idAux].first == false)
					{
						allReceived = false;
					}
				}
			}
			break;
		case ALPHA_SHAPE:
			for (int i = 0; i < resultCGAL.size(); i++)
			{
				int idAux = coordinateToId(resultCGAL[i].x(), resultCGAL[i].y());

				if (backtracking)
				{
					if (idAux != nodeId && neighbors_MSG_ENUMERNODES_buffer[idAux].first == false)
					{
						allReceived = false;
					}
				}
				else {
					if (idAux != nodeId && idAux != parentId && neighbors_MSG_ENUMERNODES_buffer[idAux].first == false)
					{
						allReceived = false;
					}
				}
			}
			break;
		default:
			//Greedy by default
			for (int i = 0; i < my_neighbors_ids.size(); i++)
			{
				if (backtracking)
				{
					if (neighbors_MSG_ENUMERNODES_buffer[my_neighbors_ids[i]].first == false)
					{
						allReceived = false;
					}
				}
				else {
					if (my_neighbors_ids[i] != parentId && neighbors_MSG_ENUMERNODES_buffer[my_neighbors_ids[i]].first == false)
					{
						allReceived = false;
					}
				}
			}
			break;
	}

	return allReceived;
}

bool SensorNode::workToBeDoneStill(void) {
	bool allWithZeroNeighborsToBeServed = false;

	for (int i = 0; i < my_neighbors_ids.size(); i++)
	{
		if (my_neighbors_ids[i] != parentId && neighbors_MSG_ENUMERNODES_buffer[my_neighbors_ids[i]].second > 0)
		{
			allWithZeroNeighborsToBeServed = true;
		}
	}


	return allWithZeroNeighborsToBeServed;
}

bool SensorNode::allUpdateMsgCountReivedBack(void) {
	bool allMsgCountUpdated = true;

	for (int i = 0; i < my_neighbors_ids.size(); i++)
	{
		if (my_neighbors_ids[i] != parentId && neighbors_MSGING_COUNT_buffer[my_neighbors_ids[i]] == false)
		{
			allMsgCountUpdated = false;
		}
	}


	return allMsgCountUpdated;
}

int SensorNode::makeGreedySelection(void) {
	//Result will be the neighbor with the largest # of neighbor sensors not served yet
	int sensorId = -1;
	int biggestDemmand = -1;

	for (int i = 0; i < my_neighbors_ids.size(); i++)
	{
		//TODO - set the parent's flag to true when receiving 'MSG_SERVED' so I wont have to check if its different from parentId down bellow.
		if (neighbors_MSG_ENUMERNODES_buffer[my_neighbors_ids[i]].first != false && neighbors_MSG_ENUMERNODES_buffer[my_neighbors_ids[i]].second > 0)
		{
			if (my_neighbors_ids[i] != parentId && neighbors_MSG_ENUMERNODES_buffer[my_neighbors_ids[i]].second > biggestDemmand) {
				//Updating optimal choice
				biggestDemmand = neighbors_MSG_ENUMERNODES_buffer[my_neighbors_ids[i]].second;
				sensorId = my_neighbors_ids[i];
			}
		}
	}

	//Error checking
	if (sensorId == -1)
	{
		errorHasOccoured("Next sensor to send the mule has not been chosen");
	}

	return sensorId;
}

int SensorNode::makeConvexHullSelection(void) {
	//Result will be the neighbor with the largest # of neighbor sensors not served yet
	int sensorId = -1;
	int biggestDemmand = -1;

	for (int i = 0; i < resultCGAL.size(); i++)
	{
		int idAux = coordinateToId(resultCGAL[i].x(), resultCGAL[i].y());

		if (neighbors_MSG_ENUMERNODES_buffer[idAux].first != false && neighbors_MSG_ENUMERNODES_buffer[idAux].second > 0)
		{
			if (idAux != nodeId && idAux != parentId && neighbors_MSG_ENUMERNODES_buffer[idAux].second > biggestDemmand) {
				//Updating optimal choice
				biggestDemmand = neighbors_MSG_ENUMERNODES_buffer[idAux].second;
				sensorId = idAux;
			}
		}
	}

	//Error checking
	if (sensorId == -1)
	{
		errorHasOccoured("Next sensor to send the mule has not been chosen");
	}

	return sensorId;
}

int SensorNode::makeAlphaShapeSelection(void) {
	//Result will be the neighbor with the largest # of neighbor sensors not served yet
	int sensorId = -1;
	int biggestDemmand = -1;

	for (int i = 0; i < resultCGAL.size(); i++)
	{
		int idAux = coordinateToId(resultCGAL[i].x(), resultCGAL[i].y());

		if (neighbors_MSG_ENUMERNODES_buffer[idAux].first != false && neighbors_MSG_ENUMERNODES_buffer[idAux].second > 0)
		{
			if (idAux != nodeId && idAux != parentId && neighbors_MSG_ENUMERNODES_buffer[idAux].second > biggestDemmand) {
				//Updating optimal choice
				biggestDemmand = neighbors_MSG_ENUMERNODES_buffer[idAux].second;
				sensorId = idAux;
			}
		}
	}

	//Error checking
	if (sensorId == -1)
	{
		errorHasOccoured("Next sensor to send the mule has not been chosen");
	}

	return sensorId;
}

int SensorNode::coordinateToId(double x, double y) {
	int id = -1;

	//Checking within N(u)
	for (int i = 0; i < my_neighbors_ids.size(); ++i)
	{
		if (my_neighbors_xy[my_neighbors_ids[i]].first == x && my_neighbors_xy[my_neighbors_ids[i]].second == y)
		{
			id = my_neighbors_ids[i];
		}
	}

	//Checking if u
	if (my_coordinates[nodeId].first == x && my_coordinates[nodeId].second == y)
	{
		id = nodeId;
	}

	//Error check
	if (id == -1)
	{
		errorHasOccoured("Attempt to find id from coordinates failed");
	}

	return id;
}

int SensorNode::methodOfChoice(void) {
	//Selected node (sensor) to send the mule next
	int selectedNeighbor = -1;

	//Checking wich selection method was elected
	switch (selected)
	{
		case GREEDY:
			return makeGreedySelection();
			break;
		case CONVEX_HULL:
			return makeConvexHullSelection();
			break;
		case ALPHA_SHAPE:
			return makeAlphaShapeSelection();
			break;
		default:
			errorHasOccoured("No neighbor selection method chosen");
			break;
	}

	//Error checking
	if (selectedNeighbor == -1)
	{
		errorHasOccoured("Neighbor selection to send the mule didn't return anything");
	}

	return selectedNeighbor;
}

//Message arrival related
void SensorNode::muleOn1stSensorStart(void) {
	parentId = 0; //1st node to be visited ("Base Station")
	served = true; //Mule is serving me right now
	isMuleWithMe = true; //Mule is with me

	hasMuleBeenWithMe = true; //INTRODUCED TO FIX ERROR_001 AND ERROR_001.1

	unattendedNeigbors = 0; /* REVISION - Pablo set's it to '0' here but I believe it should
	be the actual number of neighbors for there are msgs to be exchanged in order to serve them.
	Correction: maybe this is due to the fact that 'no one will ask this node's number of unattended
	nodes' (this is set to '0' already) - but still there might be msgs to be accounted for here still - validate with prof. Uéverton */

	//Reseting flag that marks the event of a N(u) having sent 'ACK_SERVED' back
	resetNeigAckServedSentBackBuffer();

	//Notifying N(u) that u is being served by the mule
	for (int i = 0; i < my_neighbors_ids.size(); i++)
	{
		//Debuging msg
		debugMesseging(my_neighbors_ids[i], "MSG_SERVED", -1);

		//Let N(u) know I'm being served
		MPI_Send(&infoSent, 1, MPI_INT, my_neighbors_ids[i], MSG_SERVED, MPI_COMM_WORLD);

		//Updating metrics variables
		will += 2;
	}
}

void SensorNode::msgServedReceived() {
	//Reseting msging counting helpers
	will = 0;

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
			debugMesseging(u, "ACK_SERVED", -1);
			infoSent = 0;

			// if ((infoSent % 2) > 0)
			// {
			// 	cout << "Node sending odd count*: " << nodeId << endl;
			// 	cout << "To: " << u << endl;
			// 	cout << "Count: " << infoSent << endl;
			// 	pauseExec();
			// }

			//Let u know I recognize he has been served
			MPI_Send(&infoSent, 1, MPI_INT, u, ACK_SERVED, MPI_COMM_WORLD);
		}
		else {
			//Reseting flag that marks the event of a N(v) having sent 'ACK_BEING_SERVED' back - except my parent node (mule is there)
			resetNeigAckBeingServedSentBackBuffer(); //Will only be finished when all my N(v) have sent me ACK_BEING_SERVED

			//Updating my neighbors about my status
			for (int i = 0; i < my_neighbors_ids.size(); i++)
			{
				//My parent sent me the message (no need to notify him)
				if (my_neighbors_ids[i] != parentId)
				{
					//Debuging msg
					debugMesseging(my_neighbors_ids[i], "MSG_BEING_SERVED", -1);

					//Let my N(v) know I'm being served (my parent node is with the mule at this moment talking to me)
					MPI_Send(&infoSent, 1, MPI_INT, my_neighbors_ids[i], MSG_BEING_SERVED, MPI_COMM_WORLD);

					//Updating metrics variables
					will += 2;
				}
			}
		}
	}
	else {
		//Then all my neighbors have already been notified by me - notify whoever sent me MSG_SERVED that I'm confirming acknowledgment

		//Debuging msg
		debugMesseging(u, "ACK_SERVED", -1);
		infoSent = 0;

		//Let my N(v) know I'm being served (my parent node is with the mule at this moment talking to me)
		MPI_Send(&infoSent, 1, MPI_INT, u, ACK_SERVED, MPI_COMM_WORLD);
	}
}

void SensorNode::msgAckServedReceived() {
	//TODO - error check here for odd infoRec

	will += infoRec; //Update local count of msg sent

	//Marking that who sent the message acknowledges that I've been served by the mule
	updateNeighborFlagAckServedReceived();

	//All my N(u) are updated and have sent 'ACK_SERVED' back
	if (checkAllNeighborFlagAckServedReceived()) //REVIEW - maybe Pablo is not checking for the parentId here
	{
		//Resenting the counting of # of yet to be served neighbors of each of my neighbors
		resetNeigEnumernodesBuffer();

		/*
			CONTEXT:	Starting to backtrack from Node 45 back to 8.
			ERROR:		I'm starting to backtrack here but I'm not setting on the 'neighbors_MSG_ENUMERNODES_buffer' buffer
						the sensor that has sent me the mule back.
			FIX:
						if (backtracking)
						{
							neighbors_MSG_ENUMERNODES_buffer[u].first = true; //INCORPORATED TO APPLY IMPROVE_000 (SO I WONT BE ETERNALY WAITING WHEN BACKTRACKING)
							neighbors_MSG_ENUMERNODES_buffer[u].second = 0; //INCORPORATED TO APPLY IMPROVE_000 (SO I WONT BE ETERNALY WAITING WHEN BACKTRACKING)
						}
			u:			8 (validated with debugging msg)
		*/

		if (backtracking)
		{
			neighbors_MSG_ENUMERNODES_buffer[u].first = true; //INCORPORATED TO APPLY IMPROVE_000 (SO I WONT BE ETERNALY WAITING WHEN BACKTRACKING)
			neighbors_MSG_ENUMERNODES_buffer[u].second = 0; //INCORPORATED TO APPLY IMPROVE_000 (SO I WONT BE ETERNALY WAITING WHEN BACKTRACKING)
		}

		//Requesting number of yet to be served neighbors of my neighbors - neighbor select method has to influence here - who I'm gonna 
		//send the request to | GREEDY: request to all | CONVEX_HULL: request only to CH[{u} U N(u) | ALPHA_SHAPE: request only to AS[{u} U N(u)]
		switch (selected) 
		{
			case GREEDY:
				for (int i = 0; i < my_neighbors_ids.size(); i++)
				{
					//No need to send to my parent
					if (my_neighbors_ids[i] != parentId)
					{
						//Debuging msg
						debugMesseging(my_neighbors_ids[i], "MSG_REQUEST", -1);

						//Let who sent the message now I'm acknowledging that the mule is serving it
						infoSent = 1;
						MPI_Send(&infoSent, 1, MPI_INT, my_neighbors_ids[i], MSG_REQUEST, MPI_COMM_WORLD);

						//Updating metrics variables
						will += 2; //Increase local count by x2 (account for N(u) reply)
					}
				}
				break;
			case CONVEX_HULL:
				for (int i = 0; i < resultCGAL.size(); i++)
				{
					//Getting id from node coordinates
					int idAux = coordinateToId(resultCGAL[i].x(), resultCGAL[i].y());

					//No need to send to my parent - nor to myself ({u} U N(u))
					if (idAux != parentId && idAux != nodeId)
					{
						//Debuging msg
						debugMesseging(idAux, "MSG_REQUEST", -1);

						//Let who sent the message now I'm acknowledging that the mule is serving it
						infoSent = 1;
						MPI_Send(&infoSent, 1, MPI_INT, idAux, MSG_REQUEST, MPI_COMM_WORLD);

						//Updating metrics variables
						will += 2; //Increase local count by x2 (account for N(u) reply)
					}
				}
				break;
			case ALPHA_SHAPE:
				for (int i = 0; i < resultCGAL.size(); i++)
				{
					//Getting id from node coordinates
					int idAux = coordinateToId(resultCGAL[i].x(), resultCGAL[i].y());

					//No need to send to my parent - nor to myself ({u} U N(u))
					if (idAux != parentId && idAux != nodeId)
					{
						//Debuging msg
						debugMesseging(idAux, "MSG_REQUEST", -1);

						//Let who sent the message now I'm acknowledging that the mule is serving it
						infoSent = 1;
						MPI_Send(&infoSent, 1, MPI_INT, idAux, MSG_REQUEST, MPI_COMM_WORLD);

						//Updating metrics variables
						will += 2; //Increase local count by x2 (account for N(u) reply)
					}
				}
				break;
			default:
				//Greedy by default
				for (int i = 0; i < my_neighbors_ids.size(); i++)
				{
					//No need to send to my parent
					if (my_neighbors_ids[i] != parentId)
					{
						//Debuging msg
						debugMesseging(my_neighbors_ids[i], "MSG_REQUEST", -1);

						//Let who sent the message now I'm acknowledging that the mule is serving it
						infoSent = 1;
						MPI_Send(&infoSent, 1, MPI_INT, my_neighbors_ids[i], MSG_REQUEST, MPI_COMM_WORLD);

						//Updating metrics variables
						will += 2; //Increase local count by x2 (account for N(u) reply)
					}
				}
				break;
		}
	}

	return;
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
	debugMesseging(u, "ACK_BEING_SERVED", -1); //Payload: # msgs sent

	//Let who sent the message now I'm acknowledging that the mule is serving it
	MPI_Send(&infoSent, 1, MPI_INT, u, ACK_BEING_SERVED, MPI_COMM_WORLD);

	return;
}

void SensorNode::msgAckBeingServedReceived(void) {
	neighbors_ACK_BEING_SERVED_buffer[u] = true;

	//Checking if all my N(v) have sent 'ACK_BEING_SERVED' back
	if (checkAllNeighborFlagAckBeingServedReceived())
	{
		//Debuging msg
		debugMesseging(parentId, "ACK_SERVED", -1);

		//Let who sent the message now I'm acknowledging that the mule is serving it
		infoSent = will; //Send if back to my parent
		MPI_Send(&infoSent, 1, MPI_INT, parentId, ACK_SERVED, MPI_COMM_WORLD); //REVIEW - maybe Pablo is sending to the wrong place here
	}


	return;
}

void SensorNode::msgRequestReceived(void) {
	//Saving # of neighbors not served yet I have to msg's payload
	infoSent = unattendedNeigbors;

	//Debuging msg
	debugMesseging(u, "MSG_ENUMERNODES", infoSent);

	//Sending the # of yet to be served neighbors I have to whoever sent me 'MSG_REQUEST' (my parent)
	MPI_Send(&infoSent, 1, MPI_INT, u, MSG_ENUMERNODES, MPI_COMM_WORLD); //REVIEW - maybe Pablo is sending to the wrong place here

	return;
}

void SensorNode::msgEnumernodesReceived(void) {
	//Saving (my neighbor) u's # of yet to be served neighbors
	neighbors_MSG_ENUMERNODES_buffer[u].first = true;
	neighbors_MSG_ENUMERNODES_buffer[u].second = infoRec;

	//All of my neighbors (except my father) have sent me back their updated # of neighbors waiting to be served
	if (checkAllMsgEnumernodesReceived())
	{
		//I still have to send the mule to someone - one of my neighbors have neighbor(s) yet to be served
		if (workToBeDoneStill())
		{
			//Mule will be moved - update total msgs sent thus far
			if (nodeId != 0)
			{
				infoSent = will;
				MPI_Send(&infoSent, 1, MPI_INT, 0, SAVE_MSGING_COUNT, MPI_COMM_WORLD); //Saving all msgs sent up to this point
				infoSent = 0;
			}
			else {
				totalMsgsSent += will;
			}

			//Choosing where the mule will be sent next based on the elected method of choice
			int choseNeighbor = methodOfChoice();

			//Updating number of moves the mule has made
			infoSent = (steps + 1);

			//Sending the # of yet to be served neighbors I have to whoever sent me 'MSG_REQUEST' (my parent)
			MPI_Send(&infoSent, 1, MPI_INT, choseNeighbor, SEND_MULE, MPI_COMM_WORLD); //REVIEW - maybe Pablo is sending to the wrong place here

			//Debuging msg
			debugMesseging(choseNeighbor, "SEND_MULE", -1);

			//Mule not with me anymore
			isMuleWithMe = false;
		}
		else {
			//All neighbors served

			//Im at the 1st node (and there are no more neighbors to be served) - RESCUME EXECUTION
			if (nodeId == 0)
			{
				//Update total msgs sent thus far by the last time - all msgs accounted
				totalMsgsSent += will;

				//Flaging end of processing of the network
				endExec = true;

				//Broadcasting to my neighbors
				for (int i = 0; i < my_neighbors_ids.size(); i++)
				{
					MPI_Send(&infoSent, 1, MPI_INT, my_neighbors_ids[i], SEND_END, MPI_COMM_WORLD);
				}
			}
			else {
				// ** ERROR HERE ** - passing here twice by the end - MULE BEING SENT TO 0 TWICE (CONSECUTIVE).

				//Mule will be moved - update total msgs sent thus far
				if (nodeId != 0)
				{
					infoSent = will;
					MPI_Send(&infoSent, 1, MPI_INT, 0, SAVE_MSGING_COUNT, MPI_COMM_WORLD); //Saving all msgs sent up to this point
					infoSent = 0;
				}
				else {
					totalMsgsSent += will;
				}

				//Updating number of moves the mule has made
				infoSent = (steps + 1);

				//Send mule back to my parent
				MPI_Send(&infoSent, 1, MPI_INT, parentId, SEND_MULE, MPI_COMM_WORLD); //REVIEW - maybe Pablo is sending to the wrong place here

				//Debuging msg
				debugMesseging(parentId, "SEND_MULE", -1);

				//Mule not with me anymore
				isMuleWithMe = false;
			}
		}
	}

	return;
}

void SensorNode::msgSendMuleReceived(void) {
	//Reseting counters
	will = 0;

	//Updating the mule steps counter - mule has just moved here
	steps = infoRec;

	//Mule has arrived at me for the 1st time - INTRODUCED TO FIX ERROR_001 AND ERROR_001.1
	if (!isMuleWithMe && !hasMuleBeenWithMe)
	{
		//Mule has just arrived, can serve me and my N(u) - thus no pendent neighbors to be served left
		unattendedNeigbors = 0;

		//Mule is with me
		isMuleWithMe = true;

		hasMuleBeenWithMe = true; //INTRODUCED TO FIX ERROR_001 AND ERROR_001.1

		//Mule is currently serving me
		served = true;

		//My parent has sent me the mule
		parentId = u;

		//Reseting flag that marks the event of a N(u) having sent 'ACK_SERVED' back
		resetNeigAckServedSentBackBuffer();

		//Then I'm backtracking
		if (u != parentId) //INTRODUCED TO FIX ERROR_001 AND ERROR_001.1
		{
			backtracking = true;
			neighbors_MSG_ENUMERNODES_buffer[u].first = true;
			neighbors_MSG_ENUMERNODES_buffer[u].second = 0;
		}

		//Notifying N(u) that u is being served by the mule
		for (int i = 0; i < my_neighbors_ids.size(); i++)
		{
			//No need to sent to my father
			if (my_neighbors_ids[i] != parentId)
			{
				//Debuging msg
				debugMesseging(my_neighbors_ids[i], "MSG_SERVED", -1);

				//Let N(u) know I'm being served
				MPI_Send(&infoSent, 1, MPI_INT, my_neighbors_ids[i], MSG_SERVED, MPI_COMM_WORLD);

				//Updating metrics variables
				will += 2;
			}
		}
	}
	else if (hasMuleBeenWithMe) {
		//Mule has been sent here before and I have already be served - must check for neighbors that need the mule

		//Mule is with me - again
		isMuleWithMe = true; //INTRODUCED TO FIX ERROR_001 AND ERROR_001.1

		//Resenting the counting of # of yet to be served neighbors of each of my neighbors - no need to wait for 'ACK_SERVED'
		resetNeigEnumernodesBuffer();

		//Then I'm backtracking
		if (u != parentId) //INTRODUCED TO FIX ERROR_001 AND ERROR_001.1
		{
			backtracking = true;
			neighbors_MSG_ENUMERNODES_buffer[u].first = true;
			neighbors_MSG_ENUMERNODES_buffer[u].second = 0;
		}

		switch (selected)
		{
			case GREEDY:
				//Requesting number of yet to be served neighbors of my neighbors
				for (int i = 0; i < my_neighbors_ids.size(); i++)
				{
					if (my_neighbors_ids[i] != u) //INCORPORATED TO APPLY IMPROVE_000
					{
						//Debuging msg
						debugMesseging(my_neighbors_ids[i], "MSG_REQUEST", -1);

						//Let who sent the message now I'm acknowledging that the mule is serving it
						MPI_Send(&infoSent, 1, MPI_INT, my_neighbors_ids[i], MSG_REQUEST, MPI_COMM_WORLD);

						//Updating metrics variables
						will += 2;
					}
				}
				break;
			case CONVEX_HULL:
				for (int i = 0; i < resultCGAL.size(); i++)
				{
					int idAux = coordinateToId(resultCGAL[i].x(), resultCGAL[i].y());

					if (idAux != u && idAux != nodeId) //INCORPORATED TO APPLY IMPROVE_000
					{
						//Debuging msg
						debugMesseging(idAux, "MSG_REQUEST", -1);

						//Let who sent the message now I'm acknowledging that the mule is serving it
						MPI_Send(&infoSent, 1, MPI_INT, idAux, MSG_REQUEST, MPI_COMM_WORLD);

						//Updating metrics variables
						will += 2;
					}
				}
				break;
			case ALPHA_SHAPE:
				for (int i = 0; i < resultCGAL.size(); i++)
				{
					int idAux = coordinateToId(resultCGAL[i].x(), resultCGAL[i].y());

					if (idAux != u && idAux != nodeId) //INCORPORATED TO APPLY IMPROVE_000
					{
						//Debuging msg
						debugMesseging(idAux, "MSG_REQUEST", -1);

						//Let who sent the message now I'm acknowledging that the mule is serving it
						MPI_Send(&infoSent, 1, MPI_INT, idAux, MSG_REQUEST, MPI_COMM_WORLD);

						//Updating metrics variables
						will += 2;
					}
				}
				break;
			default:
				//Greedy by default
				for (int i = 0; i < my_neighbors_ids.size(); i++)
				{
					if (my_neighbors_ids[i] != u) //INCORPORATED TO APPLY IMPROVE_000
					{
						//Debuging msg
						debugMesseging(my_neighbors_ids[i], "MSG_REQUEST", -1);

						//Let who sent the message now I'm acknowledging that the mule is serving it
						MPI_Send(&infoSent, 1, MPI_INT, my_neighbors_ids[i], MSG_REQUEST, MPI_COMM_WORLD);

						//Updating metrics variables
						will += 2;
					}
				}
				break;
		}
	}
	else {
		errorHasOccoured("No action taken upon mule arrival (SEND_MULE msg received)");
	}

	return;
}

void SensorNode::broadcastProgramTermination(void) {
	//Program termination required
	endExec = true;

	//Broadcasting to my neighbors
	for (int i = 0; i < my_neighbors_ids.size(); i++)
	{
		MPI_Send(&infoSent, 1, MPI_INT, my_neighbors_ids[i], SEND_END, MPI_COMM_WORLD);
	}

	return;
}