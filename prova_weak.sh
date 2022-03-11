#!/bin/bash

#store times
times=[]

printf '%s,%s,%s,%s\n' 'n_procs' 'n_threads' 'time_taken' 'problem size' > results/weak_scaling.csv


# establish the size of the problem 
s=10000

# use just 1 mpi process and change the
# number of threads to be used
t=10

export OMP_PLACES=cores


# keeping fixed the number of mpi processes

let n=2

for j in  $( seq 1 $t )
do  
    let ps=($j*$s*2)
    export OMP_NUM_THREADS=$j
    echo ${ps} ${j} ${n}
    mpirun --mca btl ^openib -np $n --map-by socket ./omp_kdtree $ps > time_taken.txt
 
    times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

    printf '%s,%s,%s,%s\n' ${n} ${j} ${times[i]} ${ps} >> results/weak_scaling.csv 

done

# keeping fixed the number of threads 
let t=2
export OMP_NUM_THREADS=2

let p=3

for i in  $( seq 0 $p )
do  
    let n=(2**$i)
    let ps=($n*$s*2)
    echo ${ps} ${t} ${n}
    mpirun --mca btl ^openib -np $n --map-by socket ./omp_kdtree $ps > time_taken.txt
 
    times[i]=$(cat time_taken.txt | cut -f2 -d ':')  

    printf '%s,%s,%s,%s\n' ${n} ${t} ${times[i]} ${ps} >> results/weak_scaling.csv 

done


