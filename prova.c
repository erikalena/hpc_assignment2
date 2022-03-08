#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

int main(int argc, char** argv) {
    
    int buf = 100;
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    
    if(world_rank == 0) {
        buf = 777;
    }
    
    MPI_Bcast(&buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("I'm process %d and I received %d from root process\n", world_rank, buf);
    #if defined(_OPENMP)
     
        #pragma omp parallel
        {
            #pragma omp single 
            {
                #pragma omp task
                printf("I'm thread %d from task 0\n", omp_get_thread_num());
                
                #pragma omp task
                printf("I'm thread %d from task 1\n", omp_get_thread_num());
                
                #pragma omp task
                printf("I'm thread %d from task 2\n", omp_get_thread_num());
             }
         }  
         
         MPI_Barrier( MPI_COMM_WORLD);  
    #endif

    MPI_Finalize();
    return 0;
}
