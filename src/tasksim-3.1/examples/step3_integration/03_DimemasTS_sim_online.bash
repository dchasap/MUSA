#!/bin/bash

MODE=online
TRACE=trace_ts
APP=bt-mz.A.4.200.iter
CONFIG=tasksim.conf
TMP=trace_01th_burst

mkdir $TMP

export TS_SIM_CORES=1
export TS_SIM_MEM=0
export TS_SIM_PRV=0
#export TS_RANGE_INI=0
#export TS_RANGE_FIN=100000
#export TS_MEM_INI=0
#export TS_MEM_FIN=10000000


Dimemas   -tasksim          $MODE   \
          -ts_trace_folder  $TRACE  \
          -ts_trace_name    $APP    \
          -ts_config        $CONFIG \
          -ts_prv           $TMP    \
          \
          -S                32K     \
          -pa               trace_SIMULATED/MUSA_${APP}_${MODE}.prv \
          config_NET.cfg



