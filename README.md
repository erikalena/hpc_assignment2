# FHPC - Assignment 2

Required packages:
- MPICH

to install it:

```
$ sudo apt install mpich
```

To compile the code and run the executables:

```
$ make 
```

This will create two executables: *kdtree* and *omp_kdtree*, one compiled without OpenMP and the other with OpenMP enabled.
To run the code:

```
$ mpirun -np {n} ./omp_kdtree {n_points} {dataset}
```
where *n* is the number of MPI processes we want to use, while *n_points* is the size of the dataset
to be generated. If we provide as *dataset* a file, data points will be loaded from it and so the number of
data points will be the size of the given dataset.

Before running the code, if we want to test timing and performances using also OpenMP, it may be useful
to manually set some environment variables:

```
$ export OMP_NUM_THREADS={n}
$ export OMP_PROC_BIND={false | true | close | spread | master}
$ export OMP_PLACES={threads | cores | sockets}
```

To remove everything, including object files:

```
$ make clean
```



