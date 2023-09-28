#!/bin/bash
# redux is a file containing the list of rank and sufixes.
redux=${MUSA_EXP_NAME}/MUSA_relation_redux.txt
# We get the rank (1-indexed)
if [ ! -z $OMPI_COMM_WORLD_RANK ]; then
    let index=$OMPI_COMM_WORLD_RANK+1
else
    let index=$PMIX_RANK+1
fi
# We get the traces sufix:
sufix=$(grep "^$index " $redux | cut -d " " -f5)
# OmpSs trace name:
tmpfilename=${TMPDIR_OUT_TRACE}/file${sufix}
# Memory trace name:
outfilename=${MUSA_EXP_NAME}/${APPNAME}_proc_${sufix}.ts
# OmpSs runtime arguments:
export NX_ARGS="--smp-workers=1 --disable-ut --throttle-upper=9999999"
DR_ARGS="$DR_ARGS -unsafe_build_ldstex -max_bb_instrs 32 -max_trace_bbs 4"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NANOX_PERFORMANCE_LIB
# We invoke DynamoRIO:
if [ -z ${VECTOR_LENGTH} ]; then
    VECTOR_LENGTH=256
fi

lower=$TSMPI_RANK_INIT
upper=$(($TSMPI_RANK_INIT + $TSMPI_RANK_NUM - 1))
if [ $index -ge $lower ] && [ $index -le $upper ]; then
    echo "${DR_PATH} -client ${DRTOOL_PATH} 0 "-i $tmpfilename -o $outfilename -m"  -client ${TS_ARMIE_PATH}/lib64/release/libsve_${VECTOR_LENGTH}.so 1 "" ${DR_ARGS} -- ${APPNAME} ${ARGS}"
    ${DR_PATH} -client ${DRTOOL_PATH} 0 "-i $tmpfilename -o $outfilename -m"  -client ${TS_ARMIE_PATH}/lib64/release/libsve_${VECTOR_LENGTH}.so 1 "" ${DR_ARGS} -- ${APPNAME} ${ARGS}
else
    echo "./${APPNAME} ${ARGS}"
    ./${APPNAME} ${ARGS}
fi
