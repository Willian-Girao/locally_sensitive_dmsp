# Setup

For the following instruction it is assumed that the code will be executed on a Linux environment.

## Dependencies

- g++
- libcgal-dev
- openmpi-bin
- libopenmpi-dev

### Compile and Run

In order to compile the code, run the following set of commands on a Linux terminal:

```cpp
mpicxx -c SensorNode.cpp -lCGAL -lgmp -frounding-math -o sensor.o && mpicxx -c solver.cpp -lCGAL -lgmp -frounding-math -o main.o && mpicxx main.o sensor.o -lCGAL -lgmp -frounding-math -o solver_exe && mpirun -np # -mca btl sm,self --allow-run-as-root solver_exe
```

where **#** is the number of nodes within the instance being processed and will equal the number of processes needed to simulate the sensor network.
