#!/bin/bash
#SBATCH --job-name="MUSA-run-presim"
#SBATCH -D .
#SBATCH --output=./logs_presim_run_%j.out
#SBATCH --error=./logs_presim_run_%j.err
#SBATCH --ntasks=16
#SBATCH --cpus-per-task=1
#SBATCH --time=01:00:00

### Specific for mn4
module purge
module load musa/3.1
###########################################################################
greasy_name=GREASY_NAME

greasy $greasy_name

