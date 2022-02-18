#include "sorting_data.h"

int SORT_DIM = 0;

int sorting(data_t* data, int npoints, int axis) {
    
  //process and sort data with respect to the given axis
  SORT_DIM = axis;
  
  struct timespec ts;
  int    nthreads = 1;
  double tstart = CPU_TIME;
  int mid;
 #if defined(_OPENMP)
 {
      nthreads = omp_get_num_threads();
	  printf("%d threads working, start partitioning the array..\n", nthreads);
      mid = partitioning( data, 0, npoints, compare_ge);
    
  } 
 #else

  mid = partitioning( data, 0, npoints, compare_ge);
 #endif
  
  double tend = CPU_TIME;  
  
  return mid;
}

int find_median(data_t *data, int start, int end) {
    float_t min = 100;
    float_t max = 0;
    for ( int i = start; i <= end-1; i++ ) {
        if(data[i].data[SORT_DIM] < min) 
            min = data[i].data[SORT_DIM];
        if(data[i].data[SORT_DIM] > max) 
            max = data[i].data[SORT_DIM];
    }
    float_t median = (max - min)/2;
    int pivot = 0;
    for ( int i = start; i <= end-1; i++ ) {
        if(abs(data[i].data[SORT_DIM] - median) < abs(data[pivot].data[SORT_DIM] - median)) 
            pivot = i;
    }
    return pivot; 

}

#define SWAP(A,B,SIZE) do {int sz = (SIZE); char *a = (A); char *b = (B); \
    do { char _temp = *a;*a++ = *b;*b++ = _temp;} while (--sz);} while (0)

inline int partitioning( data_t *data, int start, int end, compare_t cmp_ge )
{
	
	
  // pick up the closest to the middle as pivot 
  int median = find_median(data, start, end);
  
  //put pivot in last position
  SWAP( (void*)&data[median], (void*)&data[end-1], sizeof(data_t) ); 
  void *pivot = (void*)&data[end-1];
  
  int pointbreak = end-1;
  for ( int i = start; i <= pointbreak; i++ )
    if( cmp_ge( (void*)&data[i], pivot ) ) {
	    while( (pointbreak > i) && cmp_ge( (void*)&data[pointbreak], pivot ) ) 
	        pointbreak--;
	    if (pointbreak > i ) 
	        SWAP( (void*)&data[i], (void*)&data[pointbreak--], sizeof(data_t) );
    }  
    
  pointbreak += !cmp_ge( (void*)&data[pointbreak], pivot ) ;
  SWAP( (void*)&data[pointbreak], pivot, sizeof(data_t) ); 
  
	#if defined(DEBUG)
	#define CHECK {							\
	if ( verify_partitioning( data, start, end, mid ) ) {		\
		printf( "partitioning is wrong\n");				\
		printf("%4d, %4d (%4d, %g) -> %4d, %4d  +  %4d, %4d\n",		\
		 start, end, mid, data[mid].data[SORT_DIM],start, mid, mid+1, end); \
		show_array( data, start, end, 0 ); }}
	#else
	#define CHECK
	#endif
	int mid = pointbreak;
  CHECK;

  return pointbreak;
}




int verify_partitioning( data_t *data, int start, int end, int mid ) {
  int failure = 0;
  int fail = 0;
  
  for( int i = start; i < mid; i++ )
    if ( compare( (void*)&data[i], (void*)&data[mid] ) >= 0 )
      fail++;

  failure += fail;
  if ( fail )
    { 
      printf("failure in first half\n");
      fail = 0;
    }

  for( int i = mid+1; i < end; i++ )
    if ( compare( (void*)&data[i], (void*)&data[mid] ) < 0 )
      fail++;

  failure += fail;
  if ( fail )
    printf("failure in second half\n");

  return failure;
}


int show_array( data_t *data, int start, int end, int not_used ) {
  for ( int i = start; i < end; i++ )
    printf( "%f ", data[i].data[SORT_DIM] );
  printf("\n");
  return 0;
}


inline int compare( const void *A, const void *B ) {
  data_t *a = (data_t*)A;
  data_t *b = (data_t*)B;

  double diff = a->data[SORT_DIM] - b->data[SORT_DIM];
  return ( (diff > 0) - (diff < 0) );
}

inline int compare_ge( const void *A, const void *B ) {
  data_t *a = (data_t*)A;
  data_t *b = (data_t*)B;

  return (a->data[SORT_DIM] >= b->data[SORT_DIM]);
}
