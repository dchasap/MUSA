#!/bin/bash

MODE=presim
TRACE=../trace_ts
APP=$3
PRESIM=$1
TRACENAME=$2


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
          -S                32K     \
          -pa               ${TRACENAME} \
          ../config_NET_sc16.cfg



