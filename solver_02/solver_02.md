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

### Compile

1. mpicxx -c SensorNode.cpp -lCGAL -lgmp -frounding-math -o sensor.o
2. mpicxx -c solver.cpp -lCGAL -lgmp -frounding-math -o main.o
3. *mpicxx **<output_file_2>.o** **<output_file_1>.o** -lCGAL -lgmp -frounding-math -o **<compiled_file>***

### Run

- *mpirun -np **<num_process>** -mca btl sm,self --allow-run-as-root **<compiled_file>** > **<output>** &*

