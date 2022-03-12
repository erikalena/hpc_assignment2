#!/bin/bash

#store times
times=[]

printf '%s,%s,%s,%s\n' 'n_procs' 'n_threads' 'time_taken (s)' 'problem sie' > performances.csv

# establish the maximum number of processors on which you want to test the code
# provide it as the maximum p s.t. 2^p = n. of processors
p=3


export OMP_PLACES=cores
# set a maximum number of threads to be used
t=6

# set problem size 
ps=100000

for i in  $( seq 0 $p )
do
    let n=(2**$i)
    for j in  $(seq 1 2 $t )
    do  
        export OMP_NUM_THREADS=$j
        mpirun --mca btl ^openib -np $n --map-by socket ./omp_kdtree ${ps} > time_taken.txt
     
        times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

        printf '%s,%s,%s,%s\n' ${n} ${j} ${times[i]} ${ps} >> results/performances.csv 
    done
done


