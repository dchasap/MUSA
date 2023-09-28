#!/bin/bash

APP=bt-mz.B.4
RANKS="000004"
CORE_LIST="1 2 4 8 16 32 64"
CONFIG_NAME=mn4_musa

module load musa

# To ensure a dot separating floating point values
export LC_NUMERIC="en_US"

# First we gather native run times.
echo "Gathering native results..."
for core in $CORE_LIST; do
log_file=./native_logs/${core}th_run.txt
  native_duration="$native_duration $(grep "Total time spent in ROI" $log_file | grep -o "[0-9\.]*" | head -n1)"
done

# echo "Native executions duration: $native_duration"
if [ -e  TRACE_${APP}_${RANKS}_MEMO/LOGS/log_step.2_memory_tracing/1/rank.0/stdout ]; then
  init_phase=$(grep "ROI entry at the" TRACE_${APP}_${RANKS}_MEMO/LOGS/log_step.2_memory_tracing/1/rank.0/stdout |  grep -o "[0-9]*")
  fini_phase=$(grep "ROI exit at the" TRACE_${APP}_${RANKS}_MEMO/LOGS/log_step.2_memory_tracing/1/rank.0/stdout |  grep -o "[0-9]*")
else
  init_phase=352
  fini_phase=604
fi
echo "-$init_phase- -$fini_phase-"
sed -e "s/START_VALUE/${init_phase}/g" \
    -e "s/END_VALUE/${fini_phase}/g" <paraver_filter.cfg >paraver_filter_${APP}.cfg

# Then we gather Burst simulation times
echo "Gathering burst simulation results..."
for core in $CORE_LIST; do
  PADCORES=$(printf "%0*d" 6 $core)
  paraver_trace=TRACE_${APP}_${RANKS}_BRST/SIMULATION/A2_INTEGRATION_PRESIM/trace_SIMULATED/MUSA_${APP}_${PADCORES}cores_${CONFIG_NAME}_presim.prv
  if [ -e $paraver_trace ]; then
    result_file=burst_roi_results_${core}.txt
    paramedir $paraver_trace ./paraver_filter_${APP}.cfg ${result_file}
    duration=$(grep -e "Maximum" ${result_file} | grep -o "[0-9\.]*")
    burst_duration="${burst_duration} $duration"
    rm ${result_file}
  fi
done

#echo "Burst simulation duration: $burst_duration"

# Then we gather Memory simulation times.
echo "Gathering memory simulation results..."
for core in $CORE_LIST; do
  PADCORES=$(printf "%0*d" 6 $core)
  paraver_trace=TRACE_${APP}_${RANKS}_MEMO/SIMULATION/A2_INTEGRATION_PRESIM/trace_SIMULATED/MUSA_${APP}_${PADCORES}cores_${CONFIG_NAME}_presim.prv
  if [ -e $paraver_trace ]; then
    result_file=burst_roi_results_${core}.txt
    paramedir $paraver_trace ./paraver_filter_${APP}.cfg ${result_file}
    duration=$(grep -e "Maximum" ${result_file} | grep -o "[0-9\.]*")
    memory_duration="${burst_duration} $duration"
    rm ${result_file}
  fi
done

#echo "Memoryt simulation duration: $memory_duration"

# Now we stich everything together

corel=($CORE_LIST)
total=${#corel[*]}
nat=($native_duration)
bur=($burst_duration)
mem=($memory_duration)

result_file=results_${APP}.csv
graph_file=${APP}_speedup.png

echo "Writing result file $result_file..."
echo "CPUS:Native:Burst:Memory" >$result_file
i=0
while [ $i -lt $total ]; do
  nat_s=$(printf '%3.2f\n' $(bc <<< "scale=2; ${nat[0]}/${nat[$i]}" 2>/dev/null))
  bur_s=$(printf '%3.2f\n' $(bc <<< "scale=2; ${bur[0]}/${bur[$i]}" 2>/dev/null))
  mem_s=$(printf '%3.2f\n' $(bc <<< "scale=2; ${mem[0]}/${mem[$i]}" 2>/dev/null))
  if [ $nat_s == "0.00" ]; then
    nat_s=""
  fi
  if [ $bur_s == "0.00" ]; then
    bur_s=""
  fi
  if [ $mem_s == "0.00" ]; then
    mem_s=""
  fi

  echo "${corel[$i]}:$nat_s:$bur_s:$mem_s" >> $result_file
  let i=$i+1
done

# Plotting graph
echo "Plotting the ${graph_file}..."
gnuplot -e "filename='${graph_file}';input_csv='${result_file}';graph_title='${APP//_/ } scalability results'" speedup.plot
echo "Displaying ${graph_file}..."
display ${graph_file}
