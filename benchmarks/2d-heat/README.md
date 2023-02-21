
<h1>How use?</h1>

In /src directory:

1) Make

2) mpirun -np [ x ] ./mpi-2dheat.x -t -w [ x ] -h [ x ] -m 3 -e 3.0

-w: Widths = 64 128 256 512
-h: Heights = 64 128 256 512
-np: Procs = 1 2 4 8 16 28
-m: Method = 3
-e: Epsilon = 3.0

Example: 

- mpirun -np 2 ./mpi-2dheat.x -t -w 64 -h 64 -m 3 -e 3.0
- mpirun -np 4 ./mpi-2dheat.x -t -w 64 -h 64 -m 3 -e 3.0