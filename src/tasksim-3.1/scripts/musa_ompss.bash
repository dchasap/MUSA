#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NANOX_INSTRUMENTATION_LIB
echo "NX_ARGS="--instrument-default=all --smp-workers=1 --throttle-upper=999999 --instrumentation=nextsim-musa" LD_PRELOAD=${TS_MPI_LIB} ./${MUSA_APP}_instr ${ARGS}"
NX_ARGS="$NX_ARGS --instrument-default=all --smp-workers=1 --throttle-upper=999999 --instrumentation=nextsim-musa" LD_PRELOAD=${TS_MPI_LIB} ./${MUSA_APP}_instr ${ARGS}
if [ ! -z $OMPI_COMM_WORLD_RANK ]; then
    let index=$OMPI_COMM_WORLD_RANK+1
else
    let index=$PMIX_RANK+1
fi
if [ ! -z $OMPI_COMM_WORLD_SIZE ]; then
    rank=$OMPI_COMM_WORLD_SIZE
else
    rank=$PJM_MPI_PROC
fi
file=${MUSA_EXP_NAME}/MUSA_relation_redux_${index}.txt
padded_index=$(printf "%0*d" 6 $index)
echo "$index $rank 0 moon $padded_index" > $file
