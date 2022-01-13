#pragma once

#include <stdlib.h>
#include "quicksort.h"

#define NPOINTS 10
#define MAX_VALUE 100

#define MAX_LEVEL 100 //! arbitrarily defined

/* each kdtree point is an array of k values */
typedef float_t kpoint[NDIM];

/* structure for the nodes in the kdtree */
struct knode {
	int axis;					//on which axis we are splitting data
	float_t* split_point;		//which is the splitting element 
	struct knode *left, *right; //pointers to left and right subtrees
};

/* This function print the kdtree horizontally */
void print_kdtree(struct knode* node, int level);

/* The following function chooses the next split dimension,
it does this in a round robin fashion, but it takes into account
the extension of data in each direction */
int choose_split_dim(data_t *points, int ndim, int axis);

/*
 The following function builds up the kdtree recursively
	- input:
		- k-dimensional data points we are intersted in
		- number of points in the considered subset
		- splitting dimension of the previous call
	- output:
		- pointer to the root of the tree (/subtree)
*/
struct knode* build_kdtree(data_t *points, int n, int axis, int level);


