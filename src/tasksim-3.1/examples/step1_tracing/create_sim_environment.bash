#!/bin/bash

module load musa

# Application binary name
APPNAME=HPCG_Dibona_nosve
# Application arguments
ARGS="96 96 96"
# Number of MPI ranks (should be the same as --ntasks)
RANKS=4
# Enable/disable Memory Tracing (default 0)
export MEMORY_MODE=1
# Name of the TaskSim configuration file to be used on simulations: 
export TSMPI_SIM_CONFIG="mn4_musa.conf arm_low_power.conf arm_high_end.conf"
# List of cores per node to be simulated (Later, at the moment we only create the folder structure).
export TSMPI_SIM_CORES="1 2 4 8 16 32"


# End of configurable section.

PRCAD=$(printf "%0*d" 6 $RANKS)
if [ $MEMORY_MODE -eq 0 ]; then
  MODECAD="BRST"
else 
  MODECAD="MEMO"
fi
export MUSA_EXP_NAME=TRACE_${APPNAME}_${PRCAD}_${MODECAD}
export PR=$RANKS


create_musa_simulation_env.bash $RANKS $APPNAME $MUSA_EXP_NAME $ARGS

# Dibona tutorial code to copy third tutorial session results:
#results=$MUSA_TUTORIAL_FILES/session3/simulation_results
#destination=./TRACE_${APPNAME}_000004_MEMO/SIMULATION/A1_PRESIM
#
#for core in $TSMPI_SIM_CORES; do
#    for config in $TSMPI_SIM_CONFIG; do
#        padcore=$(printf "%0*d" 6 $core)
#        c=${config::-5}
#        mkdir -p $destination/musa_out_HPCG_Dibona_nosve_${padcore}/${c}_${padcore}_BRST
#        cp $results/musa_out_HPCG_Dibona_nosve_${padcore}/${c}_${padcore}_BRST/${c}_${padcore}_BRST.dat $destination/musa_out_HPCG_Dibona_nosve_${padcore}/${c}_${padcore}_BRST
#    done
#done
#sed  -i "s/third_session=0/third_session=1/g" ./TRACE_${APPNAME}_000004_MEMO/SIMULATION/A1_PRESIM/generate_musa_presim.bash
