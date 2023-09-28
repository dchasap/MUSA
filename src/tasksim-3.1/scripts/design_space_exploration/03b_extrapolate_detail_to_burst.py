#!/usr/bin/env python3

#import math
#import os
import itertools
import shutil
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


def main(args):
    # USER INPUT: Correction factor
    # (obtained dividing: Detail / Burst @ 1 Thread, Should be >2.6 in marenostrum )
    #correction_factor = float(input('INPUT - Correction factor (~frequency): '))

    # Get Detail phases information.
    presim_memory_file = args.filemem[0]
    detail_phases = get_mpiphase_from_file(presim_memory_file, '0') # ATTENTION THIS IS SET to 0 temporary!!!

    if VERBOSE:
        print('Reading File: {0}'.format(presim_memory_file))
        print_detail_mpiphase_info(detail_phases)

    # Get Burst phases information.
    presim_burst_file = args.fileburst[0]
    burst_values = get_mpiphase_from_file(presim_burst_file, '0')

    if VERBOSE:
        print('Loaded {0} phases with basic burst information.'.format(len(burst_values.index)))
        print('(Not shown)')

    # JOIN: burst and detail phase info by Phase ID
    join_df = pd.merge(detail_phases, burst_values, how='inner', on=['rank', 'phase_id'],
                  suffixes=('_detail', '_burst'), copy=True, indicator=False)

    join_df['detail_to_burst_ratio'] = join_df['value_detail'].astype(int) / join_df['value_burst'].astype(int)


    correction_factor = []
    if args.filecorrection:
        f = open(args.filecorrection[0], 'r')
        line = f.readline()
        #print(line)
        correction_factor = [float(i) for i in line.split(',')]

    else:
        to_file = ','.join(join_df['detail_to_burst_ratio'].astype(str).tolist())
        print(to_file)
        f = open('correction.dat', 'w')
        f.write(to_file+'\n')
        f.close()



    #for index, row in join_df.iterrows():
        #correction_factor.append(float(input('INPUT - Correction factor for Burst {0} (~frequency): '.format(index))))

    #print(correction_factor)

    if correction_factor:
        join_df['correction_factor'] = correction_factor
        # For each Phase, compute its contention factor.
        print('\n\nCorrection factor is SET to {0}'.format(correction_factor))
        print('CONTENTION factor (s.t. Contention = value_detail / (value_burst * correction factor))')

        join_df['contention_factor'] =  join_df['value_detail'].astype(float) / ( join_df['value_burst'].astype(float) * correction_factor )

    if VERBOSE:
        print('\n')
        print(join_df)

    print('\n\n\n')
    print('----------------------------------\n')

    burst_phases = get_mpiphase_from_file(presim_burst_file, '0')

    if correction_factor:
        print('ACTION 1: Create new presim file with extrapolated values')
        print('Bursts will be multiplied by the contention factor.')

        for index, row in join_df.iterrows():
            print('\t Phase {0}: ID={1}, ContentionFactor={2}'.format(index, row['phase_id'], row['contention_factor']))

        print('\n')
        if args.pattern:
            pattern_input = args.pattern[0]
        else:
            pattern_input = input('Please select a cyclic pattern to apply to the PRESIM File (e.g: 0,1,1): ')


        pattern_phases = list(map(int, pattern_input.split(',')))
        contention_factor_list = [ join_df['contention_factor'].iloc[p] for p in pattern_phases ]

        # Apply changes
        # Load burst phases file

        rank_values = pd.unique(burst_phases['rank'].values).astype(str)

        for r in rank_values:
            contention_factor_cycle = itertools.cycle(contention_factor_list)

            #print("Processing rank {0}".format(r))
            # Mutiply each burst value by a contention factor.
            for index in burst_phases.loc[burst_phases['rank'] == r].index.tolist():
                new_value = int(burst_phases['value'].loc[index]) * float(next(contention_factor_cycle))
                burst_phases.set_value(index,'value', int(round(new_value)))
                burst_phases.set_value(index,'mode', 3) # In presim files mode=3 means value extrapolation

    extrapolated_filename=presim_burst_file.replace("burst_adjusted","extrapolated")
    #print(extrapolated_filename)
    burst_phases.to_csv('{0}'.format(extrapolated_filename), sep=':', index=False, header=False)



if __name__ == "__main__":
     # PARSE ARGUMENTS
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('-m', '--filemem', metavar='filemem', type=str, nargs=1,
                        help='Path to the dat file with the presim data.')
    parser.add_argument('-b','--fileburst', metavar='fileburst', type=str, nargs=1,
                        help='Path to the dat file with the aditional BURSTS.')
    parser.add_argument('-c','--filecorrection', metavar='filecorrection', type=str, nargs=1,
                        help='.')
    parser.add_argument('-p','--pattern', type=str, nargs=1)
    args = parser.parse_args()

    main(args)

