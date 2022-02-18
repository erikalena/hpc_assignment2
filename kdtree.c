#include "kdtree.h"

/*
	---------------
	Auxiliary functions
	--------------- 
*/

void print_kdtree(struct knode* node, int level, int nprocs, int rank) {
    FILE *fptr = fopen("final_kdtree.txt", "w");
    fclose(fptr);
    for(int i = 0; i < nprocs; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (i == rank) {
            FILE *fptr = fopen("final_kdtree.txt", "a");
            print_proc_subtree(node, level, fptr);
            fclose(fptr);
        }
    }
}

void print_proc_subtree(struct knode* node, int level, FILE *fp) {
    if(node != NULL) {
        for(int i = 0; i < level; i++){
            fprintf(fp, "\t");
        }
        fprintf(fp,"(");
        for(int i = 0; i < NDIM; i++) {
            fprintf(fp, i == (NDIM - 1)? "%.2f)\n" : "%.2f,", node->split_point[i]);
        } 
        print_proc_subtree(node->left, level+1, fp);
        print_proc_subtree(node->right, level+1, fp);
    }
}

int choose_split_dim(data_t *points, int n, int axis) {
	int new_axis;
	new_axis = (axis+1)%NDIM;
	
	// manage data extension
	// if in one dimension the extension is more than double the 
	// extension in the chosen dimension, then the dimension is changed
	float_t extension[NDIM];
	float_t max=0, min=MAX_VALUE;
	float_t max_extension = new_axis;
	
	for(int i = 0; i < NDIM; i++) {
	    for(int j = 0; j < n; j++) {
	        max = points[i].data[j] > max ? points[i].data[j] : max;
	        min = points[i].data[j] < min ? points[i].data[j] : min;
	    }
	    extension[i] = max - min;
	    max_extension = max_extension < extension[i] ? i : max_extension;
	}
	
	new_axis = extension[new_axis]*2 < max_extension ? max_extension : new_axis;
	
	return new_axis;
}

/*
	---------------
	Build up kdtree
	--------------- 
*/

struct knode* build_kdtree(data_t *points, int n, int axis, int level) {
    #if defined(_OPENMP)
        printf("I'm thread %d and I'm working at level %d\n", omp_get_thread_num(), level);
    #endif
    
	// allocate memory for a new node or implement something different
	// and more efficient in terms of tree-traversal (see slides, but in any case think about a list which does not need to manage all tree data ?)
    struct knode *node = (struct knode*) malloc(sizeof(struct knode));
    
    if (n == 0) {
        node = NULL;
    }
	else if( n == 1) {
		// just one point left: return a leaf 
		node->axis = -1;
 		node->split_point = (float_t*) &points[0];
		node->left = NULL;
		node->right= NULL;
		
		#if defined(DEBUG)
		printf("Level %d: split dimension is %d \n\t split node is (%f, %f)\n", level,-1, points[0].data[0], points[0].data[1]);
		#endif
	} 
	else {		
		// decide new splitting axis in a round-robin fashion
		int new_axis = choose_split_dim(points, n, axis);
		
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
	   
	    #if defined(DEBUG)
		printf("Level %d: split dimension is %d \n\t split node is (%f, %f)\n", level, new_axis, points[n_left].data[0], points[n_left].data[1]);
		#endif
		
		//recursively build up left and right subtrees 
		#if defined(_OPENMP)
		    #pragma omp task
		        node->left = build_kdtree(lpoints, n_left, new_axis, level+1);
		    #pragma omp task
		        node->right = build_kdtree(rpoints, n_right, new_axis, level+1);
        #else
            node->left = build_kdtree(lpoints, n_left, new_axis, level+1);
            node->right = build_kdtree(rpoints, n_right, new_axis, level+1);
        #endif
		
	}
		
	return node; 
}

        
        

