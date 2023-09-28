#!/usr/bin/env python3
import os
import shutil
import subprocess
import string
import datetime
import pandas
import fnmatch
import argparse
import re
from multiprocessing import Pool

verbose = False

def get_tags_from_folder_name(folder_name):
    tags = []
    for field in folder_name.split('_'):
        tags.append(field.split('-'))

    tags.pop(0)
    return tags

def get_file_by_extension(folder_path, ext):
    simout_fname = ''
    for f in os.listdir(folder_path):
        if f.endswith(ext):
            simout_fname = f
    return simout_fname


def integrate(f, args):
    musa_base_folder        = os.environ['MUSA_HOME']
    ts_scripts_folder       = '{0}/01_src/tasksim/scripts/'.format(musa_base_folder)
    destination_folder      = args.destination_folder[0]
    detail_folder           = args.detail_folder[0]
    realpath_to_this_script = os.path.dirname(os.path.realpath(__file__))
    integration_script      = '{0}/design_space_exploration/03c_DimemasTS_integration.sh'.format(ts_scripts_folder)
    burstext_file_format    = 'ts_core-{0}_extrapolated.dat'
    app_name                = args.app[0]
    destructive             = True

    integration_greasy_file = 'integration_{0}.greasy'
    integration_cmd = ''

    timing_prv_cfg  = args.prvcfg[0]
    timing_out_file = 'timing_{0}cores_{1}.out'

    # Step 1 -- Create new architecture folder structure
    prefix_regex = 'ts_core-[0-9]*_'
    if not re.match(prefix_regex, f):
        # Skip folders that do not follow the format ts_core-XXX
        return ''

    num_cores = re.search('ts_core-(.+?)_', f).group(1)
    suffix = re.sub(prefix_regex, '', f)

    arch_folder = '{0}/{1}'.format(destination_folder, suffix)
    if not os.path.exists(arch_folder):
        os.makedirs(arch_folder)

    extrapolated_filepath = burstext_file_format.format(num_cores)
    prv_trace_filename = '{0}_{1}cores.prv'.format(suffix, num_cores)
    prm_output_filename = prv_trace_filename[:-4] + '_timing.txt'
    destruct_cmd = ''

    if destructive:
        destruct_cmd = ' && rm {0} {1}.pcf {1}.row'.format(prv_trace_filename, prv_trace_filename[:-4])

    integration_cmd += 'cd {0} && {1} {2} {3} {7} && paramedir {3} ../{4} {5} {6} \n'.format(arch_folder, integration_script,
                        extrapolated_filepath, prv_trace_filename, timing_prv_cfg, prm_output_filename, destruct_cmd, app_name)
    return integration_cmd

def process_folder(f, args):
    musa_base_folder        = os.environ['MUSA_HOME']
    ts_scripts_folder       = '{0}/01_src/tasksim/scripts/'.format(musa_base_folder)
    destination_folder      = args.destination_folder[0]
    detail_folder           = args.detail_folder[0]
    realpath_to_this_script = os.path.dirname(os.path.realpath(__file__))
    adjust_script           = '{0}/design_space_exploration/03a_adjust_presim_burst.py'.format(ts_scripts_folder)
    extrapolation_script    = '{0}/design_space_exploration/03b_extrapolate_detail_to_burst.py'.format(ts_scripts_folder)

    reference_arch_detail_file = args.reference_sim_dat[0]

    burst_sim_folder     = args.burst_folder[0]
    burst_file_format    = 'ts_core-{0}_burst.dat'
    burstadj_file_format = 'ts_core-{0}_burst_adjusted.dat'
    burstext_file_format = 'ts_core-{0}_extrapolated.dat'
    num_cores_tag        = 'core'
    application_pattern  = args.pattern[0]

    timing_prv_cfg  = args.app[0]
    timing_out_file = 'timing_{0}cores_{1}.out'

    #prv_filename_format = 'MUSA_hydro_{0}'

    do_adjust        = args.adjust
    do_extrapolation = args.extrapolate

    # Step 1 -- Create new architecture folder structure
    prefix_regex = 'ts_core-[0-9]*_'
    if not re.match(prefix_regex, f):
        # Skip folders that do not follow the format ts_core-XXX
        return

    num_cores = re.search('ts_core-(.+?)_', f).group(1)
    suffix = re.sub(prefix_regex, '', f)

    arch_folder = '{0}/{1}'.format(destination_folder, suffix)
    if not os.path.exists(arch_folder):
        os.makedirs(arch_folder)

    # Step 2 -- Run adjust
    burst_file                  = burst_file_format.format(num_cores)
    burst_filepath              = '{0}/{1}'.format(burst_sim_folder, burst_file)
    newarch_detail_001_folder   = re.sub('ts_core-[0-9]*_', 'ts_core-001_', f)
    newarch_detail_001_filepath = '{1}/{0}/{0}.dat'.format(newarch_detail_001_folder, detail_folder)

    print('Processing arch <{0}> using {1} cores'.format(suffix, num_cores))
    
    adj_filepath_exists = os.path.exists(os.path.join(arch_folder, burstadj_file_format.format(num_cores)))
    ext_filepath_exists = os.path.exists(os.path.join(arch_folder, burstext_file_format.format(num_cores)))

    if do_adjust and not adj_filepath_exists:
        cmd = 'cd {0} && {1} -db ../../{2} -dn ../../{3} -b ../../{4} -p {5}'.format(arch_folder,
                adjust_script, reference_arch_detail_file, newarch_detail_001_filepath,
                burst_filepath, application_pattern)
        try:
            res = subprocess.run(cmd, universal_newlines=True, shell=True, stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT, check=True)
        except subprocess.CalledProcessError as cpe:
            print('ERROR # Exception captured running ADJUST command')
            print('ERROR # cmd executed: {0}'.format(cpe.cmd))
            print('ERROR # Output: {0}'.format(cpe.output))
            exit(0)

        if verbose: print(res)
    else:
        if verbose:
            print('File exists. Skipping adjust')


    #Step 3 -- Extrapolate
    adjusted_filepath       = burstadj_file_format.format(num_cores)
    newarch_detail_folder   = f # re.sub('ts_core-[0-9]*_', 'ts_core-{:03d}_'.format(num_cores), f)
    newarch_detail_filepath = '{0}/{1}/{1}.dat'.format(detail_folder, newarch_detail_folder)

    if do_extrapolation and not ext_filepath_exists:
        if (int(num_cores) == 1):
            e_cmd = 'cd {0} && {1} -b {2} -m ../../{3} -p {4}'.format(arch_folder, extrapolation_script,
                    adjusted_filepath, newarch_detail_001_filepath, application_pattern)
        else:
            e_cmd = 'cd {0} && {1} -b {2} -m ../../{3} -p {4} -c correction.dat'.format(arch_folder,
                    extrapolation_script, adjusted_filepath, newarch_detail_filepath, application_pattern)

        try:
            res = subprocess.run(e_cmd, universal_newlines=True, shell=True, stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT, check=True)
        except subprocess.CalledProcessError as cpe:
            print('ERROR # Exception captured running EXTRAPOLATE command')
            print('ERROR # cmd executed: {0}'.format(cpe.cmd))
            print('ERROR # Output: {0}'.format(cpe.output))
            exit(0)

        if verbose: print(res)

    else:
        if verbose: print('File exists. Skipping extrapolate')


def main(args):
    musa_base_folder        = os.environ['MUSA_HOME']
    ts_scripts_folder       = '{0}/01_src/tasksim/scripts/'.format(musa_base_folder)
    destination_folder      = args.destination_folder[0]
    detail_folder           = args.detail_folder[0]
    realpath_to_this_script = os.path.dirname(os.path.realpath(__file__))
    adjust_script           = '{0}/design_space_exploration/03a_adjust_presim_burst.py'.format(ts_scripts_folder)
    extrapolation_script    = '{0}/design_space_exploration/03b_extrapolate_detail_to_burst.py'.format(ts_scripts_folder)
    integration_script      = '{0}/design_space_exploration/03c_DimemasTS_integration.sh'.format(ts_scripts_folder)

    reference_arch_detail_file = args.reference_sim_dat[0]

    burst_sim_folder     = args.burst_folder[0]
    burst_file_format    = 'ts_core-{0}_burst.dat'
    burstadj_file_format = 'ts_core-{0}_burst_adjusted.dat'
    burstext_file_format = 'ts_core-{0}_extrapolated.dat'
    num_cores_tag        = 'core'
    application_pattern  = args.pattern[0]

    timing_prv_cfg  = args.prvcfg[0]
    timing_out_file = 'timing_{0}cores_{1}.out'

    integration_greasy_file = 'integration_{0}.greasy'
    integration_cmd = ''
    #prv_filename_format = 'MUSA_hydro_{0}'

    multicore = True
    do_integration = args.integrate

    if not os.path.isdir(detail_folder):
        print('Simulation folder does not exists!')
        exit()

    if not os.path.exists(destination_folder):
        os.makedirs(destination_folder)

    if multicore:
        #Create (folder,args) tuple for each folder
        map_args = [(f,args) for f in sorted(os.listdir(detail_folder))]
        with Pool(6) as p:
            p.starmap(process_folder, map_args)
    else:
        for f in sorted(os.listdir(detail_folder)):
            process_folder(f, args)


    print('Preparing integration file...')
    if do_integration:
        for f in sorted(os.listdir(detail_folder)):
            integration_cmd += integrate(f, args)

        with open(integration_greasy_file.format(detail_folder), 'w+') as f:
            f.write(integration_cmd)

    print('OK.')
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='.')
    parser.add_argument('burst_folder', type=str, nargs=1, help='folder containing burst.dat files')
    parser.add_argument('detail_folder', type=str, nargs=1, help='folder containing all the detailed simulations')
    parser.add_argument('destination_folder', type=str, nargs=1, help='Where the output of this process will be placed')
    parser.add_argument('reference_sim_dat',  type=str, nargs=1)
    parser.add_argument('-p','--pattern', type=str, nargs=1)
    parser.add_argument('-a','--app', type=str, nargs=1)
    parser.add_argument('--prvcfg', type=str, nargs=1, help='Paraver config used by paramedir to cut and extract the timing from the trace.')

    #Actions to do
    parser.add_argument('--adjust', action='store_true')
    parser.add_argument('--integrate', action='store_true')
    parser.add_argument('--extrapolate', action='store_true')

    args = parser.parse_args()

    main(args)
