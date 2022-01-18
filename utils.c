#include "utils.h"


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

void load_dataset(data_t *data, char* file, int npoints) {
    int i = 0;
    FILE* fp = fopen(file, "r");
    float x,y;
    while(fscanf(fp,"%f,%f", &x, &y) ==2) {
	    data[i].data[0] = x;
	    data[i].data[1] = y;
	    i++;
    }
    fclose(fp);
    return npoints;
}


struct knode* first_ksplit(data_t *points, int n, int axis, int level, int nprocs, int rank) {
	// allocate memory for a new node or implement something different
	// and more efficient in terms of tree-traversal (see slides, but in any case think about a list which does not need to manage all tree data ?)
	//printf("I'm process %d entering first split with %d procs at level %d\n", rank, nprocs, level);
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
