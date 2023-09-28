#!/usr/bin/env python3

import itertools
import argparse
import math
import numpy as np
import pandas as pd
VERBOSE = True

def get_mpiphase_from_file(presim_file, mode):
    dataframe = pd.read_csv(presim_file, header=None, comment='#', dtype=np.dtype(str),
                            skipinitialspace=True, delimiter=':')
    dataframe.columns = ['rank', 'phase_id', 'mode', 'x', 'value']

    # Rows cointaining a '1' at column 3 (starts at 0) are detail phases
    #return dataframe.loc[dataframe['mode'] == mode] ATTENTION THIS IS NO LONGER IN USE, ASK CESAR
    return dataframe

def print_detail_mpiphase_info(phases):
    print('\nFound {0} phases with detailed information:\n'.format(len(phases.index)))
    for index, row in phases.iterrows():
        print('\tPhase {0}: On rank={1} ID={2}, Value={3}'.format(index, row[0], row[1], row[4]))
    print('\n \n')

# Load Detail Base
# Load Detail NewArch
# ArchSlowdown =  (NewArch / freqNA) / (Base / freqB)
# Load Burst file
# For each b in bursts:
#     b = b * Slowdown.

def main():
    # Load Base Detail phases information.
    detail_base_file = args.file_detail_base[0]
    detail_base_phases = get_mpiphase_from_file(detail_base_file, '0') # ATTENTION THIS IS SET to 0 temporary!!!

    if VERBOSE:
        print('Reading File: {0}'.format(detail_base_file))
        print_detail_mpiphase_info(detail_base_phases)

    # Load NewArch detail phases information.
    detail_newarch_file = args.file_detail_newarch[0]
    detail_newarch_phases = get_mpiphase_from_file(detail_newarch_file, '0')

    if VERBOSE:
        print('Loaded {0} phases.'.format(len(detail_newarch_phases.index)))

    # JOIN: Base and New Arch
    join_df = pd.merge(detail_base_phases, detail_newarch_phases, how='inner', on=['rank', 'phase_id'],
                  suffixes=('_base', '_newarch'), copy=True, indicator=False)

    join_df['Arch_Slowdown'] = join_df['value_newarch'].astype(int) / join_df['value_base'].astype(int)

    print(join_df['Arch_Slowdown'])

    for index, row in join_df.iterrows():
            print('\t Phase {0}: ID={1}, Slowdown={2}'.format(index, row['phase_id'], row['Arch_Slowdown']))
    print('\n')

    # Specify the order pattern to apply the arch slowdown
    if args.pattern:
        pattern_input = args.pattern[0]
    else:
        pattern_input = input('Please select a cyclic pattern to apply to the PRESIM File (e.g: 0,1,1): ')
 
    pattern_phases = list(map(int, pattern_input.split(',')))
    arch_slowdown_list = [ join_df['Arch_Slowdown'].iloc[p] for p in pattern_phases ]

    # Apply changes
    # LOAD BURSTS FILE
    bursts_file = args.fileburst[0]
    burst_phases = get_mpiphase_from_file(bursts_file, '0')
    rank_values = pd.unique(burst_phases['rank'].values).astype(str)

    for r in rank_values:
        arch_slowdown_cycle = itertools.cycle(arch_slowdown_list)

        # Mutiply each burst value by a contention factor.
        for index in burst_phases.loc[burst_phases['rank'] == r].index.tolist():
            new_value = int(burst_phases['value'].loc[index]) * float(next(arch_slowdown_cycle))
            burst_phases.set_value(index,'value', int(round(new_value)))
            burst_phases.set_value(index,'mode', 3) # In presim files mode=3 means value extrapolation

    adjusted_filename = bursts_file.split('/')[-1]
    adjusted_filename = adjusted_filename.replace(".dat","_adjusted.dat")
    burst_phases.to_csv('{0}'.format(adjusted_filename), sep=':', index=False, header=False)

if __name__ == "__main__":

    # PARSE ARGUMENTS
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-db', '--file-detail-base', metavar='file-detail-base', type=str, nargs=1,
                        help='dat file with detail simulation timing that will be used as base.')

    parser.add_argument('-dn', '--file-detail-newarch', metavar='file-detail-newarch', type=str, nargs=1,
                        help='dat file with detail simulation timing of a new architecture.')

    parser.add_argument('-b','--fileburst', metavar='fileburst', type=str, nargs=1,
                        help='Path to the dat file with the aditional BURSTS.')

    parser.add_argument('-p','--pattern', metavar='pattern', type=str, nargs=1,
                        help='.')

    args = parser.parse_args()

    main()

