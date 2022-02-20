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

    #if defined(_OPENMP)
        #pragma omp parallel
            MPI_Bcast(&buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
            printf("I'm process %d and I received %d from root process\n", world_rank, buf);
    #endif
        return EXIT_SUCCESS;

    MPI_Finalize();
    return 0;
}
