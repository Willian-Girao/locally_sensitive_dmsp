#pragma once

#include "SensorNode.h"

using namespace std;

//Constructor.
SensorNode::SensorNode(string instanceFileName, int sensorId) {
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
	steps = 0; //Mule has made no moves thus far

	//Starting node associated variables
	isMuleWithMe = false; //Mule has made no moves thus far
	nodeId = sensorId; //Saving process id (aka nodeId; aka sensorId)
	parentId = -1; //Parent thusfar is unkown
	u = -1; //No msgs sent thus far

	//Opening file containing problem's instance in the form of a graph
	ifstream instance_file(instanceFileName);

	//Parsing file lines as a vector in order to optimize the access to the neighbors information.
	vector<string> file_lines;

	for (string line; getline(instance_file, line); )
	{
		file_lines.push_back(line);
	}

	totalNodes = stoi(file_lines[0]); //Setting total number of nodes (1st line of the file)

	//Utils local variables.
	string str;
	int nodeFileLine = sensorId + 1;
	int nodeNeighborsFileLine = sensorId + 3 + stoi(file_lines[0]); /* '+ 3' accounts for the for index shift 
	caused by NODES_COUNT and space between nodes coordinates and neighbors information. */

	//Error check.
	if (nodeNeighborsFileLine > file_lines.size())
	{
		cout << " > ERROR - NEIGHBORS FILE INDEX CALCULATED WRONG." << endl;
	}

	//Populating my_coords structure.
	vector<string> split_vector_nodeLine;

	istringstream isss(file_lines[nodeFileLine]);

    for(string s; isss >> s; ) {
        split_vector_nodeLine.push_back(s);
    }

    double dx = stod(split_vector_nodeLine[0]);
    double dy = stod(split_vector_nodeLine[1]);

    pair<double, double> my_xy(dx, dy);
    
    my_coordinates[sensorId] = my_xy;

	//Populating my_neighbors structure.
	string neighbors_count = "";
	vector<string> split_vector;

	istringstream iss(file_lines[nodeNeighborsFileLine-1]);

    for(string s; iss >> s; ) {
        split_vector.push_back(s);
    }

    for (int i = 1; i < stoi(split_vector[0]) + 1; ++i)
    {
    	my_neighbors_ids.push_back(stoi(split_vector[i]));

    	//Get neighbor coordinates
    	int neighborFileLine = stoi(split_vector[i]) + 1;
    	vector<string> neighbor_line_split;

		istringstream iss(file_lines[neighborFileLine]);

	    for(string s; iss >> s; ) {
	        neighbor_line_split.push_back(s);
	    }

    	double cx = stod(neighbor_line_split[0]);
    	double cy = stod(neighbor_line_split[1]);

    	pair<double, double> neighbor_coord(cx, cy);

    	my_neighbors_xy[stoi(split_vector[i])] = neighbor_coord;
    }

    unattendedNeigbors = stoi(split_vector[0]); //Initially all neighbors are unattended

    cout << "MY_ID: " << nodeId << " | " << "MY_COORDINATES: " << my_xy.first << ", " << my_xy.second << " | NEIGHBORS_LINE_INDEX: " << nodeNeighborsFileLine << endl;
    cout << "Neighbors: ";
    for (int i = 0; i < my_neighbors_xy.size(); ++i)
    {
    	// cout << my_neighbors_xy[my_neighbors_ids[i]].first << " ";
    	cout << my_neighbors_ids[i] << " (" << my_neighbors_xy[my_neighbors_ids[i]].first << ", " << my_neighbors_xy[my_neighbors_ids[i]].second << " ) ";
    }
    cout << endl;

	instance_file.close();
	cout << "\n\n> Instance closed." << endl;

}

//Destructor.
SensorNode::~SensorNode() {}

/* Methods - setters */
//Node.
void SensorNode::setNodeId(int z) {
	nodeId = z;
	return;
}

void SensorNode::setUnattendedNeigbors(int z) {
	unattendedNeigbors = z;
	return;
}

void SensorNode::setU(int z) {
	u = z;
	return;
}

void SensorNode::setHasBenServed(bool z) {
	served = z;
	return;
}


//Execution.
void SensorNode::getTotalNodes(int y) {
	totalNodes = y;
	return;
}

void SensorNode::getParentId(int y) {
	parentId = y;
	return;
}

void SensorNode::getMuleStepsCountThusFar(int y) {
	steps = y;
	return;
}


//Metrics
void SensorNode::setStartServeTime(double x) {
	startServeTime = x;
	return;
}

void SensorNode::setEndServeTime(double x) {
	endServeTime = x;
	return;
}

void SensorNode::setStartServedTime(double x) {
	startServedTime = x;
	return;
}

void SensorNode::setEndServedTime(double x) {
	endServedTime = x;
	return;
}

void SensorNode::setStartRequestTime(double x) {
	startRequestTime = x;
	return;
}

void SensorNode::setEndRequestTime(double x) {
	endRequestTime = x;
	return;
}

void SensorNode::setMaxTime(double x) {
	maxTime = x;
	return;
}


/* Methods - getters */

//Utils.
void SensorNode::pauseExec(void) {
	int a;
	cout << "\n - EXECUTION PAUSED...\n" << endl;
	cin >> a;
	return;
}

//Public.
//Getters - Metrics
double SensorNode::getStartServeTime(void) 
{
    return startServeTime;
}

double SensorNode::getEndServeTime(void) 
{
    return endServeTime;
}

double SensorNode::getStartServedTime(void) 
{
    return startServedTime;
}

double SensorNode::getEndServedTime(void) 
{
    return endServedTime;
}

double SensorNode::getStartRequestTime(void) 
{
    return startRequestTime;
}

double SensorNode::getEndRequestTime(void) 
{
    return endRequestTime;
}

double SensorNode::getMaxTime(void) 
{
    return maxTime;
}

//Getters - Node
int SensorNode::getNodeId(void) {
	return nodeId;
}

int SensorNode::getUnattendedNeigbors(void) {
	return unattendedNeigbors;
}

int SensorNode::getU(void) {
	return u;
}

bool SensorNode::checkHasBenServed(void) {
	return served;
}

//Getters - Execution
int SensorNode::getTotalNodes(void) {
	return totalNodes;
}

int SensorNode::getMuleStepsCountThusFar(void) {
	return steps;
}

int SensorNode::getParentId(void) {
	return parentId;
}

bool SensorNode::checkIsExecEnded(void) {
	return endExec;
}


//Private.