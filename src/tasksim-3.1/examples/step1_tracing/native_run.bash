#!/bin/bash
#SBATCH --job-name="APP-native"
#SBATCH -D .
#SBATCH --output=./log_native_run_%j.out
#SBATCH --error=./log_native_run_%j.err
#SBATCH --ntasks=4
#SBATCH --tasks-per-node=4
#SBATCH --cpus-per-task=4
#SBATCH --time=0:30:00


module load musa/3.1
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NANOX_PERFORMANCE_LIB


APPNAME=./sp-mz.B.4
ARGS=
RANKS=4

mkdir native_logs
CORELIST="1 2 4 8 16 32"

for cpus in $CORELIST; do
  NX_ARGS="--smp-workers=$cpus" mpirun -np $RANKS --report-bindings --map-by node $APPNAME $ARGS &>native_logs/${cpus}th_run.txt
done
