#!/bin/bash 

NP=$1
APP=$2
MUSA_EXP_NAME=$3
CORE=$4
CONFIG=$5

PADCORE=`printf "%0*d" 6 $CORE`
config_name=${CONFIG::-5}

echo -e '\t'"["$1"]["$2"]["$3"]["$4"]"
MUSA_TS_TRACE=${MUSA_EXP_NAME}/TRACE/trace_ts
MUSA_PRV_TRACE=${MUSA_EXP_NAME}/TRACE/trace_prv
DIM_TRACE=${MUSA_EXP_NAME}/TRACE/trace_prv/trace_${APP}.dim


SIM=$MUSA_EXP_NAME/SIMULATION
PRESIM=$MUSA_EXP_NAME/SIMULATION/A1_PRESIM
INT_PRESIM=$MUSA_EXP_NAME/SIMULATION/A2_INTEGRATION_PRESIM

# ==================== Step 3: Tasksim phase simulation. =============================
# We copy the configuration files:

sed -e "s/mode_selector.*/mode_selector\ =\ BURST_ONLY/g" \
    -e "s/ncpus.*/ncpus\ =\ $CORE/g" \
    <$CONFIG \
    >$MUSA_EXP_NAME/SIMULATION/A1_PRESIM/${config_name}_${PADCORE}_BRST.conf

if [ $MEMORY_MODE -ne 0 ]; then
  sed -e "s/mode_selector.*/mode_selector\ =\ MEMORY/g" \
      -e "s/ncpus.*/ncpus\ =\ $CORE/g" \
      <$CONFIG \
      >$MUSA_EXP_NAME/SIMULATION/A1_PRESIM/${config_name}_${PADCORE}_MEMO.conf
fi

# ==================== Step 3: Dimemas integration. =============================
STEP_THREE_SOURCE=$MUSA_HOME/tasksim/etc/examples/step3_integration

if [ $CORE -eq 1 ] || [ $MEMORY_MODE -eq 0 ]; then
  result_name=../A1_PRESIM/musa_out_${APP}_${PADCORE}/${config_name}_${PADCORE}_BRST/${config_name}_${PADCORE}_BRST.dat
else
  result_name=../A1_PRESIM/musa_out_${APP}_${PADCORE}/${config_name}_${PADCORE}_BRST/${config_name}_${PADCORE}_FINAL.dat
fi

sed -e "s/^CORE.*/CORE="$CORE"/g" \
    -e "s/APPNAME/$APP/g" \
    -e "s#PRESIMNAME#${result_name}#g" \
    -e "s/CONFIGFILE/${config_name}/g" \
    < $STEP_THREE_SOURCE/03_DimemasTS_sim_presim.bash \
    > $INT_PRESIM/03_DimemasTS_sim_presim_${config_name}_${PADCORE}.bash

