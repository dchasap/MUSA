#!/bin/bash
#SBATCH --job-name=APPNAME_sve_trace
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --out=./logs/trace_generation-%j.out
#SBATCH --err=./logs/trace_generation-%j.err
#SBATCH --time=1:00:00

module purge
module load musa/3.1

app=APPNAME
args="APP_ARGUMENTS"
for vec in 128 256 512 1024 2048; do
    sve_trace.bash $vec $app $args &>logs/trace_generation_sve_${vec}_${app}_${args//\ /_}.txt
done
scalar_app=${app//sve/scalar}
OMP_NUM_THREADS=1 ompss_dr $scalar_app $args &>logs/trace_generation_scalar_${app}_${args//\ /_}.txt
scalar_folder=trace_${scalar_app}_${args//\ /_}
mkdir ${scalar_folder}
mv ${scalar_app}.ts.*  ${scalar_folder}
