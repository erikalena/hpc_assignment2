#!/bin/bash

#PBS -l nodes=2:ppn=8
#PBS -l walltime=00:30:00
#PBS -q dssc

cd $PBS_O_WORKDIR

#load openmpi module
module load openmpi-4.1.1+gnu-9.3.0

#run kdtree
mpirun --mca btl ^openib -np 8 kdtree

exit
