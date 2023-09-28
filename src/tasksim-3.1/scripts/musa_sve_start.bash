#!/bin/bash


function copy_musa_start_files() {
  APP=$1
  ARGS=$2
  NRANKS=$3
  CORELIST=$4
  sed -e "s/APPNAME=.*/APPNAME=$APP/g" \
      -e "s#ARGS=.*#ARGS=\"$ARGS\"#g" \
      -e "s/#SBATCH --ntasks=4/#SBATCH --ntasks=$NRANKS/g" \
      -e "s/RANKS=.*/RANKS=$NRANKS/g" \
      -e "s/export TSMPI_SIM_CONFIG=.*/export TSMPI_SIM_CONFIG=\"mn4_musa.conf\"/g" \
      -e "s/.*TSMPI_SIM_CORES.*/export TSMPI_SIM_CORES=\"$CORELIST\"/g" < $basedir/step1_tracing/job_tracer_sve.bash >job_tracer.bash

  sed -e "s/APPNAME=.*/APPNAME=$APP/g" \
      -e "s#ARGS=.*#ARGS=\"$ARGS\"#g" \
      -e "s/#SBATCH --ntasks=4/#SBATCH --ntasks=$NRANKS/g" \
      -e "s/RANKS=.*/RANKS=$NRANKS/g" \
      -e "s/export TSMPI_SIM_CONFIG=.*/export TSMPI_SIM_CONFIG=\"mn4_musa.conf\"/g" \
      -e "s/.*TSMPI_SIM_CORES.*/export TSMPI_SIM_CORES=\"$CORELIST\"/g" < $basedir/step1_tracing/job_tracer_sve_memory.bash >job_tracer_memory.bash
  cp ${MUSA_HOME}/tasksim/etc/conf/mn4_musa.conf .
  PADRANKS=$(printf "%0*d" 6 $NRANKS)
  cp $basedir/step1_tracing/create_sim_environment.bash .
  chmod 755 create_sim_environment.bash
}

basedir=${MUSA_HOME}/tasksim/etc/examples

if [ $# -eq 0 ]; then
  copy_musa_start_files sp-mz.B.4 "" 4 "1 2 4 8 16 32 64"
elif [ $# -eq 4 ]; then
  copy_musa_start_files $1 $2 $3 $4
else
  echo "Wrong number of parameters for musa_start."
  echo "Provide no parameters for BT-MZ.A.4 (default)."
  echo "Otherwise:"
  echo " $1 - Application name."
  echo " $2 - Application arguments."
  echo " $3 - Number of ranks."
  echo " $4 - List of cores to simulate."
fi
