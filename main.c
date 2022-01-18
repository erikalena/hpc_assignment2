#include "kdtree.h"


MPI_Datatype mpi_point;

int countlines(char* file) {
    FILE* fp = fopen(file, "r");
    int count = 0,c;
	if(fp != NULL) {
        while ((c = getc(fp)) != EOF) 
              if (c == '\n') // Increment count if this character is newline 
                 count++; 
    }
    fclose(fp);
    return count;
}    


struct knode* first_ksplit(data_t *points, int n, int axis, int level, int nprocs, int rank) {
	// allocate memory for a new node or implement something different
	// and more efficient in terms of tree-traversal (see slides, but in any case think about a list which does not need to manage all tree data ?)
	printf("I'm process %d entering first split with %d procs at level %d\n", rank, nprocs, level);
    struct knode *node = (struct knode*) malloc(sizeof(struct knode));
    if (n == 0){
        node = NULL;
         // right part is send to another process
         if(nprocs > 1) {
		    send_subset(NULL, 0, level+1, rank + nprocs/2);
		    first_ksplit(NULL, 0, -1, level+1, nprocs/2, rank);
		 }
        /*
        for(int i = 1; i < nprocs; i++) {
		    // if there are other process waiting notify them the must do nothing
            int diff_level = (i/2) + 1;
            send_subset(NULL, 0, level+diff_level, rank + i);
		} */
    }
	else if( n == 1) {
		// just one point left: return a leaf 
		node->axis = -1;
 		node->split_point = (float_t*) &points[0];
		node->left = NULL;
		node->right= NULL;
		
		#if defined(DEBUG)
		printf("%d: Level %d: split dimension is %d \n\t split node is (%f, %f)\n",rank, level,-1, points[0].data[0], points[0].data[1]);
		#endif
		
		if(nprocs > 1) {
		    send_subset(NULL, 0, level+1, rank + nprocs/2);
		    first_ksplit(NULL, 0, -1, level+1, nprocs/2, rank);
		 }
		/*
		for(int i = 1; i < nprocs; i++) {
		    // if there are other process waiting notify them they must do nothing
		    int diff_level = (i/2) + 1;
		    send_subset(NULL, 0, level+diff_level, rank + i);
		}*/
	} 
	else {		
	    //if the number of split is smaller than the number of processes,
		//split again, take one side and give the other half to another process
		if(level < MAX_LEVEL && nprocs > 1) {
		
		    // decide new splitting axis in a round-robin fashion
		    int new_axis = choose_split_dim(points, n, axis);
		    
		    //once the axis is chosen, determine the value of the split
		    //if data are homogeneous we can take the middle point 
		    int n_left = n/2;
	        int n_right = n - n_left -1;
		    int val = points[n_left].data[new_axis];
		    
		    node->split_point = (float_t*) &points[n_left];
	        node->axis  = new_axis;

		    //left and right subsets
		    data_t *lpoints, *rpoints;
		    lpoints = points;
		    rpoints = points + n_left + 1;
	       
	        #if defined(DEBUG)
		    printf("I'm process %d sending right part to process %d \n Level %d: split dimension is %d \n\t split node is (%f, %f)\n", rank, rank+nprocs/2,level, new_axis, points[n_left].data[0], points[n_left].data[1]);
		    #endif
		
		    // right part is send to another process
		    send_subset(rpoints, n_right, level+1, rank + nprocs/2);
		    node->right = NULL;
		    
		    node->left = first_ksplit(lpoints, n_left, new_axis, level+1, nprocs/2, rank);
	    }
		else if (level < MAX_LEVEL) {
		    // otherwise create your sub_kdtree with the data you have
		    node = build_kdtree(points, n, axis, level);
		} 
	}
		
	    return node;
}

void send_subset(data_t *points, int size, int level, int receiver) {
    if(size > 0) {
        data_t buffer[size];
        memcpy(buffer, points, (size)*sizeof(data_t));
        MPI_Send(&size, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);  
        MPI_Send(&level, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);  
        MPI_Send(&buffer, size, mpi_point, receiver, 0, MPI_COMM_WORLD); 
    } 
    else {
     MPI_Send(&size, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);  
     MPI_Send(&level, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);
    }
}


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
            // read data if given
		    npoints = countlines(argv[1]);
		    MPI_Bcast(&npoints, 1, MPI_INT, 0, MPI_COMM_WORLD);
		    
		    data = (data_t*)malloc(npoints*sizeof(data_t));
		    
		    int i = 0;
		    FILE* fp = fopen(argv[1], "r");
		    float x,y;
		    while(fscanf(fp,"%f,%f", &x, &y) ==2) {
			    data[i].data[0] = x;
			    data[i].data[1] = y;
			    i++;
		    }
		    fclose(fp);
	    }
	    else {
	        // otherwise generate points randomly
	        data = (data_t*)malloc(npoints*sizeof(data_t));
            
            for ( int i = 0; i < npoints; i++ )
            for (int j = 0; j < NDIM; j++)
                data[i].data[j] = drand48()*MAX_VALUE;
	    }
    }

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
    
	if (my_rank != 0 ) {
	    if(subtree_size != 0) {
	        MPI_Recv(&received, subtree_size, mpi_point, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	     }
	    //number of process which work on each subtree is nprocs/2^level
        int pow = 1 << level;
        //while the axis used at the previous level is usually (level+1)%2
        printf("I'm process %d calling split with %d procs, level %d\n",my_rank, nprocs/pow, level);
        root = first_ksplit(received, subtree_size, (level+1)%NDIM, level, nprocs/pow, my_rank);
    }
   
    MPI_Barrier(MPI_COMM_WORLD);
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
