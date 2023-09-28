#!/bin/bash
#SBATCH --job-name="MUSA-gen-presim"
#SBATCH -D .
#SBATCH --output=./logs_presim_generation_%j.out
#SBATCH --error=./logs_presim_generation_%j.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=2:00:00

### Specific for mn4
module purge
module load musa/3.1
###########################################################################

config_folder=CONFIG_FOLDER
sim_folder=SIM_FOLDER

### combine all simulations in one file?
greasyfile=${sim_folder}".allcores.greasy"
rm -f $greasyfile


### generate only memory marked mpi_phases?
export TS_SIM_ONLY_MEM=1
export TS_SIM_MEM=1
### do you want to keep output logs of mem statistics?
export TS_SIM_LOG=1

for file in $(ls ${config_folder}/); do

  config_file=${config_folder}/${file}
  config_name=${file::-5}
  echo "Using configuration file ${file}"
  echo "Using ${config_name} as configuration name"
  this_sim_folder=${sim_folder}/${config_name}
  mkdir -p $this_sim_folder
  echo "------------ Issuing command -----------------"
  echo "tasksim_presim -d trace_ts -o ${config_name}".dat" -c ${config_folder}"/"${file} -t ${this_sim_folder} -g -s >> $greasyfile"

  tasksim_presim \
    -d trace_ts \
    -o ${config_name}.dat \
    -c ${config_folder}/${file} \
    -t ${this_sim_folder}   \
    -g -s >>  $greasyfile
done 


