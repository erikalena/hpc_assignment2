#!/bin/bash

#PBS -l nodes=1:ppn=24
#PBS -l walltime=02:00:00
#PBS -q dssc_gpu

cd $PBS_O_WORKDIR

#load openmpi module
module load openmpi-4.1.1+gnu-9.3.0


#store times
times=[]

printf '%s,%s,%s,\n' 'n_procs' 'n_threads' 'time_taken' > results/weak_scaling.csv


# establish the size of the problem for each single thread
s=100000

# use just 1 mpi process and change the
# number of threads to be used
p=0
t=10

export OMP_PLACES=cores

let n=(2**$p)

for j in  $( seq 1 $t )
do  
    let ps=($j*$s)
    echo ${ps}
    export OMP_NUM_THREADS=$j
    mpirun --mca btl ^openib -np $n --map-by socket ./kdtree $ps > time_taken.txt
 
    times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

    printf '%s,%s,%s,\n' ${n} ${j} ${times[i]} >> results/weak_scaling.csv 

done

exit