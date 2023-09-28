#!/bin/bash

### Specific for mn4
module purge
module load musa/2.7_mn4
###########################################################################

config_folder=$1
sim_folder=$2
match=${3-""}

### combine all simulations in one file?
greasyfile=$2".allcores.greasy"
rm -f $greasyfile


### generate only memory marked mpi_phases?
export TS_SIM_ONLY_MEM=1
export TS_SIM_MEM=1
### do you want to keep output logs of mem statistics?
export TS_SIM_LOG=1

for file in $(ls ${config_folder}/); do

  if [[ $file != *"${match}"* ]] && [[ $match != "" ]]; then
    continue
  fi

  config_file=${config_folder}/${file}
  echo ${file}
  echo ${file::-5}
  this_sim_folder=${sim_folder}/${file::-5}
  mkdir -p $this_sim_folder
  echo "------------"
  echo "tasksim_presim -d trace_ts -o ${file::-5}".dat" -c ${config_folder}"/"${file} -t ${this_sim_folder} -g -s >> $greasyfile"

  tasksim_presim \
    -d trace_ts \
    -o ${file::-5}.dat \
    -c ${config_folder}/${file} \
    -t ${this_sim_folder}   \
    -g -s >>  $greasyfile

done
