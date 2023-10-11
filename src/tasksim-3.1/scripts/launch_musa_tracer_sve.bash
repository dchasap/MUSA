#!/bin/bash


export TMPDIR_ENABLED=1


# ======== launch_musa.bash ===================
# ===============================================

## load arguments
## RANKS   // Number of MPI ranks
## APPNAME // Binary name
## ARGS    // Application arguments

if [ $# -lt 2 ]; then
  echo "Error, missing parameters."
  exit
fi
PR=$1
APP=$2
ARGS=${@:3}


echo "##################################################################"
echo "##################################################################"
echo -e RANKS:[$PR]
echo -e APPNAME:[$APP]
echo -e ARGS[$ARGS]

APPNAME=$(basename ${APP})

export MUSA_APP=$APPNAME

## REMOVAL OF / | ? * + % < > ! space \
ARGSCHAIN=`echo $ARGS | sed 's/\/\||\|?\|*\|+\|%\|<\|>\|"\| \|\\\/_/g'`
if [ ! -z ${ARGSCHAIN} ] ; then
  ARGSCHAIN="X"$ARGSCHAIN"X"
else
  ARGSCHAIN=""
fi
export APPNAME
export ARGS

PRCAD=`printf "%0*d" 6 $PR `

if [ ${MEMORY_MODE} -eq 1 ]; then
  MODECAD="MEMO"
else
  MODECAD="BRST"
fi

if [ -z ${VECTOR_LENGTH} ]; then
    export VECTOR_LENGTH=128
fi

###### MUSA_EXP_NAME must be modified if TSMPI_SIM_CONFIG is provided
### and a valid configuration.
### we assume the following name pattern. tasksim_XXXX.conf
### the XXXX is going to be agreggated to the exp_name.
ARCH=""
if [ ! -z $TSMPI_SIM_CONFIG ]; then
  ## file must exist.
  if [ -f $TSMPI_SIM_CONFIG ]; then
    ## can be an absoulte path... remove string before pattern.
    ARCH=`echo ${TSMPI_SIM_CONFIG##*/}`
    ## extract arch definition.
    ARCH=`echo $ARCH | sed 's/^tasksim_//g' | sed 's/\.conf$//g'`
    ARCH="Y"$ARCH"Y"
  else
     echo "MUSA ERROR :: A non-existent configuration file has been provided."
     echo "             Can not find " $TSMPI_SIM_CONFIG
     exit
  fi
else
  ## no ARCH definition is allowed
  ARCH=""
fi

ARCH=SVE_${VECTOR_LENGTH}

export MUSA_EXP_NAME=TRACE_${APPNAME}_${ARCH}_${PRCAD}_${MODECAD}

# We check for Fortran mpi library:
fortran=$(ldd $APPNAME | grep "mpif")
if [ "x$fortran" == "x" ]; then
   EXTRAE_LIB="libmpitrace.so"
else
   EXTRAE_LIB="libmpitracef.so"
fi



source  system_musa.bash


############## STEP 1 -- TASKSIM TRACE ################################
echo "##################################################################"
echo "MUSA: STEP 1 -- BURST TRACE"
echo "##################################################################"

if [ -z $PARALLEL_INSTRUMENTATION ]; then
  PARALLEL_INSTRUMENTATION=0
fi

if [ $PARALLEL_INSTRUMENTATION -eq 1 ]; then
  echo -e "----------------------------------------------------------"
  echo -e "MUSA:  STEP 1 -- TASKSIM TRACE and EXTRAE TRACE (combined)"
  echo -e "----------------------------------------------------------"
  export EXTRAE_ON=1
  export EXTRAE_DIR=${TMPDIR_OUT_TRACE}/
  source ${EXTRAE_HOME}/etc/extrae.sh
  ### Preload tasksim and extrae wrappers
  export LD_PRELOAD=${EXTRAE_HOME}/lib/${EXTRAE_LIB}
  ### EXECUTION. BURST MODE (mandatory, parallel)
  logname="log_step.1_burst_parallel"
  { LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NANOX_PERFORMANCE_LIB mpirun -np ${PR} --report-bindings --output-filename ${LOGS}/${logname} ${TS_ARMIE_PATH}/bin64/drrun -client ${TS_ARMIE_PATH}/lib64/release/libsve_${VECTOR_LENGTH}.so 1 -- ompss_mpi_perf ${APP} ${ARGS} ; } \
              1>${LOGS}/${logname}.out 2>${LOGS}/${logname}.err
  export LD_PRELOAD=""
  export EXTRAE_ON=0

else
  echo -e "----------------------------------------------------------"
  echo -e "MUSA:  STEP 1.1 -- TASKSIM TRACE"
  echo -e "----------------------------------------------------------"
  # Tasksim trace generation

  echo -e "[MUSA] TMPDIR_TMP_TRACE=${TMPDIR_TMP_TRACE}"
  echo -e "[MUSA] TMPDIR_OUT_TRACE=${TMPDIR_OUT_TRACE}"
  echo -e "[MUSA] NX_ARGS=${NX_ARGS}"
  echo -e "[MUSA] TSMPI_MASTER_ALIAS=${TSMPI_MASTER_ALIAS}"
  echo -e "[MUSA] TSMPI_RANK_INIT=${TSMPI_RANK_INIT}"
  echo -e "[MUSA] TSMPI_RANK_NUM=${TSMPI_RANK_NUM}"
  echo -e "[MUSA] TSMPI_MEM_PHASES_INIT=${TSMPI_MEM_PHASES_INIT}"
  echo -e "[MUSA] TSMPI_MEM_PHASES_NUM=${TSMPI_MEM_PHASES_NUM}"
  echo -e "[MUSA] MUSA_EXP_NAME=${MUSA_EXP_NAME}"
  echo -e "[MUSA] mpirun --report-bindings -np ${PR} ompss_mpi_perf ${APP} ${ARGS}"

  # step 1.1 -- EXECUTION. BURST MODE only
  logname="log_step.1.1_burst"
 { LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NANOX_INSTRUMENTATION_LIB:${TS_ARMIE_PATH}/samples/bin64 mpirun -np ${PR} --bind-to core  --report-bindings --output-filename ${LOGS}/${logname} sve_ompss.bash; } \
              1>${LOGS}/${logname}.out 2>${LOGS}/${logname}.err
  if [ $? -ne 0 ]; then
      echo "STEP 1.1 TaskSim Nanos++ trace failed."
      exit
  fi

  rm ${MUSA_EXP_NAME}/MUSA_relation_redux.txt
  for rank in $(seq 1 ${PR}); do
    cat ${MUSA_EXP_NAME}/MUSA_relation_redux_${rank}.txt >>${MUSA_EXP_NAME}/MUSA_relation_redux.txt
    rm ${MUSA_EXP_NAME}/MUSA_relation_redux_${rank}.txt
  done

  echo -e "----------------------------------------------------------"
  echo -e "MUSA:  STEP 1.2 -- EXTRAE TRACE"
  echo -e "----------------------------------------------------------"

  ### Extrae/paraver trace generation
  export EXTRAE_ON=1
  export EXTRAE_DIR=./tmp/
  export EXTRAE_DIR=${TMPDIR_OUT_TRACE}/
  source ${EXTRAE_HOME}/etc/extrae.sh
  ### Preload tasksim and extrae wrappers
  export EXTRAE_INTERPOSITION_LIBRARY=${EXTRAE_HOME}/lib/${EXTRAE_LIB}

  if [ ! -e ${EXTRAE_HOME}/lib/${EXTRAE_LIB} ]; then
    echo "Extrae's mpitrace library ${EXTRAE_HOME}/lib/${EXTRAE_LIB} does not exist."
  fi

  ### step 1.2 -- EXECUTION. EXTRAE MODE only
  mkdir set-0
  logname="log_step.1.2_extrae"
  echo " Executing LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$NANOX_PERFORMANCE_LIB:\${TS_ARMIE_PATH}/samples/bin64 NX_ARGS="--smp-workers=1" mpirun -np ${PR} -- extrae_trace_command_sve.bash"
  { LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NANOX_PERFORMANCE_LIB:${TS_ARMIE_PATH}/samples/bin64 NX_ARGS="--smp-workers=1" mpirun -np ${PR} --bind-to core --report-bindings --output-filename ${LOGS}/${logname} extrae_trace_command_sve.bash ; } \
          1> ${LOGS}/${logname}.out 2> ${LOGS}/${logname}.err
  if [ $? -ne 0 ]; then
      echo "STEP 1.2 Extrae Nanos++ trace failed."
      exit
  fi
      
  export EXTRAE_ON=0
  mv ./TRACE.mpits ${TMPDIR_OUT_TRACE}
  mv set-* ${TMPDIR_OUT_TRACE}
fi


############## STEP 2 -- MEMORY TRACE (optional) ######################

if [ ${MEMORY_MODE} -eq 1 ]; then
  echo "##################################################################"
  echo "MUSA: STEP 2 -- MEMORY TRACE"
  echo "##################################################################"
  export NX_ARGS="--smp-workers=1"

  if [ ! -z $NONANOS ]; then
      EXTRA_FLAGS="-n"
  fi
  logname="log_step.2_memory_tracing"
  echo "Executing mpirun -np ${PR} --bind-to core dr_musa_command_sve.bash"
  { mpirun -np ${PR} --bind-to core --report-bindings --output-filename ${LOGS}/${logname} dr_musa_command_sve.bash ; } \
      1>${LOGS}/${logname}.out 2>${LOGS}/${logname}.err
  if [ $? -ne 0 ]; then
      echo "STEP 2.1 TaskSim Memory trace generation failed."
      exit
  fi

  logname="log_step.2.2_memory_merge"
  echo "Executing mpirun -np ${PR} --bind-to core merge_sve.bash"
  { mpirun -np ${PR} --bind-to core --report-bindings --output-filename ${LOGS}/${logname} merge_sve.bash ; } \
      1>${LOGS}/${logname}.out 2>${LOGS}/${logname}.err
  if [ $? -ne 0 ]; then
      echo "STEP 2.2 Memory trace merge with SVE failed."
      exit
  fi
else
  echo "##################################################################"
  echo "MUSA: STEP 2 -- MEMORY TRACE"
  echo "##################################################################"
  echo -e "SKIPPED"
fi

# We move the end result traces to the destination folder
mv ${MUSA_EXP_NAME}/MUSA* ${LOGS}/

echo -e "----------------------------------------------------------"
echo -e "MUSA:    STEP 3  -- extrae and dimemas traces"
echo -e "----------------------------------------------------------"

{ ${EXTRAE_HOME}/bin/mpi2prv -syn -f ${TMPDIR_OUT_TRACE}/TRACE.mpits -o ${FINAL_TRACE_PRV}/trace_${APPNAME}.prv -e ./${APP} ; }  \
              2>${LOGS}/log_step.3.2_mpi2prv.err \
              1>${LOGS}/log_step.3.2_mpi2prv.out \
  && { ${DIMEMAS_HOME}/bin/prv2dim ${FINAL_TRACE_PRV}/trace_${APPNAME}.prv ${FINAL_TRACE_PRV}/trace_${APPNAME}.dim ; } \
              2>${LOGS}/log_step.3.3_prv2dim.err \
              1>${LOGS}/log_step.3.3_prv2dim.out
#------------------------------------------------------------


### create a simulation experiment
echo "##################################################################"
echo "MUSA:  FINAL STAGE -- Creating simulaton environments"
echo "##################################################################"

if [ -z $TSMPI_SIM_CONFIG ]; then
  TSMPI_SIM_CONFIG=$(ls *.conf)
  echo "Configuration file was not provided, using:\" $TSMPI_SIM_CONFIG\""
fi

create_musa_simulation_env.bash \
        $PR \
        $APPNAME \
        $MUSA_EXP_NAME \
        $ARGSCHAIN

echo "##################################################################"
echo "##################################################################"

