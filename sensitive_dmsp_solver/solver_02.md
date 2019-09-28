## Setup (Linux) - CGAL

### Dependencies

- g++
- cmake
- libcgal-dev

### Compile

*g++ **<target_code_file>.cpp** -lCGAL -lgmp -frounding-math -o **<output_file>***

### Run

***./<output_file>***

## Setup (Linux) - MPI

### Dependencies

- openmpi-bin
- libopenmpi-dev

### Compile

*mpicxx **<target_code_file>.cpp** -o **<output_file>***

### Run

*mpirun -np **<num_process>** **./<compiled_file>***

## Solver_02 (Linux)

### Compile and Run

```cpp
mpicxx -c SensorNode.cpp -lCGAL -lgmp -frounding-math -o sensor.o && mpicxx -c solver.cpp -lCGAL -lgmp -frounding-math -o main.o && mpicxx main.o sensor.o -lCGAL -lgmp -frounding-math -o solver_exe && mpirun -np # -mca btl sm,self --allow-run-as-root solver_exe
```

