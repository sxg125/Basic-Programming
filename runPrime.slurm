#!/bin/bash
#SBATCH -J Benchmarks
#SBATCH --time=24:00:00
#SBATCH -N 1
#SBATCH -c 4
##SBATCH -n 4
#SBATCH --mem=5g
#SBATCH -p gpuk40 --gres=gpu:1


echo "The job is running in $SLURM_NODELIST"

NPROCS=$(( $SLURM_NNODES * $SLURM_CPUS_PER_TASK )) 
#Copy the script and other input files to the scratch directory and change directory
cp primeNumbers.c primeNumbers.m primeNumbers.sh primeNumbers.py primeNumbersPar.m primeNumbersOmp.c primeNumbersMpi.c primeNumbersAcc.c $PFSDIR
cd $PFSDIR

# Compile C program
gcc -o prime primeNumbers.c

# Compile C program with OpenMP
gcc -o primeOmp -fopenmp primeNumbersOmp.c

# Compile C program with MPI
mpicc -o primeMpi primeNumbersMpi.c

# Compiling C Program with OpenACC
module load pgi
pgcc -ta=nvidia -acc -Minfo=accel -o primeAcc primeNumbersAcc.c

#Execute 

echo "running serial ..."
time ./prime

echo "running Python ..."
python primeNumbers.py

echo "running parallel OpenMP ..."

export OMP_NUM_THREAD=$NPROCS
echo "Number of Threads = $NPROCS"
time ./primeOmp

echo "running parallel MPI"
mpirun ./primeMpi

echo "running in GPUs using OpenACC ..."
time ./primeAcc

#MATLAB 
#Load MATLAB module
module load matlab

#MATLAB Preference Setting
matlab_prefdir="/tmp/$USER/matlab/`hostname`_PID$$"
test -d $matlab_prefdir || mkdir -p $matlab_prefdir
export MATLAB_PREFDIR="$matlab_prefdir"

#RUN MATLAB script
echo "Running Matlab Serial ..."
matlab -singleCompThread -nodisplay -r 'primeNumbers(2,131072)'

echo "Running Matlab Parallel parfor ..."
matlab -singleCompThread -nodisplay -r 'primeNumbersPar(2,131072)' 
# quit
echo "running Bash ..."
time ./primeNumbers.sh

cp -r * $SLURM_SUBMIT_DIR

