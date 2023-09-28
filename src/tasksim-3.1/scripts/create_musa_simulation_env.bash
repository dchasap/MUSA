#!/bin/bash

PR=$1
APP=$2
MUSA_EXP_NAME=$3
## optional
ARGSCHAIN=$4

config_name_list=""
for config_file in $TSMPI_SIM_CONFIG; do
  config_name_list="$config_name_list ${config_file::-5}"
done

# ====================== FOLDER SETUP =========================================

SIM=$MUSA_EXP_NAME/SIMULATION
PRESIM=$MUSA_EXP_NAME/SIMULATION/A1_PRESIM
INT_PRESIM=$MUSA_EXP_NAME/SIMULATION/A2_INTEGRATION_PRESIM

# We create the folder structure
mkdir -p $PRESIM
mkdir -p $INT_PRESIM

# We link to the trace folders
ln -s ../../TRACE/trace_ts  $PRESIM/trace_ts
ln -s ../../TRACE/trace_prv $INT_PRESIM/trace_prv
ln -s ../../TRACE/trace_ts  $INT_PRESIM/trace_ts

# And make the result trace folder
mkdir -p $INT_PRESIM/trace_SIMULATED

# ====================== STEP TWO FILES =======================================

STEP_TWO_SOURCE=$MUSA_HOME/tasksim/examples/step2_presim

sed -e "s/APP/$APP/g" -e "s/MODE/${MEMORY_MODE}/g"< ${MUSA_HOME}/tasksim/bin/generate_musa_presim.bash > $MUSA_EXP_NAME/SIMULATION/A1_PRESIM/generate_musa_presim.bash
chmod 755 $MUSA_EXP_NAME/SIMULATION/A1_PRESIM/generate_musa_presim.bash

corelminus=${TSMPI_SIM_CORES:2}
for config_name in $config_name_list; do
sed -e "s/APPNAME/${APP}/g" \
    -e "s/CONFIG_NAME/${config_name}/g" \
    -e "s/CORELIST/${corelminus}/g" \
    < $STEP_TWO_SOURCE/extrapolate_burst_duration.bash \
    > $PRESIM/extrapolate_burst_duration_${config_name}.bash
chmod 755 $PRESIM/extrapolate_burst_duration_${config_name}.bash
done

# We copy the cleanup script.
sed -e "s/APPNAME/${APP}/g" \
    -e "s/CORELIST/${TSMPI_SIM_CORES}/g" \
    < $STEP_TWO_SOURCE/cleanup.bash \
    > $PRESIM/cleanup.bash
chmod 755 $PRESIM/cleanup.bash


# ====================== STEP THREE FILES =====================================

STEP_THREE_SOURCE=${MUSA_HOME}/tasksim/etc/examples/step3_integration
DIM_TRACE=${MUSA_EXP_NAME}/TRACE/trace_prv/trace_${APP}.dim

cp $STEP_THREE_SOURCE/02_Dimemas_sim.bash $INT_PRESIM/
sed -e "s/CORELIST/$TSMPI_SIM_CORES/g" \
    -e "s/CONFIGLIST/$config_name_list/g" \
    < $STEP_THREE_SOURCE/integrate_dimemas_simulations.bash \
    > ${INT_PRESIM}/integrate_dimemas_simulations.bash
chmod 755 ${INT_PRESIM}/integrate_dimemas_simulations.bash

cad="0"
long=""
for (( c=1; c<$PR; c++ ))
do
  cad+=","$c
  long+=$'\\n"node information" {0, '$c', "", 1, 1.0, 0.000002, 6000.0, 1, 1, 0, 0.0, 1, 1, 0.0};;'
done

sed -e "s|dimemas.collectives|$STEP_THREE_SOURCE/dimemas.collectives|g" \
    -e "s|XXXX.dim|../../../$DIM_TRACE|g" \
    -e "s|YYYY|$PR|g" \
    -e "s|ZZZZ|$cad|g" \
    -e "s|BBBB|$long|g" \
    < $STEP_THREE_SOURCE/config_NET.cfg \
    > $INT_PRESIM/config_NET.cfg



# Create the files that depend on core+config
for config in $TSMPI_SIM_CONFIG; do
  for core in $TSMPI_SIM_CORES; do
    create_musa_core_sim.bash $PR $APP $MUSA_EXP_NAME $core $config
  done
done

