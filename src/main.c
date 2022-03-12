#include "utils.h"

MPI_Datatype mpi_point;

int main(int argc, char** argv) {
    
    MPI_Init(&argc, &argv);
    
    int npoints = NPOINTS, subtree_size = 0;
    data_t *data;
    
    
    int my_rank = 0, master = 0, nprocs = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    if((nprocs & (nprocs - 1)) != 0) {
        if(my_rank == master)
            printf("USAGE: This application is meant to be run with a number of processes which is a power of two.\n USAGE: mpirun -np nprocs kdtree [npoints] [file]\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    
    //load dataset
    if(my_rank == master) {
        if(argc > 1) 
            npoints = atoi(argv[1]);

        if(argc > 2) {
            // read data if provided
            npoints = countlines(argv[2]);
            data = (data_t*)malloc(npoints*sizeof(data_t));
            load_dataset(data, argv[2], npoints);
          //  MPI_Bcast(&npoints, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
	    else {
	        // otherwise generate points randomly
	        data = (data_t*)malloc(npoints*sizeof(data_t));
            
            for (int i = 0; i < npoints; i++ )
                for (int j = 0; j < NDIM; j++)
                    data[i].data[j] = drand48()*MAX_VALUE;
                
	    }
    }
    MPI_Bcast(&npoints, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    
    // build mpi_point type to exchange data points between MPI processes
    build_mpi_point_type();
    
    // root node for each process and the level at which 
    // it must be placed        
    struct knode *root = NULL;
    int level;
    
    struct timespec ts;
    double tstart, tend;
    
    // find first nprocs root split node
    if(my_rank == master) { 
        level = 0;
        tstart = CPU_TIME;
        #if defined(_OPENMP)
            #pragma omp parallel
            {
                #pragma omp single nowait
	            root = first_ksplit(data, npoints, -1, level, nprocs, my_rank);
	        }
	    #else
	         root = first_ksplit(data, npoints, -1, level, nprocs, my_rank);
	    #endif
	} 
    
    // each process receives the size of its subset and 
    // waits for the data to start building the subtree
    if(my_rank != master) {
        MPI_Recv(&subtree_size, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&level, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    
    data_t *received = (data_t*)malloc(subtree_size*sizeof(data_t));//[subtree_size];
    
    // each process saves the data assigned to build its subtree
	if (my_rank != master) {
	     MPI_Recv(received, subtree_size, mpi_point, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	     
	    //number of process which work on each subtree is nprocs/2^level
        int pow = 1 << level;
        
        //while the axis used at the previous level is usually (level+1)%2
        #if defined(_OPENMP)
        #pragma omp parallel
        {
            #pragma omp single nowait
            root = first_ksplit(received, subtree_size, (level+1)%NDIM, level, nprocs/pow, my_rank);
        }
        #else
          root = first_ksplit(received, subtree_size, (level+1)%NDIM, level, nprocs/pow, my_rank);
        #endif
    }
    // wait for each process for finishing building
    MPI_Barrier(MPI_COMM_WORLD);
    
    if(my_rank == master) {
        tend = CPU_TIME;  
        printf("Time taken to build the kdtree: %.2f \n", tend-tstart);
    }
    
    //print the tree
    //print_kdtree(root, level, nprocs, my_rank);
    
    free_tree(root);
    free(received);
	if(my_rank == master) 
	    free(data);
	    
	MPI_Finalize();
 
    return EXIT_SUCCESS;
}
