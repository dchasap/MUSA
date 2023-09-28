#!/bin/bash
#SBATCH --job-name="MUSA-extrapolate"
#SBATCH -D .
#SBATCH --output=./logs_presim_extrapolation_%j.out
#SBATCH --error=./logs_presim_extrapolation_%j.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=0:30:00

module purge
module load musa/3.1
module load python/3.6
APP=APPNAME
config_name=CONFIG_NAME
# First we generate the Memory-Burst correction factor for the simulated architecture:
PADCORES="000001"
burst_dat=../A1_PRESIM/musa_out_${APP}_${PADCORES}/${config_name}_${PADCORES}_BRST/${config_name}_${PADCORES}_BRST.dat
memory_dat=../A1_PRESIM/musa_out_${APP}_${PADCORES}/${config_name}_${PADCORES}_MEMO/${config_name}_${PADCORES}_MEMO.dat

if [ ! -e $burst_dat ]; then
  echo "[ERROR] MPI phases duration in burst mode -${burst_dat}- not found."
  exit
fi
if [ ! -e $memory_dat ]; then
  echo "[ERROR] MPI phases duration in memory mode -${memory_dat}- not found."
  exit
fi

extrapolate_detail_to_burst.py -b $burst_dat -m $memory_dat
cores="CORELIST"
for core in $cores
do
  PADCORES=$(printf "%0*d" 6 $core)
  burst_dat=./musa_out_${APP}_${PADCORES}/${config_name}_${PADCORES}_BRST/${config_name}_${PADCORES}_BRST.dat
  memory_dat=./musa_out_${APP}_${PADCORES}/${config_name}_${PADCORES}_MEMO/${config_name}_${PADCORES}_MEMO.dat
  if [ ! -e $burst_dat ]; then
    echo "[ERROR] MPI phases duration in burst mode -${burst_dat}- not found."
    exit
  fi
  if [ ! -e $memory_dat ]; then
    echo "[ERROR] MPI phases duration in memory mode -${memory_dat}- not found."
    exit
  fi
  if [ ! -e correction.dat ]; then
    echo "[ERROR] Correction factor file correction.dat not found."
    exit
  fi
  echo "extrapolate_detail_to_burst.py -b $burst_dat -m $memory_dat"
  extrapolate_detail_to_burst.py -b $burst_dat -m $memory_dat -c correction.dat -p 0
done
