#!/bin/bash
rm greasy*log
rm logs_presim*
rm *greasy
rm correction.dat

cores="CORELIST"
app="APPNAME"

for core in $cores
do
  PADCORES=$(printf "%0*d" 6 $core)
  folder=musa_out_${app}_${PADCORES}
  rm $folder -r
done
