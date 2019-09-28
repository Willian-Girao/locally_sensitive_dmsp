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

*mpicxx **<target_code_file>.cpp** -lCGAL -lgmp -frounding-math -o **<output_file>***

### Run

- *mpirun -np **<num_process>** **<compiled_file>***
- *mpirun -np **<num_process>** --allow-run-as-root -mca btl sm,self **<compiled_file>***
- *mpirun --mca btl self,sm,tcp -np **<num_process>** --allow-run-as-root **<compiled_file>***
- <mark>*mpirun -np **<num_process>** -mca btl sm,self --allow-run-as-root **<compiled_file>** > **<output>** &*</mark>

