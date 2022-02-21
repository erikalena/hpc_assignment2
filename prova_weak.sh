#!/bin/bash

#store times
times=[]

printf '%s,%s,%s,\n' 'n_procs' 'n_threads' 'time_taken (us)' > weak_scaling.csv

# establish the size of the problem for each single thread
s=100000

# use just 1 mpi process and change the
# number of threads to be used
p=0
t=5

export OMP_PLACES=cores

let n=(2**$p)

for j in  $( seq 1 $t )
do  
    let ps=($j*$s)
    echo ${ps}
    export OMP_NUM_THREADS=$j
    mpirun -np $n ./kdtree $ps > time_taken.txt
 
    times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

    printf '%s,%s,%s,\n' ${n} ${j} ${times[i]} >> weak_scaling.csv 

done


