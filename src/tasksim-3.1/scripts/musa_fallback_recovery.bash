#!/bin/bash

function usage() {
  echo "Usage: musa_fallback_recovery.bash $SESSION_ID $STEP_ID."
  echo "Available sessions:"
  echo " - Session 1: MUSA Introduction and Burst mode."
  echo "   - step 1: Introduyction"
  echo "   - step 2: Enviroment setup"
  echo "   - step 3: Native run"
  echo "   - step 4: Burst mode trace generation"
  echo "   - step 5: Burst mode simulation"
  echo "   - step 6: Simulation integration and analysis"
  echo " - Session 2: MUSA memory mode."
}
# If this script is called, MUSA module is loaded.

# Check number of arguments:
if [ $# -eq 2 ]; then
  if [ $1 == "1" ]; then
  # First session
    if [ $2 == "1" ]; then
      echo "Recovering session $1 step $2"
      echo "Nothing to recover yet, just remember:"
    elif [ $2 == "2" ]; then
      echo "Recovering session $1 step $2"
      # It's easier to call the commands than copy anything
      musa_start.bash
      musa_load_tracing_example.bash
      echo "MUSA Initial setup done."
    elif [ $2 == "3" ]; then
      echo "Recovering session $1 step $2"
      musa_start.bash
      musa_load_tracing_example.bash
      # Copy native run results.
      cp ${MUSA_HOME}/tutorial_results/session1/native_logs . -r
      echo "Native benchmark results obtained."
    elif [ $2 == "4" ]; then
      echo "Recovering session $1 step $2"
      musa_start.bash
      musa_load_tracing_example.bash
      cp ${MUSA_HOME}/tutorial_results/session1/native_logs . -r
      # We copy only the generated trace, no further steps.
      if [ -e TRACE_sp-mz.B.4_000004_BRST ]; then
        rm -rf TRACE_sp-mz.B.4_000004_BRST
      fi
      cp ${MUSA_HOME}/tutorial_results/session1/burst_trace TRACE_sp-mz.B.4_000004_BRST -r
      echo "Burst mode trace generated."
    elif [ $2 == "5" ]; then
      echo "Recovering session $1 step $2"
      musa_start.bash
      musa_load_tracing_example.bash
      cp ${MUSA_HOME}/tutorial_results/session1/native_logs . -r
      if [ -e TRACE_sp-mz.B.4_000004_BRST ]; then
        rm -rf TRACE_sp-mz.B.4_000004_BRST
      fi
      cp ${MUSA_HOME}/tutorial_results/session1/burst_trace TRACE_sp-mz.B.4_000004_BRST -r
      rm ./TRACE_sp-mz.B.4_000004_BRST/SIMULATION/A1_PRESIM -r
      tar -xf ${MUSA_HOME}/tutorial_results/session1/presim_results.tar.gz
      mv A1_PRESIM TRACE_sp-mz.B.4_000004_BRST/SIMULATION/
      echo "Burst mode phase simulations finalized."
    elif [ $2 == "6" ]; then
      echo "Recovering session $1 step $2"
      musa_start.bash
      musa_load_tracing_example.bash
      cp ${MUSA_HOME}/tutorial_results/session1/native_logs . -r
      if [ -e TRACE_sp-mz.B.4_000004_BRST ]; then
        rm -rf TRACE_sp-mz.B.4_000004_BRST
      fi
      cp ${MUSA_HOME}/tutorial_results/session1/burst_trace TRACE_sp-mz.B.4_000004_BRST -r
      rm ./TRACE_sp-mz.B.4_000004_BRST/SIMULATION/A1_PRESIM -r
      tar -xf ${MUSA_HOME}/tutorial_results/session1/presim_results.tar.gz
      mv A1_PRESIM TRACE_sp-mz.B.4_000004_BRST/SIMULATION/
      rm ./TRACE_sp-mz.B.4_000004_BRST/SIMULATION/A2_INTEGRATION_PRESIM -r
      tar -xf ${MUSA_HOME}/tutorial_results/session1/integration_results.tar.gz
      mv A2_INTEGRATION_PRESIM ./TRACE_sp-mz.B.4_000004_BRST/SIMULATION/
      echo "Burst mode simulation integrated."
    else
      echo "Unknow Step $2 for Session $1."
      usage
    fi
  elif [ $1 == "2" ]; then
    # Fallback session 2
    if [ $2 == "1" ]; then
      # Memory trace generation
      tar -xf s2_memory_trace.tar.gz
      echo "Memory trace generated."
    elif [ $2 == "2" ]; then
      # Memory presim executed
      tar -xf s2_memory_trace.tar.gz
      cd TRACE_sp-mz.B.4_000004_MEMO/SIMULATION
      tar -xf s2_presim_results.tar.gz
      echo "Memory trace generated and presim generated."
    elif [ $2 == "3" ]; then
      # Memory integration
      tar -xf s2_memory_trace.tar.gz
      cd TRACE_sp-mz.B.4_000004_MEMO/SIMULATION
      tar -xf s2_presim_results.tar.gz
      cd A1_PRESIM
      tar -xf s2_extrapolation_results.tar.gz
      echo "Memory trace generated, presim generated, and presim results extrapolated."
    elif [ $2 == "4" ]; then
      # Result analysis
      tar -xf s2_memory_trace.tar.gz
      cd TRACE_sp-mz.B.4_000004_MEMO/SIMULATION
      tar -xf s2_presim_results.tar.gz
      cd A1_PRESIM
      tar -xf s2_extrapolation_results.tar.gz
      cd ../A2_INTEGRATION_PRESIM/trace_SIMULATED
      tar -xf s2_integration_results.tar.gz
      echo "Memory trace generated, presim generated, and presim results extrapolated and integrated."
    else
      echo "Unknow Step $2 for Session $1."
    fi
  else
    echo "Unknow Session $1."
    usage
  fi
else
  usage
fi
