#!/bin/bash
export config_list="musa_sve_template_A.conf musa_sve_template_B.conf musa_sve_template_C.conf musa_sve_template_D.conf"
./extract_data_sve_thread.bash
mkdir -p pngs_sve_thread
mv *png pngs_sve_thread

./extract_data_sve_config.bash
mkdir -p pngs_sve_config
mv *png pngs_sve_config

./extract_data_thread_config.bash
mkdir -p pngs_thread_config
mv *png pngs_thread_config

./extract_data_thread_sve.bash
mkdir -p pngs_thread_sve
mv *png pngs_thread_sve
