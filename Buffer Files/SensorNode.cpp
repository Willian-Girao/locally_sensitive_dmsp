#pragma once

#include "SensorNode.h"

using namespace std;

//Constructor.
SensorNode::SensorNode(string instanceFileName, int sensorId) {
	nodeId = sensorId;

	ifstream instance_file(instanceFileName);

	//Parsing file lines as a vector in order to optimize the access to the neighbors information.
	vector<string> file_lines;

	for (string line; getline(instance_file, line); )
	{
		file_lines.push_back(line);
	}

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

    	// my_neighbors_xy.push_back(n_i);
    }

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