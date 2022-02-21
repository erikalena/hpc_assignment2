#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>


#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
                  (double)ts.tv_nsec * 1e-9)


#if defined(DEBUG)
#define VERBOSE
#endif

#if defined(VERBOSE)
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define MAX( a, b ) ( (a)->data[HOT] >(b)->data[HOT]? (a) : (b) );
#define MIN( a, b ) ( (a)->data[HOT] <(b)->data[HOT]? (a) : (b) );

#if !defined(DATA_SIZE)
#define DATA_SIZE 8
#endif
#define HOT       0

#if (!defined(DEBUG) || defined(_OPENMP))
#define N_dflt    100000
#else
#define N_dflt    10
#endif

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
#define X 0
#define Y 1

#define NPOINTS 100000
#define MAX_VALUE 100
#define MIN_VALUE 0

typedef struct
{
  float_t data[NDIM];
} data_t;


typedef int (compare_t)(const void*, const void*, int);
typedef int (verify_t)(data_t *, int, int, int, int);

compare_t compare;
compare_t compare_ge;
verify_t  verify_partitioning;
verify_t  verify_sorting;
verify_t  show_array;

int partitioning( data_t *, int start, int end, int dim, compare_t );

/* the following function calls other routines, in order
to move elements with respect to a given pivot, such that
all the elements < p are on left side and all the others are 
on right side */
int sorting(data_t *, int length, int axis);

int find_median(data_t *data, int start, int end, int dim);
