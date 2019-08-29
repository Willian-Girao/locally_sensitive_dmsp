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

	SensorNode nodeI(instanceFileName, 1);

	return 0;
}