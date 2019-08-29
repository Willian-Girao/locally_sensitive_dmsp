#pragma once

#include "SensorNode.h"

using namespace std;

//Constructor.
SensorNode::SensorNode(string instanceFileName, int sensorId) {
	nodeId = sensorId;

	ifstream instance_file(instanceFileName);
	string str;

	int line_index = 0;
	int total_nodes = 0;
	int nodeFileLine = sensorId + 1;

	while(getline(instance_file, str)) 
	{
		string parsed_line = "";

		switch(line_index) 
		{
			case 0 :
				for(char& c : str) 
				{
					if(!isspace(c) && c != '\n') 
					{
						parsed_line += c;
					}
			    }

				total_nodes = stoi(parsed_line);
				break;
			default :
				if(line_index == nodeFileLine)
				{
					string x, y = "";
					bool processing_x = true;

					for(char& c : str) 
					{
						if(!isspace(c) && c != '\n' && processing_x) 
						{
							x += c;
						} else {
							processing_x =  false;
						}

						if(!isspace(c) && c != '\n' && !processing_x) 
						{
							y += c;
						}
				    }
				    
				    coords = make_pair<double,double>(stod(x), stod(y));
				}
				break;
		}

	    line_index++;
	}

	instance_file.close();
	cout << "> Instance closed." << endl;
}

//Destructor.
SensorNode::~SensorNode() {}