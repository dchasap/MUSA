#!/bin/bash
#SBATCH --job-name="musa-presim"
#SBATCH -D .
#SBATCH --output=./log_presim_%j.out
#SBATCH --error=./log_presim_%j.err
#SBATCH --ntasks=16
#SBATCH --cpus-per-task=8
#SBATCH --time=0:30:00

##############################################################################
## USER DEFINED FLAGS
export TS_SIM_CORES=1
## If enabled(1), simulate memory traced phases in memory mode
export TS_SIM_MEM=0
## If enabled(1), generates a paraver trace for every phase
export TS_SIM_PRV=1
## If enabled(1), keep per phase tasksim output.
## e.g.: To analyze hardware counters of simulations in memory mode.
export TS_SIM_LOG=0
##############################################################################

launch_musa_presim.bash 16 trace_ts tasksim.conf

