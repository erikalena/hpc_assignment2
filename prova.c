#include <quicksort.h>

int main(int argc, char** argv) {
    
     
    int npoints = 10, subtree_size = 0;
    data_t *data;
    
     // otherwise generate points randomly
    data = (data_t*)malloc(npoints*sizeof(data_t));
    
    for ( int i = 0; i < npoints; i++ )
    for (int j = 0; j < NDIM; j++)
        data[i].data[j] = drand48()*100;
    
    for ( int i = 0; i < npoints; i++ )
        printf("(%.2f, %.2f)", data[i].data[0], data[i].data[1]); 
    
    printf("\n");
    sorting(data, npoints, 0); 

    for ( int i = 0; i < npoints; i++ )
        printf("(%.2f, %.2f)", data[i].data[0], data[i].data[1]); 
    
    sorting(data, npoints, 1); 

    for ( int i = 0; i < npoints; i++ )
        printf("(%.2f, %.2f)", data[i].data[0], data[i].data[1]); 
 
    return EXIT_SUCCESS;
}
