#include "utils.h"

void build_mpi_point_type() {
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
}

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
}



struct knode* first_ksplit(data_t *points, int n, int axis, int level, int nprocs, int rank) {
    
    struct knode *node = (struct knode*) malloc(sizeof(struct knode));
    
    if(nprocs <= 1) { 
         #pragma omp parallel
        {
            #pragma omp single
                node = build_kdtree(points, n, axis, level);
        }
    }
    else if (n == 0 || n == 1) {
    
        node = build_kdtree(points, n, axis, level);

        // notify to processes which are waiting   
	    send_subset(NULL, 0, level+1, rank + nprocs/2);
	    first_ksplit(NULL, 0, -1, level+1, nprocs/2, rank);
	} 
	else {		
	    //if the number of split is smaller than the number of processes,
		//split again, take one side and give the other half to another process
		
	    // decide new splitting axis in a round-robin fashion
	    int new_axis = (axis+1)%NDIM;//choose_split_dim(points, n, axis);
	    // sort points with respect to chosen axis
        int mid = sorting(points, n, new_axis);

	    //once the axis is chosen, determine the value of the split
	    //if data are homogeneous we can take the middle point 
	    int n_left = mid;
        int n_right = n - n_left -1;
	    
	    node->split_point = (float_t*) &points[n_left];
	    
        node->axis  = new_axis;

	    //left and right subsets
	    data_t *lpoints, *rpoints;
	    lpoints = points;
	    rpoints = points + n_left + 1; 
	    
	    // right part is sent to another process
	    send_subset(rpoints, n_right, level+1, rank + nprocs/2);
	    node->right = NULL;
	    // current process continues building left branch
	    node->left = first_ksplit(lpoints, n_left, new_axis, level+1, nprocs/2, rank); 
	   
	}
		
    return node;
}

void send_subset(data_t *points, int size, int level, int receiver) {
    // copy data structure and send its size, the level at which subtree
    // must be must be placed and the data structure
    data_t buffer[size];
    memcpy(buffer, points, (size)*sizeof(data_t));
    MPI_Send(&size, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);  
    MPI_Send(&level, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);  
    MPI_Send(&buffer, size, mpi_point, receiver, 0, MPI_COMM_WORLD); 
}
