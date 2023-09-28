#!/bin/bash
if [ $# -eq 1 ]; then
  if [ $1 == "bt-mz" ]; then
    echo "Copying and linking bt-mz"
    cp ${MUSA_HOME}/app_examples/bt . -R
    ln -s bt/bin/bt-mz.A.4 .
    ln -s bt/bin/bt-mz.A.4_instr .
  elif [ $1 == "sp-mz" ]; then
    echo "Copying and linking sp-mz"
    cp ${MUSA_HOME}/app_examples/spmz . -R
    ln -s spmz/bin/sp-mz.B.4 .
    ln -s spmz/bin/sp-mz.B.4_instr .
  else
    echo "Unknown benchmark $1. Possible parameters are:"
    echo " 1 sp-mz"
    echo " 2 bt-mz"
  fi
else
  echo "Copying and linking sp-mz"
  cp ${MUSA_HOME}/app_examples/spmz . -R
  ln -s spmz/bin/sp-mz.B.4 .
  ln -s spmz/bin/sp-mz.B.4_instr .
fi
