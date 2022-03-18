#include "sorting_data.h"


int partitioning(data_t* data, int npoints, int axis, int parallel) {

    int start = 0, end = npoints, dim = axis;

    // pick up the closest to the median as pivot 
    int median;
    if(!parallel)
        median = find_median(data, start, end, dim);
    else 
        median = find_median_parallel(data, start, end, dim);
        
    //put pivot in last position
    swap((void*)&data[median], (void*)&data[end-1]);
    int pivot = end-1;

    int idx = -1;
    for(int j = start; j < end-1; j++) {
        if( data[j].data[dim] <= data[pivot].data[dim] ) {
             idx++;
             swap((void*)&data[j], (void*)&data[idx]);
        }
    }

    swap((void*)&data[pivot], (void*)&data[idx+1]);
    
    #if defined(DEBUG)
    #define CHECK {							\
        if (verify_partitioning( data, start, end, idx+1, dim ) ) {		\
          printf( "partitioning is wrong\n");				\
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

    for(int i = start; i < end; i++) 
       pivot = (abs(data[i].data[dim] - median) < abs(data[pivot].data[dim] - median)) ? i : pivot;
  
    return pivot; 
}

int find_median_parallel(data_t *data, int start, int end, int dim) {
  
    float_t min = MAX_VALUE, max = MIN_VALUE;

    #if defined(_OPENMP) 
    #pragma omp parallel
    {
        #pragma omp for reduction(max:max) reduction(min:min)
            for(int i = start; i < end; i++) {
                max = data[i].data[dim] > max ? data[i].data[dim] : max;
                min = data[i].data[dim] < min ? data[i].data[dim] : min;
            }
    }
    #endif
    // find element which is closest to the median
    float_t median = (max - min)/2 + min;

    int index = 0;
    min = MAX_VALUE;
   
    #if defined(_OPENMP)
    #pragma omp parallel shared(index, median)
    {
        int index_local = 0;
        double min_local = MAX_VALUE;  
        #pragma omp for 
        for (int i = start; i < end; i++) {        
            if (fabs(data[i].data[dim]-median) <= min_local) {
                min_local = fabs(data[i].data[dim]-median);
                index_local = i;
            }
        }
        #pragma omp critical 
        {
            if (min_local < min) {
                min = min_local;
                index = index_local;
            }
        }
    }
    #endif
    
    return index; 
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
    if (compare( (void*)&data[i], (void*)&data[mid], dim ) < 0)
      fail++;

    failure += fail;
    if (fail)
    printf("failure in second half\n");

    return failure;
}


int compare( const void *A, const void *B, int dim ) {
    data_t *a = (data_t*)A;
    data_t *b = (data_t*)B;

    double diff = a->data[dim] - b->data[dim];
    return ( (diff > 0) - (diff < 0) );
}

void swap(data_t* a, data_t* b) {
    data_t tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
} 

