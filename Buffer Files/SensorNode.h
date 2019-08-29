#ifndef SENSORNODE_H
#define SENSORNODE_H

#include <iostream> 
#include <fstream>
#include <string>
#include <vector>

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
        //TODO - codes variables should all become this classes's properties.
        int nodeId;

        /* Botton two properties are used to calculate the Alpha-Shape of (N(u) U {u})*/
        pair<int, pair<double, double>> my_coordinates;
        vector<pair<int, pair<double, double>>> my_neighbors;
        // 'pair<int, pair<double, double>>' has to become a Point for the CGLA lib.
};

#endif
