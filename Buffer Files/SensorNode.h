#ifndef SENSORNODE_H
#define SENSORNODE_H

#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iomanip>

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
        //Node associated properties.
        int nodeId; //Graph's node id (sensor within the network)
        int unattendedNeigbors; //Number of neighbors yet to be served
        int quemMsgAtendMula; //Identifies which node has send me the msg 'MSG_SERVED'
        int u; //Identifies which node has send me the msg 'MSG_SERVED'
        bool served = false; //Flagged once the mule has passed by the node

        //Execution controll associated properties.
        int totalNodes; //Total number of sensors whithin the network
        int parentId; //The parent of the node within the mules path on the network
        int steps; //# of movements made by the mule thus far
        bool endedExec; //Flags the end of the execiton
        bool ended; //Flags the termination of the program execution
        bool isMuleWithMe; //Checks whether or not the mule is at the current node

        //Metrics related properties.
        double startServeTime;
        double endServeTime;
        double startServedTime;
        double endServedTime;
        double startRequestTime;
        double endRequestTime;
        double maxTime;


        /* Botton two properties are used to calculate the Alpha-Shape of (N(u) U {u})*/
        map<int, pair<double, double>> my_coordinates;
        vector<int> my_neighbors_ids;

        //TODO - Change this for a map of 'int' and 'pair'.
        //TODO - 'pair' has to become a Point for the CGLA lib.
        map<int, pair<double, double>> my_neighbors_xy;
};

#endif
