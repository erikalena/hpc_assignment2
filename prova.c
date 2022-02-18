#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    
    
#if defined(_OPENMP)
	#pragma omp parallel
        printf("hello\n"); 
 #endif
    return EXIT_SUCCESS;
}
