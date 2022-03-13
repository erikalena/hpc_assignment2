#include "kdtree.h"

#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
                  (double)ts.tv_nsec * 1e-9)
                  
struct timespec ts;
/*
	---------------
	Auxiliary functions
	--------------- 
*/

/* Function for creating an MPI datatype which will be useful
    to exchange data between processes */
void build_mpi_point_type();

/* Simple function for counting file lines that
    is the number of points provided */
int countlines(char* file);

/* load dataset from file */
void load_dataset(data_t *data, char* file, int npoints);

/* This function determines the initial partion and
    assigns to each process a suitable subset of points */
struct knode* first_ksplit(data_t *points, int n, int axis, int level, int nprocs, int rank);

/* This function send to each process the data structure containing the points
    it will use to buils its subtree. The level at which the subtree will be placed
    is communicated as well. */
void send_subset(data_t *points, int size, int level, int receiver);
