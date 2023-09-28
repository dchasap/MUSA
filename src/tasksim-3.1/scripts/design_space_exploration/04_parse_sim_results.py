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
import numpy
import fnmatch
import mmap
import sys
import operator

verbose = False
ts_simout_dataframe_header = ['COUNTER', 'CORES', 'VALUE']

def normalize_power_and_perf_by_category(dataset, cat ):

    # Init
    perf_col        = '{}-speedup'.format(cat)
    power_col       = '{}-power'.format(cat)
    power_core_col  = '{}-core-power'.format(cat)
    power_cache_col = '{}-cache-power'.format(cat)
    power_mem_col   = '{}-mem-power'.format(cat)

    mod_dataset = dataset.copy()

    mod_dataset[perf_col]        = numpy.nan
    mod_dataset[power_col]       = numpy.nan
    mod_dataset[power_core_col]  = numpy.nan
    mod_dataset[power_cache_col] = numpy.nan
    mod_dataset[power_mem_col]   = numpy.nan

    # Sort (To be sure we operate with the same row when dividing full columns)
    mod_dataset = mod_dataset.sort_values(by=['folder-tag', 'core', 'burst-id'])
    
    # Get baseline values
    # Note baseline data has to be put in a vector so pandas doesnt get confused with the 
    # row indexes, not the best approach, but it works.
    category_values = sorted(pandas.unique(mod_dataset[cat]))
    baseline_value  = category_values[0]

    baseline_perf   = mod_dataset.loc[mod_dataset[cat] == baseline_value, 'execution-time'].tolist()
    baseline_power  = mod_dataset.loc[mod_dataset[cat] == baseline_value, 'total-node-power'].tolist()

    baseline_mcpat        = mod_dataset.loc[mod_dataset[cat] == baseline_value, 'mcpat-total-power'].tolist()
    baseline_core_power   = mod_dataset.loc[mod_dataset[cat] == baseline_value, 'mcpat-cores-power'].tolist()
    baseline_cache_power  = list(map(operator.sub, baseline_mcpat, baseline_core_power))
    baseline_mem_power    = mod_dataset.loc[mod_dataset[cat] == baseline_value, 'dram-total-avg-power'].tolist()
    
    for val in category_values:
        subset_perf  = mod_dataset.loc[mod_dataset[cat] == val, 'execution-time']
        subset_power = mod_dataset.loc[mod_dataset[cat] == val, 'total-node-power']

        subset_mcpat  = mod_dataset.loc[mod_dataset[cat] == val, 'mcpat-total-power']
        subset_core_power  = mod_dataset.loc[mod_dataset[cat] == val, 'mcpat-cores-power']
        subset_cache_power = subset_mcpat.astype(float) - subset_core_power.astype(float)
        subset_mem_power   = mod_dataset.loc[mod_dataset[cat] == val, 'dram-total-avg-power']

        mod_dataset.loc[mod_dataset[cat] == val, perf_col]  = baseline_perf / subset_perf.astype(float)
        mod_dataset.loc[mod_dataset[cat] == val, power_col] = subset_power.astype(float) / baseline_power

        mod_dataset.loc[mod_dataset[cat] == val, power_core_col]   = subset_core_power.astype(float) / baseline_power
        mod_dataset.loc[mod_dataset[cat] == val, power_cache_col]  = subset_cache_power.astype(float) / baseline_power
        mod_dataset.loc[mod_dataset[cat] == val, power_mem_col]    = subset_mem_power.astype(float) / baseline_power

    if verbose:
        print(mod_dataset.head(n=10))

    return mod_dataset

def get_tags_from_folder_name(folder_name):
    tags = []

    for field in folder_name.split('_'):
        tags.append(field.split('-'))

    tags.pop(0)
    return tags

def get_files_ending_with(folder_path, extension):
    filepath = []
    for fp in os.listdir(folder_path):
        if fp.endswith(extension):
            filepath.append(fp)
    return filepath

def get_simout_file_from_folder(folder_path):
    simout_filepath = ''
    for fp in os.listdir(folder_path):
        if fp.endswith(".simout"):
            simout_filepath = fp

    return simout_filepath

def get_files_by_pattern(path, pattern):
    files_that_match = []
    for file in os.listdir(path):
            if fnmatch.fnmatch(file, pattern):
                files_that_match.append(path + file)

    return files_that_match

def get_mcpat_infoblock(s, is_device):
    # Info in McPat follows the pattern:
    # <Name_of_the_metric> = <value>
    info = {}
    device_metrics = ['device-type', 'area', 'peak-dynamic', 'sub-leakage', 'sub-leakage-w-power',
                      'gate-leakage', 'runtime-dynamic' ]

    non_device_metrics = ['area', 'peak-power', 'total-leakage', 'peak-dynamic',
                          'sub-leakage', 'sub-leakage-w-power', 'gate-leakage', 'runtime-dynamic']

    tmp_line = str(s.readline(), 'utf-8')
    info['block-name'] = tmp_line.split(':')[0].strip()

    if is_device:
        for t in device_metrics:
            tmp_line = str(s.readline(), 'utf-8')
            info[t] = tmp_line.split('=')[1].strip()
    else:
        for t in non_device_metrics:
            tmp_line = str(s.readline(), 'utf-8')
            info[t] = tmp_line.split('=')[1].strip()
    return info

def parse_mcpat(powerfiles_path):
    mcpat_file = get_files_by_pattern(powerfiles_path, '*mcpat.out')

    if len(mcpat_file) == 0:
        if verbose: print('Error: McPat input file not found.')
        exit()

    f = open(mcpat_file[0])
    s = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)

    mcpat_block_name = ['Processor:', 'Total Cores', 'Total L2s', 'Total L3s']
    all_info_blocks = []

    for ib in mcpat_block_name:
        is_device = True
        if ib == 'Processor:': is_device = False

        s.seek(s.find(str.encode(ib)))
        proc_info = get_mcpat_infoblock(s, is_device)
        all_info_blocks.append(proc_info)

    return all_info_blocks

def parse_power_folders(root_folder, power_subfolder, df, csv_folder):
    # New columns to be added to the CSV

    df['dram-total-avg-power'] = numpy.nan
    df['dram-total-energy']    = numpy.nan
    df['mcpat-total-power']    = numpy.nan
    df['mcpat-cores-power']    = numpy.nan
    df['mcpat-l2-power']       = numpy.nan
    df['mcpat-l3-power']       = numpy.nan
    df['total-node-power']     = numpy.nan
        
    table = []
    prefix_regex = 'ts_core-[0-9]*_'
    drampower_pattern = '*results.txt'
    all_power=[]
    all_power_mcpat=[]
    folders_processed = 0

    for f in sorted(os.listdir(root_folder)):
        if not re.match(prefix_regex, f):
            continue

        tags = get_tags_from_folder_name(f)
        sim_folderpath = os.path.join(root_folder, f)
        
        print('Checking folder: ', sim_folderpath)
        for burst in sorted(os.listdir(sim_folderpath)):
            burst_folderpath = os.path.join(sim_folderpath, burst)
            
            if not os.path.isdir(burst_folderpath):
                continue

            power_folder = os.path.join(burst_folderpath, power_subfolder)

            if not os.path.isdir(power_folder):
                continue

            print('Looking inside: {0}'.format(power_folder))
            drampower_files = get_files_by_pattern(power_folder, drampower_pattern)

            if not drampower_files:
                print('[ DRAM ] Skipping {0}'.format(burst))
                continue

            if verbose:
                print(' {0} file(s) found.'.format(len(drampower_files)))

            list_rank_power = []
            list_rank_energy = []

            try:
                ''' For every DRAM Power 'cmdtrace' file
                     - Open the file
                     - Get total energy and power
                     - Add values to a vector
                '''

                for drp_file in drampower_files:
                    dramfile = open(drp_file)
                    s = mmap.mmap(dramfile.fileno(), 0, access=mmap.ACCESS_READ)

                    s.seek(s.find(str.encode('Total Trace Energy:')) + 20)
                    total_trace_energy = s.readline()[:-4]
                    list_rank_energy.append(float (total_trace_energy))

                    s.seek(s.find(str.encode('Average Power:')) + 15)
                    tmp_line = s.readline()[:-4]
                    list_rank_power.append(float (tmp_line) / 1000)

                    if verbose: print(tmp_line)
                    dramfile.close()

                # Insert DRAMPower info into dataframe

                df.loc[(df['burst-id'] == int(burst)) & (df['folder-tag'] == f), 'dram-total-avg-power'] = numpy.sum(list_rank_power)
                df.loc[(df['burst-id'] == int(burst)) & (df['folder-tag'] == f), 'dram-total-energy']    = numpy.sum(list_rank_energy)

            except ValueError as err:
                print('value error on dram: {0}'.format(burst_folderpath))
                continue
            except KeyError as kerr:
                print('key error on dram {0}'.format(burst_folderpath))
                continue

            print('[ DRAM ] OK {0}'.format(burst))

            try:

                mcpatinfo = parse_mcpat(power_folder)

                proc_pwr = float(mcpatinfo[0]['sub-leakage'][:-2]) + float(mcpatinfo[0]['gate-leakage'][:-2]) + float(mcpatinfo[0]['runtime-dynamic'][:-2])
                df.loc[(df['burst-id'] == int(burst)) & (df['folder-tag'] == f), 'mcpat-total-power'] = proc_pwr

                core_pwr = float(mcpatinfo[1]['sub-leakage'][:-2]) + float(mcpatinfo[1]['gate-leakage'][:-2]) + float(mcpatinfo[1]['runtime-dynamic'][:-2])
                df.loc[(df['burst-id'] == int(burst)) & (df['folder-tag'] == f), 'mcpat-cores-power'] = core_pwr 

                l2_pwr = float(mcpatinfo[2]['sub-leakage'][:-2]) + float(mcpatinfo[2]['gate-leakage'][:-2]) + float(mcpatinfo[2]['runtime-dynamic'][:-2])
                df.loc[(df['burst-id'] == int(burst)) & (df['folder-tag'] == f), 'mcpat-l2-power'] = l2_pwr
                
                l3_pwr = float(mcpatinfo[3]['sub-leakage'][:-2]) + float(mcpatinfo[3]['gate-leakage'][:-2]) + float(mcpatinfo[3]['runtime-dynamic'][:-2])
                df.loc[(df['burst-id'] == int(burst)) & (df['folder-tag'] == f), 'mcpat-l3-power'] = l3_pwr
                
            except ValueError as err:
                print('value error on mcpat')
                exit
            except KeyError as kerr:
                print('key error on mcpat')
                exit
                
           
            print('[ MCPAT ] OK {0}'.format(burst))
 
            folders_processed += 1

    # POST: Calculated fields
    # 1. Total node power
    df['total-node-power'] = df['dram-total-avg-power'] + df['mcpat-total-power']        

    # 2. Normalized power to lowest
    min_power = df['total-node-power'].min()
    df['norm-node-power'] = df['total-node-power'].astype(float) / float(min_power)

    print('Folders processed: {0}'.format(folders_processed))


    df.to_csv(csv_folder + '/allsim_results_and_power.csv', sep=',', index=False,
             header=True, float_format='%.5f', na_rep='nan')

def parse_tasksim_output(sim_file, num_cores=1):
    rows = []

    # Why I do this: TaskSim sim output is a bit messy. Also, modules may change so
    # we cannot assume a fixed order. We have to check line by line if its
    # a Tasksim 'counter' of some sort.

    with open(sim_file, 'r') as f:
        for line in f:
            line_fields = line.split(':')
            num_fields = len(line_fields)
            tmp_row = []

            if num_fields == 2:
                if line_fields[0] == 'Final cycle count':
                    tmp_row = ['cycle-count', num_cores, line_fields[1].strip()]
            elif num_fields == 4:
                counter_tag = line_fields[1]
                if line_fields[2] == 'ROA':
                    value = int(line_fields[0].strip())

                    if counter_tag == 'ROA number of cycles spent waiting for store responses':
                        tmp_row = ['roa-mem-stall-cycles', num_cores, value ]
                    elif counter_tag == 'ROA number of cycles spent waiting for load responses':
                        tmp_row = ['roa-mem-stall-cycles', num_cores, value ]
                    elif counter_tag == 'ROA total number of stalled cycles':
                        tmp_row = ['roa-total-stall-cycles', num_cores, value ]
                    elif counter_tag == 'ROA total number of committed instructions':
                        tmp_row = ['roa-total-instructions', num_cores, value ]
                if line_fields[2] == 'CacheL1':
                    value = int(line_fields[0].strip())

                    if counter_tag in ['write_miss', 'write_half_miss', 'read_half_miss', 'read_miss']:
                        tmp_row = ['l1-miss', num_cores, value]
                    elif counter_tag in ['write_hit', 'read_hit']:
                        tmp_row = ['l1-hit', num_cores, value]
                if line_fields[2] == 'CacheL2':
                    value = int(line_fields[0].strip())

                    if counter_tag in ['write_miss', 'write_half_miss', 'read_half_miss', 'read_miss']:
                        tmp_row = ['l2-miss', num_cores, value]
                    if counter_tag in ['write_hit', 'read_hit']:
                        tmp_row = ['l2-hit', num_cores, value]

                if line_fields[2] == 'CacheL3':
                    value = int(line_fields[0].strip())

                    if counter_tag in ['write_miss', 'write_half_miss', 'read_half_miss', 'read_miss']:
                        tmp_row = ['l3-miss', num_cores, value]
                    if counter_tag in ['write_hit', 'read_hit']:
                        tmp_row = ['l3-hit', num_cores, value]
                if line_fields[2] == 'RamulatorController':
                    if counter_tag == 'Ramulator total request count':
                        value = int(line_fields[0].strip())
                        tmp_row = ['ramulator-total-requests', num_cores, value]

            elif num_fields == 5:
                if line_fields[3] == 'Memory':
                    if line_fields[2] == 'Memory CPU store size histogram':
                        value = int(line_fields[0].strip())
                        st_size = line_fields[1]
                        if st_size == 'underflows':
                            tmp_row = ['mem-st-size-uflow', num_cores, value]
                        elif st_size == '0 to 4':
                            tmp_row = ['mem-st-size-0', num_cores, value]
                        elif st_size == '4 to 8':
                            tmp_row = ['mem-st-size-4', num_cores, value]
                        elif st_size == '8 to 12':
                            tmp_row = ['mem-st-size-8', num_cores, value]
            else:
                if verbose:
                    print("Cannot be classified: {0}".format(line))

            if (tmp_row):
                    rows.append(tmp_row)

    tmp_df = pandas.DataFrame(rows, columns=ts_simout_dataframe_header)
    tmp_df = tmp_df.groupby(['COUNTER'], as_index=False)['VALUE'].sum()

    return tmp_df

def add_ts_simout_results(row, df, counters):
    for c in counters:
        row.extend(df.loc[df['COUNTER'] == c]['VALUE'])

def parse_simout(root_folder):
    prefix_regex = 'ts_core-[0-9]*_'
    dataframe_header = ['folder-tag', 'suffix','burst-id']
    header_extension = ['l1-hit','l1-miss', 'l2-hit', 'l2-miss', 'l3-hit', 'l3-miss',
                        'roa-total-stall-cycles','roa-total-instructions',
                        'ramulator-total-requests']

    # Obtain the header column names using the first folder available
    # Given all folder should have the same tags with different values
    sample_tags = get_tags_from_folder_name((os.listdir(root_folder))[0])
    dataframe_header = dataframe_header + [st[0] for st in sample_tags] + ['total-cycles'] + header_extension

    if verbose:
        print(dataframe_header)

    table = []
    '''
        Traverses all folders inside root_folder looking for tasksim out files to parse.

        * For each folder containing a simulation
            + Reads the Tasksim output file
            + Adds a row containing the relevant data to a table
        * The resulting table containing info of all bursts is converted to a dataframe and then to CSV
    '''

    for f in sorted(os.listdir(root_folder)):
        if not re.match(prefix_regex, f):
            continue

        tags = get_tags_from_folder_name(f)
        suffix = re.sub(prefix_regex, '', f).replace('_','-')

        for burst in os.listdir('{0}/{1}'.format(root_folder, f)):
            if not os.path.isdir(os.path.join(root_folder, f, burst)):
                continue

            burst_folderpath = '{0}/{1}/{2}'.format(root_folder, f, burst)
            simout_filepath = get_simout_file_from_folder(burst_folderpath)

            if (simout_filepath):
                #print('Parsing architecture:burst {0}:{1}'.format(suffix, burst))
                df = parse_tasksim_output(burst_folderpath + '/' + simout_filepath)
                total_cycles = df.loc[df['COUNTER'] == 'cycle-count']['VALUE']

                row = [f, suffix, burst]
                row.extend([t[1] for t in tags])
                row.extend(total_cycles)
                add_ts_simout_results(row, df, header_extension )

                table.append(row)
            else:
                print('Skipping architecture:burst {0}:{1}'.format(suffix, burst))

    # Convert temporal python matrix to a Pandas dataframe.
    sim_results = pandas.DataFrame(table, columns=dataframe_header, dtype=int)
    sim_results = sim_results.sort_values(by=['folder-tag', 'burst-id' , 'core'])

    # POST PARSE 
    # Add calculated field 'execution time', 'speedup', 
    sim_results['execution-time'] = sim_results['total-cycles'].astype(float) / (sim_results['freq'].astype(float) * 1000000000)
    sim_results['speedup'] = numpy.nan
    sim_results['l1-mpki'] = (sim_results['l1-miss'].astype(float) * 1000) / sim_results['roa-total-instructions'].astype(float)
    sim_results['l2-mpki'] = (sim_results['l2-miss'].astype(float) * 1000) / sim_results['roa-total-instructions'].astype(float)
    sim_results['l3-mpki'] = (sim_results['l3-miss'].astype(float) * 1000) / sim_results['roa-total-instructions'].astype(float)
    sim_results['mem-access-rate'] = sim_results['ramulator-total-requests'].astype(float) / sim_results['execution-time'].astype(float)
    sim_results['stall-ratio'] = (sim_results['roa-total-stall-cycles'].astype(float) / sim_results['total-cycles'].astype(float))  / sim_results['core'].astype(float)

    for b in pandas.unique(sim_results['burst-id']):
        max_exe_time = sim_results.loc[sim_results['burst-id'] == b, 'execution-time' ].max()
        print(max_exe_time)
        sim_results.loc[sim_results['burst-id'] == b, 'speedup'] = float(max_exe_time) / sim_results.loc[sim_results['burst-id'] == b, 'execution-time' ].astype(float)

    # Write Tasksim simulation results to CSV
    sim_results.to_csv(root_folder + '/csv/allsim_results.csv', sep=',', index=False,
                             header=True, float_format='%.4f', na_rep='nan')

def parse_paramedir_output(paramedir_file):
    tmp_row = []
    max_value = 0
    with open(paramedir_file, 'r') as f:
        for line in f:
            line_fields = line.split('\t')
            if line_fields[0] == 'Maximum':
                max_value = line_fields[1]

    return max_value

def parse_prv_timings(root_folder):
    dataframe_header = ['folder-tag', 'num-cores']

    # Extract the header column names using the first folder available
    # All folder should have the same tags with different values
    sample_tags = []
    prefix_regex = '[a-z]*-[a-z]*_'

    for x in sorted(os.listdir(root_folder)):
        path = os.path.join(root_folder, x)
        if os.path.isdir(path):
            if re.match(prefix_regex, x):
                sample_tags = get_tags_from_folder_name(x)

    dataframe_header = dataframe_header + [st[0] for st in sample_tags]
    dataframe_header = dataframe_header + ['parallel-region-time']

    table = []
    for arch_folder in os.listdir(root_folder):

        if not os.path.isdir(os.path.join(root_folder, arch_folder)):
            if not re.match(prefix_regex, arch_folder):
                continue

        tags = get_tags_from_folder_name(arch_folder)

        arch_folderpath = '{0}/{1}'.format(root_folder, arch_folder)
        timings = get_files_ending_with(arch_folderpath, 'timing.txt')

        for t in timings:
            t_filepath = os.path.join(arch_folderpath, t)
            max_value = parse_paramedir_output(t_filepath)
            str_cores = re.search('[0-9]+cores', t).group(0)
            num_cores = int(str_cores.replace('cores',''))
            row = [arch_folder.replace('_','-'), num_cores]
            row.extend([t[1] for t in tags])
            row.extend([max_value])
            table.append(row)

    all_integrated_data = pandas.DataFrame(table, columns=dataframe_header)
    all_integrated_data = all_integrated_data.sort_values(by=['folder-tag', 'num-cores'])
    print(all_integrated_data)
    all_integrated_data.to_csv(root_folder + '/all_integrated_info.csv', sep=',', index=False,
                               header=True, float_format='%.0f', na_rep='nan')


def normalized_average_stats(df, categories):

    stats_header = ['burst-id', 'core', 'category', 'category-value', 'speedup', 'speedup-std',
                    'total-power', 'total-power-std', 'core-power', 'cache-power', 'mem-power',
                    'speedup-base', 'speedup-base-std', 'total-power-base', 'total-power-base-std']
    
    df = df.sort_values(by=['folder-tag', 'core', 'burst-id'])
    burst_values    = sorted(pandas.unique(df['burst-id']))
    core_values     = sorted(pandas.unique(df['core']))

    table = []

    for cat in categories:
        perf_col        = '{}-speedup'.format(cat)
        power_col       = '{}-power'.format(cat)
        power_core_col  = '{}-core-power'.format(cat)
        power_cache_col = '{}-cache-power'.format(cat)
        power_mem_col   = '{}-mem-power'.format(cat)
        speedup_base_col= 'speedup'
        power_base_col  = 'norm-node-power'

        category_values = sorted(pandas.unique(df[cat]))
        
        for c in core_values:
            for val in category_values:
                for b in burst_values:

                    # Filter dataframe to select only rows with category==val and burstid==b
                    filtered_df = []
                    filtered_df = df.loc[(df[cat] == val) & (df['burst-id'] == b) & (df['core'] == c)]

                    speedup     = filtered_df[perf_col].mean(skipna=True)
                    total_power = filtered_df[power_col].mean(skipna=True)
                    core_power  = filtered_df[power_core_col].mean(skipna=True)
                    cache_power = filtered_df[power_cache_col].mean(skipna=True)
                    mem_power   = filtered_df[power_mem_col].mean(skipna=True)

                    speedup_std     = filtered_df[perf_col].std(skipna=True)
                    total_power_std = filtered_df[power_col].std(skipna=True)

                    # Grouped by category but normalized to slowest
                    speedup_base     = filtered_df[speedup_base_col].mean(skipna=True)
                    total_power_base = filtered_df[power_base_col].mean(skipna=True)

                    speedup_base_std     = filtered_df[speedup_base_col].std(skipna=True)
                    total_power_base_std = filtered_df[power_base_col].std(skipna=True)

                    row = [b, c, cat, val, speedup, speedup_std, total_power, total_power_std,
                           core_power, cache_power, mem_power, speedup_base, speedup_base_std, 
                           total_power_base, total_power_base_std ]

                    table.append(row)


    stats_df = pandas.DataFrame(table, columns=stats_header, copy=True)
    
    if verbose:
        print(stats_df)

    return stats_df


def main(args):
    root_folder = args.root_folder[0]
    csv_folder = os.path.join(root_folder, 'csv')

    print('CSVs will be placed at: {0}'.format(csv_folder))
    if not os.path.isdir(csv_folder):
        os.makedirs(csv_folder)
        
    do_ts_simout = args.tasksim
    do_prv_timings = args.paraver
    do_power = args.power
    do_normalize = args.normalize

    if (do_ts_simout):
        parse_simout(root_folder)

    if (do_prv_timings):
        parse_prv_timings(root_folder)

    if (do_power):
        allsim_results = os.path.join(csv_folder, 'allsim_results.csv')
        input_dataframe = pandas.read_csv(allsim_results, header=0, comment='#', dtype=None, skipinitialspace=True)

        powerfiles_subfolder = 'all_power_results/'
        
        parse_power_folders(root_folder, powerfiles_subfolder, input_dataframe, csv_folder)

    if (do_normalize):

        categories = ['coretype', 'cache', 'freq', 'vec', 'mem']

        # 1 LOAD
        sim_results_w_power = os.path.join(csv_folder, 'allsim_results_and_power.csv')
        df_to_normalize = pandas.read_csv(sim_results_w_power, header=0, comment='#', dtype=None, skipinitialspace=True)


        # 2 Add normalized columns and output _normalized.csv
        for cat in categories:
            df_to_normalize = normalize_power_and_perf_by_category(df_to_normalize, cat)

        all_sim_norm = os.path.join(csv_folder, 'allsim_results_power_and_normalized.csv')
        df_to_normalize.to_csv(all_sim_norm, sep=',', index=False, header=True, 
                                float_format='%.5f', na_rep='nan')
        print('Generated {0}'.format(all_sim_norm))

        # 3 Generate stats for normalized columns and output _normalized_stats.csv
        df_stats = normalized_average_stats(df_to_normalize, categories)
        all_sim_norm_stats = os.path.join(csv_folder, 'allsim_results_power_and_normalized_stats.csv')
        df_stats.to_csv(all_sim_norm_stats, sep=',', index=False,
                        header=True, float_format='%.5f', na_rep='nan')
        print('Generated {0}'.format(all_sim_norm_stats))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Parses Tasksim output files and translates to csv format.')
    parser.add_argument('root_folder', metavar='RootFolder', type=str, nargs=1,
                        help='Path to the CSV file with the input data.')

    parser.add_argument('--tasksim', action='store_true')
    parser.add_argument('--paraver', action='store_true')
    parser.add_argument('--power', action='store_true')
    parser.add_argument('--normalize', action='store_true')

    args = parser.parse_args()

    main(args)
