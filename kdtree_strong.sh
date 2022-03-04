#!/bin/bash

#PBS -l nodes=1:ppn=24
#PBS -l walltime=02:00:00
#PBS -q dssc_gpu

cd $PBS_O_WORKDIR

#load openmpi module
module load openmpi-4.1.1+gnu-9.3.0


#store times
times=[]

printf '%s,%s,%s,\n' 'n_procs' 'n_threads' 'time_taken' > results/strong_scaling.csv

# establish the maximum number of processors on which you want to test the code
# provide it as the maximum p s.t. 2^p = n. of processors
p=1

n_procs=24
export OMP_PLACES=cores
#set a maximum number of threads to be used
t=5

for i in  $( seq 0 $p )
do
    let n=(2**$i)
    for j in  $(seq 1 2 $t )
    do  
        export OMP_NUM_THREADS=$j
        mpirun --mca btl ^openib -np $n --map-by socket ./kdtree 10000000 > time_taken.txt
     
        times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

        printf '%s,%s,%s,\n' ${n} ${j} ${times[i]} >> results/strong_scaling.csv 
    done
done


exit