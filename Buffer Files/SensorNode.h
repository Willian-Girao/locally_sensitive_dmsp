#ifndef SENSORNODE_H
#define SENSORNODE_H

#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <map>

using namespace std;

class SensorNode {
    //TODO - codes "if's"/msg sending and receiving/checkings should all become this classes's getters/setters.
    public:
        SensorNode(string instanceFileName, int sensorId);
        ~SensorNode();

        /* Getters */

        /* Setters */

        /* Utils */

        /* Utilities */

    private:
        int nodeId; //Graph's node id (sensor within the network)

        /* Botton two properties are used to calculate the Alpha-Shape of (N(u) U {u})*/
        map<int, pair<double, double>> my_coordinates;


        //TODO - Change this for a map of 'int' and 'pair'.
        //TODO - 'pair' has to become a Point for the CGLA lib.
        vector<pair<int, pair<double, double>>> my_neighbors;
};

#endif
