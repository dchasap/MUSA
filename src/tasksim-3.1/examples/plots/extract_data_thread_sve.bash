#!/bin/bash
scenario_list="noscaling"
vector_list="128 256 512 1024 2048"
corelist="1 2 4 8 16"
time_file="total_execution_time$sufix.csv"
request_file="total_dram_requests$sufix.csv"
bw_file="simulation_bandwidth$sufix.csv"
speedup_file="simulation_speedup$sufix.csv"
configA="musa_sve_template_A.conf"
for vec in $vector_list; do
    sufix="_thread_SVE_"${vec}
    scenario="noscaling"
    result="Data"
    col=1
    for config in $config_list; do
        config_name=${config/\.conf/}
        config_name=${config_name/musa_sve_template_/}
        result="${result}:${config_name} SVE $vec bits"
        let col=$col+1
    done

    echo $result > $time_file
    echo $result > $request_file
    echo $result > $bw_file
    echo $result > $speedup_file
    for cpus in $corelist; do
        result="${cpus}"
        req_result="${cpus}"
        bw_result="${cpus}"
        speedup_result="${cpus}"
        normalized_result="${vec}"
        for config in $config_list; do
            log_file=./logs/simulation_iso_sve_ompss_${config}_${cpus}cores_${scenario}_vec_${vec}.txt
            ref_log_file=./logs/simulation_iso_sve_ompss_${config}_1cores_${scenario}_vec_${vec}.txt
            sim_ref_log_file=./logs/simulation_iso_sve_ompss_${configA}_1cores_${scenario}_vec_128.txt
            line=$(grep "Final cycle count:" $log_file)
            timeval=${line/Final\ cycle\ count:\ /}
            ref_line=$(grep "Final cycle count:" $ref_log_file)
            ref_timeval=${ref_line/Final\ cycle\ count:\ /}
            line=$(grep "Final cycle count:" $sim_ref_log_file)
            sim_ref_timeval=${line/Final\ cycle\ count:\ /}
            line=$(grep ":Request count:RamulatorController:0" $log_file)
            requests=${line/:Request count:RamulatorController:0/}
            bw=$(echo "scale=6;$requests*64.0/($timeval/2.0)" | bc | awk '{printf "%f", $0}')

            result="$result:${timeval}"
            req_result="$req_result:${requests}"
            bw_result="$bw_result:$bw"
            speedup_ratio=$(echo "scale=18; $ref_timeval/$timeval" | bc)
            speedup_result="${speedup_result}:${speedup_ratio}"
            sim_ratio=$(echo "scale=18; $timeval/$sim_ref_timeval" | bc)
            normalized_result="${normalized_result}:${sim_ratio}"
        done
        #echo $result >> $time_file
        echo $normalized_result >> $time_file
        echo ${req_result} >> $request_file
        echo ${bw_result} >> $bw_file
        echo ${speedup_result} >> $speedup_file
    done
    bw_plottitle="Simulation bandwidth for SVE Register size ${vec} bits [GB/s]"
    sim_plottitle="Total executed time for SVE Register size ${vec} bits [normalized]\n{/*0.8 (normalized to configuration A 1 core SVE 128 bits)}"
    gnuplot -e "columns=$col;filename='bandwidth${sufix}.png';input_csv='${bw_file}';plottitle='${bw_plottitle}'" bw_t.plot
    gnuplot -e "columns=$col;filename='requests${sufix}.png';input_csv='${request_file}'" requests_t.plot
    gnuplot -e "columns=$col;filename='simulation_time${sufix}.png';input_csv='${time_file}';plottitle=\"${sim_plottitle}\"" sim_time_t.plot
    gnuplot -e "columns=$col;filename='speedup${sufix}.png';input_csv='${speedup_file}'" speedup_t.plot
done
