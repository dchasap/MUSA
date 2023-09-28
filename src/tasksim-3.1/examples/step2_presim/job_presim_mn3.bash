#!/bin/bash
#BSUB -J musa_presim
#BSUB -q bsc_debug
#BSUB -W 0:30
#BSUB -eo log_presim_%J.err
#BSUB -oo log_presim_%J.out
#BSUB -n 16
#BSUB -x
#BSUB -R "span[ptile=8]"

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



