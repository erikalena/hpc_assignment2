#pragma once
#include <string.h>
#include <omp.h>
#include <time.h>
#include <mpi.h>
#include "sorting_data.h"

#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
                  (double)ts.tv_nsec * 1e-9)

struct timespec ts;

/* each kdtree point is an array of k values */
typedef float_t kpoint[NDIM];

/* 
    structure for the nodes in the kdtree, accounting for: 
    - the axis on which we are splitting data
    - the splitting element (data point)
    - pointers to left and right subtrees
*/
struct knode {
	int axis;					
	float_t* split_point;		
	struct knode *left, *right; 
};

/*
	---------------
	Auxiliary functions
	--------------- 
*/

/* This function print the kdtree recursively using a depth-first strategy */
void print_kdtree(struct knode* node, int level, int nprocs, int rank);

void print_proc_subtree(struct knode* node, int level, FILE *fp);

/**
    The following function chooses the next split dimension,
    it does this in a round robin fashion, but it takes into account
    the extension of data in each direction 
*/

int choose_split_dim(data_t *points, int ndim, int axis);


/*
	---------------
	Build up kdtree
	--------------- 
*/

/**
 The following function builds up the kdtree recursively
	- input:
		- k-dimensional data points we are interested in
		- number of points in the considered subset
		- splitting dimension of the previous call
		- current level in the final kdtree
	- output:
		- pointer to the root of the tree (/subtree)
*/
struct knode* build_kdtree(data_t *points, int n, int axis, int level);

/* recursively free kdtree */
void free_tree(struct knode* root);

