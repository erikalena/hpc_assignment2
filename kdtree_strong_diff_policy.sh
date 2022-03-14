#!/bin/bash

#PBS -l nodes=1:ppn=48
#PBS -l walltime=02:00:00
#PBS -q dssc_gpu

cd $PBS_O_WORKDIR

#load openmpi module
module load openmpi-4.1.1+gnu-9.3.0


#store times
times=[]

printf '%s,%s,%s,%s\n' 'n_procs' 'n_threads' 'time_taken' 'problem_size' > results/strong_scaling_touch_by_all.csv

# establish the maximum number of processors on which you want to test the code
# provide it as the maximum p s.t. 2^p = n. of processors
p=3

export OMP_PLACES=cores
# set a maximum number of threads to be used
t=30

# set problem size 
ps=10000000

for i in  $( seq 0 $p )
do
    let n=(2**$i)
    for j in  $(seq 1 $t )
    do  
        export OMP_NUM_THREADS=$j
        mpirun --mca btl ^openib -np $n --map-by socket ./omp_kdtree ${ps} > time_taken.txt
     
        times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

        printf '%s,%s,%s,%s\n' ${n} ${j} ${times[i]} ${ps} >> results/strong_scaling_touch_by_all.csv 
    done
done


exit
