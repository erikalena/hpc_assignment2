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

    int idx = -1;
    for(int j = start; j < end-1; j++) {
        if( data[j].data[dim] <= data[pivot].data[dim] ) {
             idx++;
             SWAP( (void*)&data[j], (void*)&data[idx], sizeof(data_t) );
        }
    }
    SWAP((void*)&data[pivot], (void*)&data[idx+1], sizeof(data_t) );

    #if defined(DEBUG)
    #define CHECK {							\
        if ( verify_partitioning( data, start, end, idx+1, dim ) ) {		\
          printf( "partitioning is wrong\n");				\
        }\
        else{\
             printf("Everything ok\n");\
        }}
    #else
    #define CHECK
    #endif

    CHECK;
    return idx+1;
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


int compare( const void *A, const void *B, int dim ) {
  data_t *a = (data_t*)A;
  data_t *b = (data_t*)B;

  double diff = a->data[dim] - b->data[dim];
  return ( (diff > 0) - (diff < 0) );
}


