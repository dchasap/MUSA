#!/bin/bash
#SBATCH --job-name="Musa"
#SBATCH -D .
#SBATCH --output=./logs_musa_generation-%j.out
#SBATCH --error=./logs_musa_generation-%j.err
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=48
#SBATCH --tasks-per-node=1
#SBATCH --time=0:15:00
#SBATCH --qos=debug
#SBATCH --cpu-freq=2100000

module purge
module load musa/clang/3.0
#======== User defined parameters ===========================================

# Application binary name
APPNAME=bt-mz.A.4
# Application arguments
ARGS=
# Number of MPI ranks (should be the same as --ntasks)
RANKS=4


#======== Tracing and filtering parameters ==================================

# Enable/disable Memory Tracing (default 0)
export MEMORY_MODE=1

# Filter for phases. Only MPI phases with OmpSs code [INIT, INIT+NUM) will 
# be traced in memory mode
export TSMPI_MEM_PHASES_INIT=1
export TSMPI_MEM_PHASES_NUM=9999
# Filter for ranks. Only ranks [INIT, INIT+NUM) will be traced in memory mode
export TSMPI_RANK_INIT=1
export TSMPI_RANK_NUM=4
# In memory mode traces contain the executed basic blocs and the memory
# addresses accessed.

# Name of the TaskSim configuration file to be used on simulations: 
export TSMPI_SIM_CONFIG=marenostrum_configuration.conf
# List of cores per node to be simulated (Later, at the moment we only create the folder structure).
export TSMPI_SIM_CORES="1 2 4 8 16"

# ===============================================
vec="128 256 512 1024 2048"
for v in $vec; do
    mkdir $v
    cd $v
    cp ../$TSMPI_SIM_CONFIG .
    ln -s ../${APPNAME} .
    ln -s ../${APPNAME}_instr .
    export VECTOR_LENGTH=$v
    launch_musa_tracer_sve.bash $RANKS $APPNAME $ARGS
    cd ../
done
launch_musa_tracer_sve.bash $RANKS $APPNAME $ARGS

cp /apps/BSCTOOLS/wxparaver/latest/cfgs/mpi/views/MPI_call.cfg ~/
