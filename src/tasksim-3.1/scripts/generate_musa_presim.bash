#!/bin/bash
#SBATCH --job-name="MUSA-presim"
#SBATCH -D .
#SBATCH --output=./logs_presim_generation_%j.out
#SBATCH --error=./logs_presim_generation_%j.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=0:30:00

function generate_presim () {
  config_file=$1
  config_name=${config_file::-5}
  mode=${config_name:(-4)}
  aux=${config_name::-5}
  PADCORES=${aux:(-6)}
  app=$2
  sim_folder=musa_out_${app}_${PADCORES}
  greasyfile=${sim_folder}".${config_name}.${mode}.allcores.greasy"
  if [ -e $greasyfile ]; then
    rm $greasyfile
  fi
  total_greasy=$3
  this_sim_folder=${sim_folder}/${config_name}
  mkdir -p $this_sim_folder
  echo "----- Generating greasy files for ${config_name::-12} with $PADCORES threads and $mode mode -----"
  tasksim_presim -d trace_ts -o ${config_name}.dat -c ${config_file} -t ${this_sim_folder} -g -s >> $greasyfile
  cat $greasyfile >>$total_greasy
  rm $greasyfile
  echo "Done."
}

### Specific for mn4
module load musa/3.1
app=APP
memory_mode=MODE
only_memory_mode=0
# MUSA tutorial in Dibona code:
# third_session=0

### do you want to keep output logs of mem statistics?
export TS_SIM_LOG=1
# Tutorial shortcut not to simulate Burst mode simulations.
if [ $only_memory_mode -ne 0 ]; then
  files=$(ls *MEMO.conf)
elif [ $memory_mode -ne 0 ]; then
  files=$(ls *[MEMO,BRST].conf)
else
  files=$(ls *BRST.conf)
fi
total_greasy=all_${app}_sims.greasy

if [ -e $total_greasy ]; then
  rm $total_greasy
fi

for config_file in $files
do
  if [[ $config_file == *BRST*  ]]; then
    export TS_SIM_ONLY_MEM=0
    export TS_SIM_MEM=0
  else
    export TS_SIM_ONLY_MEM=1
    export TS_SIM_MEM=1
  fi
  generate_presim $config_file $app $total_greasy
done
sed -e "s/GREASY_NAME/${total_greasy}/g" < ${MUSA_HOME}/tasksim/etc/examples/step2_presim/launch_musa_presim.bash > launch_all_musa_presims.bash
exit 

## Tutorial shortcut to copy Burst mode simulation results.
if [ $third_session -ne 0 ]; then
    echo "Burst results already copied."
elif [ $memory_mode -ne 0 ]; then
    folder=$MUSA_TUTORIAL_FILES/session1/burst_simulation_results/TRACE_HPCG_Dibona_nosve_000004_BRST/SIMULATION/A1_PRESIM
else
    folder=$MUSA_TUTORIAL_FILES/session2/memory_simulation_results/TRACE_HPCG_Dibona_nosve_000004_MEMO/SIMULATION/A1_PRESIM
fi
corelist="1 2 4 8 16 32 64"
for cores in $corelist; do
    padcores=$(printf "%0*d" 6 $cores)
    musa_out_HPCG_Dibona_nosve_${padcores}/mn4_musa_${padcores}_BRST
    mkdir -p musa_out_HPCG_Dibona_nosve_${padcores}/mn4_musa_${padcores}_BRST
    cp $folder/musa_out_HPCG_Dibona_nosve_${padcores}/mn4_musa_${padcores}_BRST/mn4_musa_${padcores}_BRST.dat musa_out_HPCG_Dibona_nosve_${padcores}/mn4_musa_${padcores}_BRST
done

