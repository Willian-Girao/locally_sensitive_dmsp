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
        //Node.
        int getNodeId(void); //Returns the
        int getUnattendedNeigbors(void); //Returns the
        int getU(void); //Returns the
        bool checkHasBenServed(void); //Returns the

        //Execution.
        int getTotalNodes(void); //Returns the
        int getMuleStepsCountThusFar(void); //Returns the
        int getParentId(void); //Returns the
        bool checkIsExecEnded(void); //Returns the

        //Metrics.
        double getStartServeTime(void);
        double getEndServeTime(void);
        double getStartServedTime(void);
        double getEndServedTime(void);
        double getStartRequestTime(void);
        double getEndRequestTime(void);
        double getMaxTime(void);


        /* Setters */
        //Metrics.
        void setStartServeTime(double x);
        void setEndServeTime(double x);
        void setStartServedTime(double x);
        void setEndServedTime(double x);
        void setStartRequestTime(double x);
        void setEndRequestTime(double x);
        void setMaxTime(double x);

        //Execution.
        void getTotalNodes(int y);
        void getParentId(int y);
        void getMuleStepsCountThusFar(int y);

        //Node.
        void setNodeId(int z);
        void setUnattendedNeigbors(int z);
        void setU(int z);
        void setHasBenServed(bool z);

        /* Utils */
        void pauseExec(void);

    private:
        /* Properties */
        //Node associated properties.
        int nodeId; //Graph's node id (sensor within the network)
        int unattendedNeigbors; //Number of neighbors yet to be served
        int u; //Identifies which node has send me the msg 'MSG_SERVED'
        bool served; //Flagged once the mule has passed by the node

        //Execution controll associated properties.
        int totalNodes; //Total number of sensors whithin the network
        int parentId; //The parent of the node within the mules path on the network
        int steps; //# of movements made by the mule thus far

        bool endExec; //Flags the end of the execiton
        bool isMuleWithMe; //Checks whether or not the mule is at the current node

        //Metrics related properties.
        double startServeTime;
        double endServeTime;
        double startServedTime;
        double endServedTime;
        double startRequestTime;
        double endRequestTime;
        double maxTime;

        //Botton two properties are used to calculate the Alpha-Shape of (N(u) U {u})
        map<int, pair<double, double>> my_coordinates;
        vector<int> my_neighbors_ids;

        //TODO - Change this for a map of 'int' and 'pair'.
        //TODO - 'pair' has to become a Point for the CGLA lib.
        map<int, pair<double, double>> my_neighbors_xy;

        /* Methods */

        //Utils.
        void pause(void); //Pauses the program for debbuging purposes
};

#endif
