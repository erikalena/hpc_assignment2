#include "utils.h"



int main(int argc, char** argv) {
    
    MPI_Init(&argc, &argv);
    
    int npoints = NPOINTS, subtree_size = 0;
    data_t *data;

    int my_rank = 0, master = 0, nprocs = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    //load dataset
    if(my_rank == master) {
        if(argc > 1) {
            // read data if provided
            npoints = countlines(argv[1]);
            data = (data_t*)malloc(npoints*sizeof(data_t));
            load_dataset(data, argv[1], npoints);
            MPI_Bcast(&npoints, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
	    else {
	        // otherwise generate points randomly
	        data = (data_t*)malloc(npoints*sizeof(data_t));
            
            for ( int i = 0; i < npoints; i++ )
            for (int j = 0; j < NDIM; j++)
                data[i].data[j] = drand48()*MAX_VALUE;
	    }
    }
    
   /* build mpi point type */
    int lengths[1] = {NDIM};
    MPI_Aint displacements[1];
    data_t point;
    MPI_Aint base_address;
    MPI_Get_address(&point, &base_address);
    MPI_Get_address(&point.data, &displacements[0]);
    displacements[0] = MPI_Aint_diff(displacements[0], base_address);

    MPI_Datatype types[1] = { mpif_t };
    MPI_Type_create_struct(1, lengths, displacements, types, &mpi_point);
    MPI_Type_commit(&mpi_point);
    ////
    
    // root node for each process and the level at which 
    // it must be placed        
    struct knode *root = NULL;
    int level;
    
    // find first nprocs root split node
    if(my_rank == master) { 
        level = 0;
	    root = first_ksplit(data, npoints, -1, level, nprocs, my_rank);
	} 
    
    // each process receives the size of its subset and 
    // waits for the data to start building the subtree
    if(my_rank != 0) {
        MPI_Recv(&subtree_size, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&level, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    
    data_t received[subtree_size];
    
    // each process saves the data assigned to build
    // its subtree
	if (my_rank != 0 ) {
	    if(subtree_size != 0) {
	        MPI_Recv(&received, subtree_size, mpi_point, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	     }
	    //number of process which work on each subtree is nprocs/2^level
        int pow = 1 << level;
        //while the axis used at the previous level is usually (level+1)%2
        root = first_ksplit(received, subtree_size, (level+1)%NDIM, level, nprocs/pow, my_rank);
    }
    // wait for each process for finishing building
    MPI_Barrier(MPI_COMM_WORLD);
    
    // print the tree
    if(my_rank == master) printf("Print tree\n");
    print_kdtree(root, level, nprocs, my_rank);
    
 
     
	/*
	//build kdtree on the given dataset
	#if defined(_OPENMP)

    #pragma omp parallel
    {
        #pragma omp single
	    root = build_kdtree(data, npoints, -1, 0);
    }
    #else 
         root = build_kdtree(data, npoints, -1, 0);
    #endif
    
    print_kdtree(root, 0);  
    */ 
	MPI_Finalize();
 
    return EXIT_SUCCESS;
}
