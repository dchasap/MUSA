#!/bin/bash

MODE=presim
TRACE=trace_ts
APP=APPNAME
PRESIM=PRESIMNAME
CONFIG=CONFIGFILE
CORE=CORECOUNT
PADCORE=`printf "%0*d" 6 $CORE`
PRV_TRACE=trace_SIMULATED/MUSA_${APP}_${PADCORE}cores_${CONFIG}_${MODE}.prv
if [ ! -e $PRESIM ]; then
  echo "[ERROR] Data file $PRESIM not found."
  exit
fi

export TS_SIM_MEM=0
export TS_SIM_PRV=0
export TS_RANGE_INI=0
export TS_RANGE_FIN=100000
export TS_MEM_INI=0
export TS_MEM_FIN=10000000

Dimemas   -tasksim          $MODE   \
          -ts_trace_folder  $TRACE  \
          -ts_trace_name    $APP    \
          -ts_presim_file   $PRESIM \
          \
          -S                32K     \
          -pa               ${PRV_TRACE} \
          config_NET.cfg &> integration_log_${APP}_${CONFIG}_${MODE}_${PADCORE}.log

if [ -e $PRV_TRACE ]; then
  echo "Integrated trace $PRV_TRACE generated."
  exit 0
else
  echo "ERROR: Integrated trace was not generated"
  exit -1
fi



