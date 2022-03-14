#!/bin/bash

#PBS -l nodes=1:ppn=24
#PBS -l walltime=02:00:00
#PBS -q dssc

cd $PBS_O_WORKDIR

#load openmpi module
module load openmpi-4.1.1+gnu-9.3.0


#store times
times=[]
mpi_times=[]
omp_times=[]

printf '%s,%s,%s,%s,%s,%s\n' 'n_procs' 'n_threads' 'time_taken' 'mpi_time' 'omp_time' 'problem_size' > results/weak_scaling_cpu_bycore.csv

# establish the maximum number of processors on which you want to test the code
# provide it as the maximum p s.t. 2^p = n. of processors
p=4

export OMP_PLACES=cores
# set a maximum number of threads to be used
t=24

# set problem size 
s=1000000

for i in  $( seq 0 $p )
do
    let n=(2**$i)
    for j in  $(seq 1 $t )
    do  
        export OMP_NUM_THREADS=$j
	let ps=($n*$s*$j)
        mpirun --mca btl ^openib -np $n --map-by core ./omp_kdtree ${ps} > time_taken.txt
     	
	mpi_times[j]=$(cat time_taken.txt | grep send | cut -f2 -d ':')
	omp_times[j]=$(cat time_taken.txt | grep subtree | cut -f2 -d ':')
        times[j]=$(cat time_taken.txt | grep kdtree | cut -f2 -d ':')  

        printf '%s,%s,%s,%s,%s,%s\n' ${n} ${j} ${times[j]} ${mpi_times[j]} ${omp_times[j]} ${ps} >> results/weak_scaling_cpu_bycore.csv 
    done
done


exit
