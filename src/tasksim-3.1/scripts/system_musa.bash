#!/bin/bash 


### experiment folder
if [ -z $MUSA_EXP_NAME ]; then
  NEWFOLD="./exec_0001"
else
  NEWFOLD=$MUSA_EXP_NAME
fi
mkdir -p $NEWFOLD


### trace folder
FOLDTRACE=${NEWFOLD}/TRACE
mkdir -p $FOLDTRACE
export FINAL_TRACE=${FOLDTRACE}"/trace_ts"
mkdir -p $FINAL_TRACE
export FINAL_TRACE_PRV=${FOLDTRACE}"/trace_prv"
mkdir -p $FINAL_TRACE_PRV
### logs folder
export LOGS=${NEWFOLD}/LOGS/
mkdir -p $LOGS

### SYSTEM PARAMETERS
### Temporal dir. MPI 1.8.1 required
export TMPDIR=/tmp/
mkdir -p $TMPDIR

### where to store temporal
FOLDTEMP=${NEWFOLD}/xxtemp/
### Final temporary directory. Local gpfs folder
export TMPDIR_OUT_TRACE=${FOLDTEMP}/tmp/
mkdir -p $TMPDIR_OUT_TRACE
### Node temporary directory. Not gpfs
#export TMPDIR_TMP_TRACE=$TMPDIR/musa_temporal
export TMPDIR_TMP_TRACE=${FOLDTEMP}/tmp/
mkdir -p $TMPDIR_TMP_TRACE

### MARENOSTRUM default extrae environment
## update...instead of config file, let's use EXTRAE_ON

#if [ -z "$EXTRAE_CONFIG_FILE" ]; then
#  export EXTRAE_CONFIG_FILE=${MUSA_HOME}/01_src/tasksim/examples/step1_tracing/extrae.xml
#fi


if [ -z "$EXTRAE_HOME" ]; then 
  echo "[ERROR] EXTRAE_HOME is not defined."
fi

# We set a default master alias of 1800500 as was in the tutorial example
if [ -z $TSMPI_MASTER_ALIAS ]; then
  export TSMPI_MASTER_ALIAS=18005000
fi


