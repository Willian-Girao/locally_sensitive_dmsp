/* Standard libraries */
#include <iostream>
#include <array>
#include <stdlib.h>
#include <time.h>

/* Headers containing classes definitions */
#include "SensorNode.h"

/* Cpp files containing classes implementations */
#include "SensorNode.cpp"

using namespace std;

int main() {	
	string instanceFileName = "./test_instances/instance_input_test00.dat";

	// SensorNode node0(instanceFileName, 0);
	// SensorNode node1(instanceFileName, 1);
	// SensorNode node2(instanceFileName, 2);
	// SensorNode node3(instanceFileName, 3);
	SensorNode node4(instanceFileName, 4);
	// SensorNode node5(instanceFileName, 5);

	cout << "Unattended count thus far: " << node4.getUnattendedNeigbors() << endl;

	return 0;
}