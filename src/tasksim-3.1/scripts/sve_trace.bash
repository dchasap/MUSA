#!/bin/bash
vec=$1
shift
app=$1
shift
args=$*

export OMPSS_TRACE_FILE="${app}_${vec}_ompss.ts"
export DR_TRACE="${app}_${vec}_intermediate.ts"
export OUTPUT_TRACE="${app}_${vec}.ts"
export NX_ARGS="--instrument-default=all --disable-ut --smp-workers=1 --throttle-upper=9999999 --instrumentation=nextsim"
export OMP_NUM_THREADS=1
echo "First Step: OmpSs trace generation."
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$NANOX_INSTRUMENTATION_LIB:\${TS_ARMIE_PATH}/samples/bin64"
echo "${TS_ARMIE_PATH}/bin64/drrun -unsafe_build_ldstex -max_bb_instrs 32 -max_trace_bbs 4 -c ${TS_ARMIE_PATH}/lib64/release/libsve_128.so -- ./${app}_instr ${args}"
old_LD_LIBRARY_PATH=$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NANOX_INSTRUMENTATION_LIB:${TS_ARMIE_PATH}/samples/bin64
${TS_ARMIE_PATH}/bin64/drrun -unsafe_build_ldstex -max_bb_instrs 32 -max_trace_bbs 4 -vm_size 512M -c ${TS_ARMIE_PATH}/lib64/release/libsve_128.so -- ./${app}_instr ${args}
rm sve-memtrace.${app}.*.log
export NX_ARGS="--disable-ut --smp-workers=1 --throttle-upper=9999999"
echo "Second step: DynamoRIO trace generation."
export LD_LIBRARY_PATH=$old_LD_LIBRARY_PATH:$NANOX_PERFORMANCE_LIB
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${TS_ARMIE_PATH}/samples/bin64 ${TS_ARMIE_PATH}/bin64/drrun -client ${DRTOOL_PATH} 0 "-i $OMPSS_TRACE_FILE -o $DR_TRACE" -client ${TS_ARMIE_PATH}/lib64/release/libmemtrace_sve_${vec}.so 1 "" -unsafe_build_ldstex -max_bb_instrs 32 -max_trace_bbs 4 -vm_size 512M -- ${app} ${args}
echo "Third step: Merge DynamoRIO trace and SVE result txt."
echo "merge_sve_memtrace -i $DR_TRACE -o $OUTPUT_TRACE -l sve-memtrace.${app}.*.log"
merge_ompss_dr_traces -n $OMPSS_TRACE_FILE -d $DR_TRACE
merge_sve_memtrace -i $DR_TRACE -o $OUTPUT_TRACE -l sve-memtrace.${app}.*.log
trace_folder="trace_${vec}_${app}_${args//\ /_}"
mkdir -p $trace_folder
mv ${OUTPUT_TRACE}.* $trace_folder
rm ${DR_TRACE}.*
rm ${OMPSS_TRACE_FILE}.*
rm sve-memtrace.${app}.*.log
export LD_LIBRARY_PATH=$old_LD_LIBRARY_PATH
