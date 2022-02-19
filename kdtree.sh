#!/bin/bash

#PBS -l nodes=1:ppn=24
#PBS -l walltime=00:30:00
#PBS -q dssc_gpu

cd $PBS_O_WORKDIR

#load openmpi module
module load openmpi-4.1.1+gnu-9.3.0


#store times
times=[]

printf '%s,%s,%s,\n' 'n_procs' 'n_threads' 'time_taken (us)' > performances.csv

# establish the maximum number of processors on which you want to test the code
# provide it as the maximum p s.t. 2^p = n. of processors
p=4

n_procs=24
export OMP_PLACES=cores


for i in  $( seq 0 $p )
do
    let n=(2**$i)
    let t=($n_procs/$n)
    for j in  $( seq 1 $t )
    do  
        export OMP_NUM_THREADS=$j
        mpirun --mca btl ^openib -np $n --map-by socket ./kdtree > time_taken.txt
     
        times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

        printf '%s,%s,%s,\n' ${n} ${j} ${times[i]} >> performances.csv 
    done
done

export OMP_PLACES=cores


exit
