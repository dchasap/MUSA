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


def get_files_by_pattern(path, pattern):
    files_that_match = []
    for file in os.listdir(path):
            if fnmatch.fnmatch(file, pattern):
                files_that_match.append(path + file)

    return files_that_match

def get_file_by_extension(folder_path, ext):
    fname = ''
    for f in os.listdir(folder_path):
        if f.endswith(ext):
            fname = f
    return fname

def get_files_ending_with(folder_path, extension):
    filepath = []
    for fp in os.listdir(folder_path):
        if fp.endswith(extension):
            filepath.append(fp)
    return filepath


def main(args):
    root_folder             = args.root_folder[0]
    realpath_to_this_script = os.path.dirname(os.path.realpath(__file__))
    #run_model_script        = '{0}/{1}'.format(realpath_to_this_script, 'run_power_models.sh')
    drampower_dir           = '/home/bsc18/bsc18880/phd/musa/projects/soft/DRAMPower-4.0'
    mcpat_dir               = '/home/bsc18/bsc18880/phd/musa/projects/soft/gem5-mcpat'
    ts_to_mcpat             = '/home/bsc18/bsc18880/phd/musa/projects/soft/gem5-mcpat/gem5-mcpat'
    mcpat_exe               = '/home/bsc18/bsc18880/phd/musa/projects/soft/gem5-mcpat/mcpat/mcpat'
    template                = '{0}/gem5_mcpat/tasksim_templates/example-arm-l3.xml'.format(mcpat_dir)

    prefix_regex            = 'ts_core-[0-9]*_'
    mcpat_greasy_filename   = 'mcpat_cmd_{0}.greasy'.format(root_folder.replace('/',''))
    drampower_greasy_filename = 'drampower_cmd_{0}.greasy'.format(root_folder.replace('/',''))
    mcpat_greasy_file       = ''
    drampower_greasy_file   = ''
    power_folder_name       = 'all_power_results'

    do_mcpat     = args.mcpat
    do_drampower = args.drampower
    destruct_cmdtrace_on_read = True

    avoided_folder_count = 0
    invalid_simout_count = 0

    for f in sorted(os.listdir(root_folder)):

        if not re.match(prefix_regex, f):
            continue

        sim_folderpath = '{0}/{1}'.format(root_folder, f)
        conf_filename = get_file_by_extension(sim_folderpath, ".conf")

        for burst in os.listdir(sim_folderpath):

            if os.path.isdir(os.path.join(sim_folderpath, burst)):
                burst_folderpath = '{0}/{1}'.format(sim_folderpath, burst)

                local_power_folder = os.path.join(burst_folderpath, power_folder_name)
                if not os.path.exists(local_power_folder):
                    os.makedirs(local_power_folder)

                if do_mcpat:
                    ts2mcpat_output_file = '{0}/mcpat_power_numbers.input'.format(power_folder_name)
                    mcpat_results        = '{0}/results_sim_mcpat.out'.format(power_folder_name)
                    simout_filepath      = get_file_by_extension(burst_folderpath, ".simout")

                    tasksim_to_mcpat_cmd = 'cd {0} && {1} generate_tasksim ../{2} {3} {4} --set-variable cores_name=system.cpu --set-variable technology=22 > {5}'.format(burst_folderpath, ts_to_mcpat, conf_filename, simout_filepath, template, ts2mcpat_output_file )
                    mcpat_cmd = '{0} -infile {1} -print_level 1 > {2}'.format(mcpat_exe, ts2mcpat_output_file, mcpat_results)

                    if (simout_filepath):
                        #cmd = 'cd {0} && {1} {2} ../{3} \n'.format(burst_folderpath, run_model_script, simout_filepath, conf_filename)
                        cmd = '{0} && {1} \n'.format(tasksim_to_mcpat_cmd, mcpat_cmd)
                        mcpat_greasy_file += cmd
                        #res = subprocess.check_output(cmd, universal_newlines=True, shell=True)
                    else:
                        print("Simout not found. Skipping directory {0}".format(burst_folderpath))
                        avoided_folder_count += 1

                if do_drampower:

                    valid_folder = True
                    num_ranks = 2
                    num_chan  = 0

                    if len(get_files_ending_with(burst_folderpath, "cmdtrace")):

                        search = re.search('mem-.ch(.+?)_', f)
                        if search:
                            mem_type = search.group(1)

                            search_ch = re.search('mem-(.+?)ch', f)
                            num_chan = int(search_ch.group(1))

                            if  mem_type == 'DDR4':
                                mem_model_file = '{0}/memspecs/MICRON_8GB_DDR4-2400_ECC_SR_64bit_B_RDIMM.xml'.format(drampower_dir)
                            elif mem_type == 'DDR3':
                                mem_model_file = '{0}/memspecs/MICRON_1Gb_DDR3-1600_8bit_G.xml'.format(drampower_dir)
                            else:
                                print('Wrong memtype {0}'.format(mem_type))
                                valid_folder = False
                        else:
                            print('Couldnt find memtype {0}'.format(mem_type))
                            valid_folder = False

                    else:
                        print("This folder does not contain .cmdtrace files")
                        valid_folder = False

                    if valid_folder:
                        for c in range(num_chan):
                            for r in range(num_ranks):
                                destroy_cmd = ''
                                chan_rank   = 'chan-{0}-rank-{1}'.format(c, r)

                                if os.path.isfile('{0}/cmd-trace-{1}.cmdtrace'.format(burst_folderpath, chan_rank)):
                                    if (destruct_cmdtrace_on_read):
                                        destroy_cmd = '&& rm cmd-trace-{0}.cmdtrace'.format(chan_rank)

                                    cmd = 'cd {0} && {1}/drampower -m {2} -c cmd-trace-{3}.cmdtrace > {4}/ram-power-{3}-results.txt {5}\n'.format(burst_folderpath, drampower_dir,
                                        mem_model_file, chan_rank, power_folder_name, destroy_cmd)

                                    drampower_greasy_file += cmd
                                else:
                                    print("file doesnt exists.")
                    else:
                        avoided_folder_count += 1

    print('Total folders: {0}'.format(len(os.listdir(root_folder))))
    print('We avoided {0} folders'.format(avoided_folder_count))
    print('Empty simout files: {0}'.format(invalid_simout_count))

    if do_mcpat:

        with open(mcpat_greasy_filename, 'w+') as f:
            f.write(mcpat_greasy_file)

    if do_drampower:

        with open(drampower_greasy_filename, 'w+') as f:
            f.write(drampower_greasy_file)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Parses Tasksim output files and translates to csv format.')
    parser.add_argument('root_folder', metavar='RootFolder', type=str, nargs=1,
                        help='Path to the CSV file with the input data.')
    parser.add_argument('--drampower', action='store_true')
    parser.add_argument('--mcpat', action='store_true')
    args = parser.parse_args()

    main(args)
