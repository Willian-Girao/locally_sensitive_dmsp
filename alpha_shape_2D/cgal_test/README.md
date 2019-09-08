# How to execute
- Build
- npmexec.exe -np **{numNodes}** solver.exe **{instanceFileName}** **{debugOption}** **{debugLevel}**

**numNode** - total number (integer) of nodes constituting the graph G represented in *instanceFileName*

**instanceFileName** - path (e.g. "instances/file_name.dat") to file (with .dat extension) containing a problem instance

**debugOption** - yes/no option for debugging messages (messages sent between nodes on the net)

**debugLevel** - granularity of messages (integer) shown during execution
- 1 - All messages are shown
- 2 - Only mule movements are shown
