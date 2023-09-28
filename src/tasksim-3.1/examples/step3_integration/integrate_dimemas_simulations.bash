#!/bin/bash
#SBATCH --job-name="MUSA-integrate"
#SBATCH -D .
#SBATCH --output=./logs_merge_simulations_%j.out
#SBATCH --error=./logs_merge_simulations_%j.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=0:30:00

module load musa/3.1

config_name_list="CONFIGLIST"
cores="CORELIST"
for config in $config_name_list; do
  for core in $cores
  do
    echo "Integrating Dimemas data for $core cores."
    PADCORES=$(printf "%0*d" 6 $core)
    bash 03_DimemasTS_sim_presim_${config}_${PADCORES}.bash
  done
done

