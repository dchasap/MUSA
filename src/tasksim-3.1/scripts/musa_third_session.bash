#!/bin/bash


function copy_musa_start_files() {
  APP=$1
  ARGS=$2
  NRANKS=$3
  CORELIST=$4
  cp ${MUSA_HOME}/tasksim/etc/conf/mn4_musa.conf .
  cp ${MUSA_HOME}/tasksim/etc/conf/arm_low_power.conf .
  cp ${MUSA_HOME}/tasksim/etc/conf/arm_high_end.conf .
  cp $basedir/step1_tracing/dark2.pal .
  cp $basedir/step1_tracing/paraver_filter.cfg .
  #cp $basedir/step1_tracing/speedup.plot .
  cp $basedir/step1_tracing/comparison_hpccg.plot .

  cp $basedir/step1_tracing/generate_hpccg_comparison.bash ./generate_hpcg_comparison.bash
  chmod 755 generate_hpcg_comparison.bash
  cp $basedir/step1_tracing/create_sim_environment.bash .
  chmod 755 create_sim_environment.bash

}

basedir=${MUSA_HOME}/tasksim/etc/examples

copy_musa_start_files HPCG_Dibona_nosve "96 96 96" 4 "1 2 4 8 16 32"


cp $MUSA_TUTORIAL_FILES/session3/memory_trace/TRACE_HPCG_Dibona_nosve_000004_MEMO/ . -R
echo "HPCG trace copied form the second tutorial session"
#cp ${MUSA_HOME}/app_examples/hpccg-mpi . -R
#ln -s hpccg-mpi/HPCCG .
#ln -s hpccg-mpi/HPCCG_instr .
