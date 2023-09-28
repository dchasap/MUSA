#!/bin/bash
#BSUB -q bsc_debug
#BSUB -n 4
#BSUB -oo log_APP_%J.out
#BSUB -eo log_APP_%J.err
#BSUB -R "span[ptile=2]"
#BSUB -x
#BSUB -J APP
#BSUB -W 00:10

module purge
module load musa
#======== User defined parameters ===========================================

# Application binary name
APPNAME=bt-mz.A.4
# Application arguments
ARGS=
# Number of MPI ranks (should be the same as SLURM's --ntasks)
RANKS=4


# Enable/disable Memory Tracing (default 0)
export MEMORY_MODE=0

# Filter for phases. Only MPI phases with OmpSs code [INIT, INIT+NUM) will 
# be traced in memory mode
export TSMPI_MEM_PHASES_INIT=4
export TSMPI_MEM_PHASES_NUM=1
# Filter for ranks. Only ranks [INIT, INIT+NUM) will be traced in memory mode
export TSMPI_RANK_INIT=1
export TSMPI_RANK_NUM=1
# In memory mode traces contain the executed basic blocs and the memory
# addresses accessed.

# Optional parameters:

# Name of the TaskSim configuration file to be used on simulations: 
export TSMPI_SIM_CONFIG=marenostrum_configuration.conf
# List of cores per node to be simulated (Later, at the moment we only create the folder structure).
export TSMPI_SIM_CORES="1 2 4 8 16"

# ===============================================

launch_musa_tracer.bash $RANKS $APPNAME $ARGS
