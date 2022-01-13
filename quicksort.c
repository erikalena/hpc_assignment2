#include "quicksort.h"

int SORT_DIM = 0;

void sorting(data_t* data, int npoints, int axis) {
    
  //process and sort data with respect to the given axis
  SORT_DIM = axis;
  
  struct timespec ts;
  int    nthreads = 1;
  double tstart = CPU_TIME;
  
 #if defined(_OPENMP)
 {
      nthreads = omp_get_num_threads();
	  printf("%d threads working, start sorting the array..\n", nthreads);
      pqsort( data, 0, npoints, compare_ge);
    
  } 
 #else

  pqsort( data, 0, npoints, compare_ge);
 #endif
  
  double tend = CPU_TIME;  
  
  //  check sorting

  if ( verify_sorting( data, 0, npoints, 0) ) {
    #if defined(DEBUG)
	    printf("Order verified, the ordered array is:\n[ ");
	    for (int i = 0; i < npoints; i++) {
	        printf("(");
            for (int j = 0; j < NDIM; j++) {
		        printf(j == (NDIM - 1)? "%.2f)," : "%.2f,", data[i].data[j]);
		    }
        } 
        printf("]\n");
        printf("Time taken to order the data %d\t%g sec\n", nthreads, tend-tstart);
    #endif
  }
  else
    fprintf(stderr, "the array is not sorted correctly\n");

}


 #define SWAP(A,B,SIZE) do {int sz = (SIZE); char *a = (A); char *b = (B); \
    do { char _temp = *a;*a++ = *b;*b++ = _temp;} while (--sz);} while (0)

inline int partitioning( data_t *data, int start, int end, compare_t cmp_ge )
{
  
  // pick up the meadian of [0], [mid] and [end] as pivot
  //
  /* to be done */

  // pick up the last element as pivot
  //
  --end;  
  void *pivot = (void*)&data[end];
  
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
  
  return pointbreak;
}


void pqsort( data_t *data, int start, int end, compare_t cmp_ge )
{

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

  int size = end-start;
  if ( size > 2 ) {
      int mid = partitioning( data, start, end, cmp_ge );

      CHECK;
      
     #pragma omp task shared(data) firstprivate(start, mid)
      pqsort( data, start, mid, cmp_ge );
     #pragma omp task shared(data) firstprivate(mid, end)
      pqsort( data, mid+1, end , cmp_ge );
  }
  else {
      if ( (size == 2) && cmp_ge ( (void*)&data[start], (void*)&data[end-1] ) )
	     SWAP( (void*)&data[start], (void*)&data[end-1], sizeof(data_t) );
  }
}




 
int verify_sorting( data_t *data, int start, int end, int not_used ) {
  int i = start;
  while( (++i < end) && (data[i].data[SORT_DIM] >= data[i-1].data[SORT_DIM]) );
  return ( i == end );
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
