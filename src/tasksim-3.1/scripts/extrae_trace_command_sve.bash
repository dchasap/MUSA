#!/bin/bash
#echo "Interposing $EXTRAE_INTERPOSITION_LIBRARY"
echo "LD_PRELOAD=\${EXTRAE_INTERPOSITION_LIBRARY}  \${TS_ARMIE_PATH}/bin64/drrun -max_bb_instrs 32 -max_trace_bbs 4 -client \${TS_ARMIE_PATH}/lib64/release/libsve_128.so 0 \"\" -client \${TS_ARMIE_PATH}/lib64/release/libarmv8x.so 1 \"armv82,armv83\" -- ./${APPNAME} ${ARGS}"
LD_PRELOAD=${EXTRAE_INTERPOSITION_LIBRARY}  ${TS_ARMIE_PATH}/bin64/drrun -client ${TS_ARMIE_PATH}/lib64/release/libsve_128.so 0 "" -client ${TS_ARMIE_PATH}/lib64/release/libarmv8x.so 1 "armv82,armv83" -unsafe_build_ldstex -max_bb_instrs 32 -max_trace_bbs 4 -vm_size 512M -- ./${APPNAME} ${ARGS}
