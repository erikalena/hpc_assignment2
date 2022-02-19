#!/bin/bash

#store times
times=[]

printf '%s,%s,%s,\n' 'n_procs' 'n_threads' 'time_taken (us)' > performances.csv

# establish the maximum number of processors on which you want to test the code
# provide it as the maximum p s.t. 2^p = n. of processors
p=4

# set number of threads o be used
n_threads=2
export OMP_NUM_THREADS=${n_threads}
export OMP_PLACES=cores


for i in  $( seq 0 $p )
do
    let n=(2**$i)
    mpirun -np $n ./kdtree > time_taken.txt
 
    times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

    printf '%s,%s,%s,\n' ${n} ${n_threads} ${times[i]} >> performances.csv 
done




