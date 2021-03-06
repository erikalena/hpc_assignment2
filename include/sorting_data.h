#pragma once
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>


#if !defined(DOUBLE_PRECISION)
#define float_t float
#else
#define float_t double
#endif

#if !defined(DOUBLE_PRECISION)
#define mpif_t MPI_FLOAT
#else
#define mpif_t MPI_DOUBLE
#endif

#define NDIM 2

#define NPOINTS 100000
#define MAX_VALUE 100000
#define MIN_VALUE 0

typedef struct
{
  float_t data[NDIM];
} data_t;


typedef int (compare_t)(const void*, const void*, int);
typedef int (verify_t)(data_t *, int, int, int, int);
compare_t compare;
verify_t  verify_partitioning;

/** 
    The following function move data points with respect to a given pivot, 
    such that all the elements < pivot are on left side and all the others are 
    on right side. 
*/
int partitioning(data_t *data, int length, int axis, int parallel);

/** 
    Mid-point is found for partitioning the data structure.
    Both functions can take an additional parameter which can be
    used to specify if we want to parallelize the search of the median
    using openmp 
*/
int find_median(data_t *data, int start, int end, int dim, int parallel);

void swap(data_t* a, data_t* b);
