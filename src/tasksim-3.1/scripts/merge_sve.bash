#!/bin/bash
let index=$OMPI_COMM_WORLD_RANK+1
sve_num_file=${MUSA_EXP_NAME}/sve_pid_${index}.txt
sve_pid=$(cat $sve_num_file)
if [ ! -z $NONANOS ]; then
    EXTRA_FLAGS="-n"
fi
# OmpSs runtime arguments:
mpi_merge_sve ${EXTRA_FLAGS} -k -f ${MUSA_EXP_NAME}/MUSA_relation_redux.txt -e ${MUSA_EXP_NAME} -p $sve_pid -- ${MUSA_APP} ${ARGS}
rm $sve_num_file
