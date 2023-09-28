#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NANOX_INSTRUMENTATION_LIB
echo "NX_ARGS="--instrument-default=all --smp-workers=1 --throttle-upper=999999 --instrumentation=nextsim-musa" LD_PRELOAD=${TS_MPI_LIB} ./${MUSA_APP}_instr ${ARGS}"
NX_ARGS="$NX_ARGS --instrument-default=all --smp-workers=1 --throttle-upper=999999 --instrumentation=nextsim-musa" LD_PRELOAD=${TS_MPI_LIB} ./${MUSA_APP}_instr ${ARGS}
let index=$OMPI_COMM_WORLD_RANK+1
file=${MUSA_EXP_NAME}/MUSA_relation_redux_${index}.txt
padded_index=$(printf "%0*d" 6 $index)
echo "$index $OMPI_COMM_WORLD_SIZE 0 moon $padded_index" > $file
