#!/bin/bash

#store times
times=[]

printf '%s,%s,%s,\n' 'n_procs' 'n_threads' 'time_taken (us)' > performances.csv

# establish the maximum number of processors on which you want to test the code
# provide it as the maximum p s.t. 2^p = n. of processors
p=3

# set number of threads o be used
n_procs=16
export OMP_PLACES=cores

for i in  $( seq 0 $p )
do
    let n=(2**$i)
    let t=($n_procs/$n)
    for j in  $( seq 1 $t )
    do  
        export OMP_NUM_THREADS=$j
        mpirun -np $n ./kdtree > time_taken.txt
     
        times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

        printf '%s,%s,%s,\n' ${n} ${j} ${times[i]} >> performances.csv 
    done
    echo $n $t
done


