#!/bin/bash
#SBATCH --job-name=APPNAME_sve_simulate
#SBATCH --ntasks=16
#SBATCH --cpus-per-task=1
#SBATCH --out=./logs/simulations-%j.out
#SBATCH --err=./logs/simulations-%j.err
#SBATCH --time=4:00:00

module load musa/3.1

config_list="CONFIG_LIST"
app=APPNAME
args="APP_ARGUMENTS"
vector_list="SVE_REGISTER_SIZE_LIST"
corecount="1 2 4 8 16"
greasy_name="greasy_${app}_${args//\ /_}.txt"
config_folder=./configs

rm $greasy_name
mkdir -p ${config_folder}

for configuration in $config_list; do
    for cpus in $corecount; do
        trace_name=trace_${app//sve/scalar}_${args//\ /_}/${app//sve/scalar}.ts
        log_file=./logs/simulation_${app}_${configuration}_${cpus}cores_noscaling_scalar.txt
        new_config="./${config_folder}/${configuration%%\.conf}_${cpus}cores_noscaling_scalar.conf"
        l3size=$(echo "1024*1024*$cpus" | bc)
        l3mshrsize=$(echo "16*$cpus" | bc)
        sed -e "s/SIMULATED_VECTOR/128/g" \
            -e "s/TRACE_VECTOR/128/g" \
            -e "s/NUMBER_OF_CPUS/$cpus/g" \
            -e "s/L3CACHE_SIZE/$l3size/g" \
            -e "s/L3MSHR_SIZE/$l3mshrsize/g" \
            <$configuration >${new_config}
        echo "tasksim $new_config $trace_name &>$log_file" >> $greasy_name
    done

    for vector_length in $vector_list; do
        for cpus in $corecount; do
            trace_name=trace_${vector_length}_${app}_${args//\ /_}/${app}_${vector_length}.ts
            log_file=./logs/simulation_${app}_${configuration}_${cpus}cores_noscaling_vec_${vector_length}.txt
            new_config="./${config_folder}/${configuration%%\.conf}_${cpus}cores_noscaling_vec_${vector_length}.conf"
            l3size=$(echo "1024*1024*$cpus" | bc)
            l3mshrsize=$(echo "16*$cpus" | bc)
            sed -e "s/SIMULATED_VECTOR/$vector_length/g" \
                -e "s/TRACE_VECTOR/$vector_length/g" \
                -e "s/NUMBER_OF_CPUS/$cpus/g" \
                -e "s/L3CACHE_SIZE/$l3size/g" \
                -e "s/L3MSHR_SIZE/$l3mshrsize/g" \
                <$configuration >${new_config}
            echo "tasksim $new_config $trace_name &>$log_file" >> $greasy_name
        done
    done
done

greasy $greasy_name
