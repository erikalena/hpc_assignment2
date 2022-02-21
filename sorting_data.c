#include "sorting_data.h"

#define SWAP(A,B,SIZE) do {int sz = (SIZE); char *a = (A); char *b = (B); \
    do { char _temp = *a;*a++ = *b;*b++ = _temp;} while (--sz);} while (0)

int sorting(data_t* data, int npoints, int axis) {

  #if defined(DEBUG)
    int nthreads = omp_get_num_threads();
    printf("%d threads working, start partitioning the array..\n", nthreads);
  #endif

  
  int start = 0, end = npoints, dim = axis;
  
  // pick up the closest to the middle as pivot 
  
  int median = find_median(data, start, end, dim);
 
  //put pivot in last position
  SWAP( (void*)&data[median], (void*)&data[end-1], sizeof(data_t) ); 
  int pivot = end-1;
  
  int pointbreak = end-1;
  for ( int i = start; i <= pointbreak; i++ )
    if( data[i].data[dim] >= data[pivot].data[dim] ) {
	    while( (pointbreak > i) && data[i].data[dim] >= data[pointbreak].data[dim] ) 
	        pointbreak--;
	    if (pointbreak > i ) 
	        SWAP( (void*)&data[i], (void*)&data[pointbreak--], sizeof(data_t) );
    }  
    
  pointbreak += !(data[pointbreak].data[dim] >= data[pivot].data[dim]) ;
  SWAP( (void*)&data[pointbreak], (void*)&data[pivot], sizeof(data_t) ); 
  

  return pointbreak;
}

int find_median(data_t *data, int start, int end, int dim) {
    // find max and min
    float_t min = MAX_VALUE, max = MIN_VALUE;
    float_t median;
    int pivot = 0;
    
    for(int i = start; i < end; i++) {
        max = data[i].data[dim] > max ? data[i].data[dim] : max;
        min = data[i].data[dim] < min ? data[i].data[dim] : min;
    }
   
    // find element which is closest to the median
    median = (max - min)/2 + min;
    min = MAX_VALUE;
  
    for(int i = start; i < end; i++) {
       pivot = (abs(data[i].data[dim] - median) <= abs(data[pivot].data[dim] - median)) ? i : pivot;
    }

    return pivot; 
}



int verify_partitioning( data_t *data, int start, int end, int mid, int dim) {
  int failure = 0;
  int fail = 0;
  
  for( int i = start; i < mid; i++ )
    if ( compare( (void*)&data[i], (void*)&data[mid], dim ) >= 0 )
      fail++;

  failure += fail;
  if ( fail )
    { 
      printf("failure in first half\n");
      fail = 0;
    }

  for( int i = mid+1; i < end; i++ )
    if ( compare( (void*)&data[i], (void*)&data[mid], dim ) < 0 )
      fail++;

  failure += fail;
  if ( fail )
    printf("failure in second half\n");

  return failure;
}


int show_array( data_t *data, int start, int end, int dim, int not_used ) {
  for ( int i = start; i < end; i++ )
    printf( "%f ", data[i].data[dim] );
  printf("\n");
  return 0;
}


int compare( const void *A, const void *B, int dim ) {
  data_t *a = (data_t*)A;
  data_t *b = (data_t*)B;

  double diff = a->data[dim] - b->data[dim];
  return ( (diff > 0) - (diff < 0) );
}


