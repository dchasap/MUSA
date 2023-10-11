#!/bin/bash
if [ ! -z $OMPI_COMM_WORLD_RANK ]; then
    let index=$OMPI_COMM_WORLD_RANK+1
else
    let index=$PMIX_RANK+1
fi
# We get the traces sufix:
sufix=$(grep "^$index " ${MUSA_EXP_NAME}/MUSA_relation_redux.txt  | cut -d " " -f5)
# OmpSs trace name:
tmpfilename=${TMPDIR_OUT_TRACE}/file${sufix}
# Memory trace name:
outfilename=${MUSA_EXP_NAME}/${APPNAME}_proc_${sufix}.ts
# OmpSs runtime arguments:
export NX_ARGS="--smp-workers=1 --disable-ut --throttle-upper=9999999"
# We invoke DynamoRIO:
if [ ! -z $NONANOS ]; then
    EXTRA_FLAGS="-n"
fi


lower=$TSMPI_RANK_INIT
upper=$(($TSMPI_RANK_INIT+$TSMPI_RANK_NUM))
if [ $index -ge $lower ] && [ $index -le $upper ]; then
    echo "LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$NANOX_PERFORMANCE_LIB:\${TS_ARMIE_PATH}/samples/bin64 \${TS_ARMIE_PATH}/bin64/drrun -client \${DRTOOL_PATH} 0 \"-i $tmpfilename -o $outfilename -m -r $index ${EXTRA_FLAGS}\" -client \${TS_ARMIE_PATH}/lib64/release/libmemtrace_sve_${VECTOR_LENGTH}.so 1 \"\" -client \${TS_ARMIE_PATH}/lib64/release/libarmv8x.so 2 \"armv82,armv83\" -unsafe_build_ldstex -max_bb_instrs 32 -max_trace_bbs 4 -vm_size 512M -- ${MUSA_APP} ${ARGS}"
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NANOX_PERFORMANCE_LIB:${TS_ARMIE_PATH}/samples/bin64 ${TS_ARMIE_PATH}/bin64/drrun -client ${DRTOOL_PATH} 0 "-i $tmpfilename -o $outfilename -m -r $index ${EXTRA_FLAGS}" -client ${TS_ARMIE_PATH}/lib64/release/libmemtrace_sve_${VECTOR_LENGTH}.so 1 "" -client ${TS_ARMIE_PATH}/lib64/release/libarmv8x.so 2 "armv82,armv83" -unsafe_build_ldstex -max_bb_instrs 32 -max_trace_bbs 4 -vm_size 512M -- ${MUSA_APP} ${ARGS}
else
    echo "./${APPNAME} ${ARGS}"
    ./${APPNAME} ${ARGS}
fi
