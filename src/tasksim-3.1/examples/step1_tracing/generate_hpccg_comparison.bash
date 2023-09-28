#!/bin/bash

APP=HPCG_Dibona_nosve
RANKS="000004"
CORE_LIST="1 2 4 8 16 32"

module load musa

# To ensure a dot separating floating point values
export LC_NUMERIC="en_US"

plot_file=comparison_hpccg.plot
graph_file=${APP}_configuration_comparison.png

# echo "Native executions duration: $native_duration"
if [ -e  TRACE_${APP}_${RANKS}_MEMO/LOGS/log_step.2_memory_tracing/1/rank.0/stdout ]; then
  init_phase=$(grep "ROI entry at the" TRACE_${APP}_${RANKS}_MEMO/LOGS/log_step.2_memory_tracing/1/rank.0/stdout |  grep -o "[0-9]*")
  fini_phase=$(grep "ROI exit at the" TRACE_${APP}_${RANKS}_MEMO/LOGS/log_step.2_memory_tracing/1/rank.0/stdout |  grep -o "[0-9]*")
else
  init_phase=352
  fini_phase=604
fi
sed -e "s/START_VALUE/${init_phase}/g" \
    -e "s/END_VALUE/${fini_phase}/g" <paraver_filter.cfg >paraver_filter_${APP}.cfg

CONFIG_NAME="mn4_musa"
mn4_duration=""
echo "Gathering memory simulation results for $config_name..."
for core in $CORE_LIST; do
  PADCORES=$(printf "%0*d" 6 $core)
  paraver_trace=TRACE_${APP}_${RANKS}_MEMO/SIMULATION/A2_INTEGRATION_PRESIM/trace_SIMULATED/MUSA_${APP}_${PADCORES}cores_${CONFIG_NAME}_presim.prv
  if [ -e $paraver_trace ]; then
    result_file=roi_results_${core}.txt
    paramedir $paraver_trace ./paraver_filter_${APP}.cfg ${result_file}
    duration=$(grep -e "Maximum" ${result_file} | grep -o "[0-9\.]*")
    rm $result_file
    mn4_duration="${mn4_duration} $duration"
  fi
done

CONFIG_NAME="arm_high_end"
armh_duration=""
echo "Gathering memory simulation results for $config_name..."
for core in $CORE_LIST; do
  PADCORES=$(printf "%0*d" 6 $core)
  paraver_trace=TRACE_${APP}_${RANKS}_MEMO/SIMULATION/A2_INTEGRATION_PRESIM/trace_SIMULATED/MUSA_${APP}_${PADCORES}cores_${CONFIG_NAME}_presim.prv
  if [ -e $paraver_trace ]; then
    result_file=roi_results_${core}.txt
    paramedir $paraver_trace ./paraver_filter_${APP}.cfg ${result_file}
    duration=$(grep -e "Maximum" ${result_file} | grep -o "[0-9\.]*")
    rm $result_file
    armh_duration="${armh_duration} $duration"
  fi
done

CONFIG_NAME="arm_low_power"
arml_duration=""
echo "Gathering memory simulation results for $config_name..."
for core in $CORE_LIST; do
  PADCORES=$(printf "%0*d" 6 $core)
  paraver_trace=TRACE_${APP}_${RANKS}_MEMO/SIMULATION/A2_INTEGRATION_PRESIM/trace_SIMULATED/MUSA_${APP}_${PADCORES}cores_${CONFIG_NAME}_presim.prv
  if [ -e $paraver_trace ]; then
    result_file=roi_results_${core}.txt
    paramedir $paraver_trace ./paraver_filter_${APP}.cfg ${result_file}
    duration=$(grep -e "Maximum" ${result_file} | grep -o "[0-9\.]*")
    rm $result_file
    arml_duration="${arml_duration} $duration"
  fi
done

#echo "Memoryt simulation duration: $memory_duration"
#echo "MN4: $mn4_duration"
#echo "ARM H: $armh_duration"
#echo "ARM L: $arml_duration"

# Now we stich everything together

corel=($CORE_LIST)
total=${#corel[*]}
mn4_d=($mn4_duration)
armh_d=($armh_duration)
arml_d=($arml_duration)


result_file=results_roi_${APP}.csv
echo "Writing result file $result_file..."
#echo "CPUS:MareNostrum4e:ARM High End:ARM Low Power" >$result_file
echo "X:Y" >$result_file
i=0
while [ $i -lt $total ]; do
  mn4_s=$(printf '%3.2f\n' $(bc <<< "scale=2; (${mn4_d[0]})/(${mn4_d[$i]})" 2>/dev/null))
  armh_s=$(printf '%3.2f\n' $(bc <<< "scale=2; (${mn4_d[0]})/(${armh_d[$i]})" 2>/dev/null))
  arml_s=$(printf '%3.2f\n' $(bc <<< "scale=2; (${mn4_d[0]})/(${arml_d[$i]})" 2>/dev/null))
  if [ $mn4_s == "0.00" ] || [ -z ${mn4_d[$i]} ]; then
    mn4_s=""
  fi
  if [ $armh_s == "0.00" ] || [ -z ${armh_d[$i]} ]; then
    armh_s=""
  fi
  if [ $arml_s == "0.00" ] || [ -z ${arml_d[$i]} ]; then
    arml_s=""
  fi

  #echo "${corel[$i]}:$mn4_s:$armh_s:$arml_s" >> $result_file
  pos1=$(bc <<< "scale=2; $i * 2.0 + 0.5")
  pos2=$(bc <<< "scale=2; $i * 2.0 + 1.0")
  pos3=$(bc <<< "scale=2; $i * 2.0 + 1.5")
  echo "$pos1:$mn4_s" >> $result_file
  echo "$pos2:$armh_s" >> $result_file
  echo "$pos3:$arml_s" >> $result_file
  let i=$i+1
done
# Plotting graph
echo "Plotting the ${graph_file}..."
gnuplot -e "filename='${graph_file}';input_csv='${result_file}';graph_title='${APP//_/ } configuration comparsion'" ${plot_file}
echo "Displaying ${graph_file}..."
display ${graph_file}
