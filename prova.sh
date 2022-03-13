#!/bin/bash

#store times
times=[]
mpi_times=[]
omp_times=[]

printf '%s,%s,%s,%s,%s,%s\n' 'n_procs' 'n_threads' 'time_taken (s)' 'mpi_time' 'omp_times' 'problem size' > results/performances.csv

# establish the maximum number of processors on which you want to test the code
# provide it as the maximum p s.t. 2^p = n. of processors
p=3


export OMP_PLACES=cores
# set a maximum number of threads to be used
t=6

# set problem size 
ps=1000000

for i in  $( seq 0 $p )
do
    let n=(2**$i)
    for j in  $(seq 1 2 $t )
    do  
        export OMP_NUM_THREADS=$j
        mpirun --mca btl ^openib -np $n --map-by socket ./omp_kdtree ${ps} > time_taken.txt
        
        mpi_times[j]=$(cat time_taken.txt | grep send | cut -f2 -d ':') 
        omp_times[j]=$(cat time_taken.txt | grep subtree | cut -f2 -d ':') 
        times[j]=$(cat time_taken.txt | grep kdtree | cut -f2 -d ':')  

        printf '%s,%s,%s,%s,%s,%s\n' ${n} ${j} ${times[j]} ${mpi_times[j]} ${omp_times[j]} ${ps} >> results/performances.csv 
    done
done


