#!/bin/bash

#PBS -l nodes=1:ppn=24
#PBS -l walltime=01:00:00
#PBS -q dssc

cd $PBS_O_WORKDIR

#load openmpi module
module load openmpi-4.1.1+gnu-9.3.0


#store times
times=[]

printf '%s,%s,%s,%s\n' 'n_procs' 'n_threads' 'time_taken' 'problem size' > results/weak_scaling_cpu.csv


# establish the size of the problem 
s=1000000

# use just 2 mpi process and change the
# number of threads to be used
t=24

export OMP_PLACES=cores


# keeping fixed the number of mpi processes
let n=2

for j in  $( seq 1 $t )
do  
    let ps=($j*$s*2)
    export OMP_NUM_THREADS=$j
    mpirun --mca btl ^openib -np $n --map-by socket ./omp_kdtree $ps > time_taken.txt
 
    times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

    printf '%s,%s,%s,%s\n' ${n} ${j} ${times[i]} ${ps} >> results/weak_scaling_cpu.csv 

done

# keeping fixed the number of threads 
export OMP_NUM_THREADS=2

#change the number of MPI processes until the following power of 2
let p=4

for i in  $( seq 0 $p )
do  
    let n=(2**$i)
    let ps=($n*$s*2)
    mpirun --mca btl ^openib -np $n --map-by socket ./omp_kdtree $ps > time_taken.txt
 
    times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

    printf '%s,%s,%s,%s\n' ${n} ${j} ${times[i]} ${ps} >> results/weak_scaling_cpu.csv 

done

exit
