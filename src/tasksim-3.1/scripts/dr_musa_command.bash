#!/bin/bash
# redux is a file containing the list of rank and sufixes.
redux=${MUSA_EXP_NAME}/MUSA_relation_redux.txt
# We get the rank (1-indexed)
let index=$OMPI_COMM_WORLD_RANK+1
# We get the traces sufix:
sufix=$(grep "^$index " $redux | cut -d " " -f5)
# OmpSs trace name:
tmpfilename=${TMPDIR_OUT_TRACE}/file${sufix}
# Memory trace name:
outfilename=${MUSA_EXP_NAME}/${APPNAME}_proc_${sufix}.ts
# OmpSs runtime arguments:
export NX_ARGS="--smp-workers=1 --disable-ut --throttle-upper=9999999"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NANOX_PERFORMANCE_LIB
# We invoke DynamoRIO:
echo "${DR_PATH} ${DR_ARGS} -c ${DRTOOL_PATH} -i $tmpfilename -o $outfilename -m -- ${APPNAME} ${ARGS}"
${DR_PATH} ${DR_ARGS} -c ${DRTOOL_PATH} -i $tmpfilename -o $outfilename -m -- ${APPNAME} ${ARGS}

