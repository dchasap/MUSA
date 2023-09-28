#!/usr/bin/env python3

# Tool for auto-generating sets of Tasksim Config (tsconf) files.

# 1.- DEFINE RULES
# 2.- SELECT RULES you want to apply
# 3.- GENERATE files applying rules

import os
import argparse

config_store_folder = 'ts_conf_big'

class SafeDict(dict):
    def __missing__(self, key):
        return '{' + key + '}'

def define_all_rules():
    '''
        d['Example_Rule_tag'] = [['tsconf_field_1', 'tsconf_field_2'],
                                 {'option_tag'    : ['replace_in_field1','replace_in_field2'], 
                                  'option2_tag'   : ['replace_field1','replace_field2'] }
    '''

    rules = {}

    rules['core'] = [['ncpus'], 
                    { '001' : ['1'], '002' : ['2'],
                      '004' : ['4'],  '008' : ['8'],
                      '016' : ['16'], '032' : ['32'],
                      '064' : ['64'] }]          

    rules['vec'] = [['vector_register_length'], 
                    {'128' : ['128'], '256' : ['256'], '512' : ['512']}]

    rules['ctype'] = [['out_buff', 'rob_size', 'issue_rate', 'commit_rate'], 
                      {'small' : ['100','112', '2', '2'], 
                       'med': ['100','150', '4', '4'],
                       'big'   : ['150','224', '8', '8'] }]

    #cache size
    rules['cache'] = [['l1_latency', 'l1_size', 'l1_assoc', 'l2_latency', 'l2_size', 'l2_assoc',
                   'l3_latency', 'l3_size', 'l3_assoc'], 
                  {
                   'thx2'    : ['4','32768', '8', '12', '262144', '16', '42', '33554432', '32'],
                   'postk'   : ['4','32768', '8', '12', '524288', '16', '45', '67108864', '32'] 
                  }]

    rules['l3shr'] = [['l3_mshr'],
                      {'med': ['32'],
                       'big': ['256']
                      }] 

    rules['freq'] = [['cpu_freq'],
                      {'1.5': ['1500'],
                       '2.0': ['2000'],
                       '2.5': ['2500'],
                       '3.0': ['3000'],
                       '3.5': ['3500']
                      }]

    rules['mem'] = [['mem_type', 'mem_chan', 'mem_speed', 'mem_org'], 
                    {'DDR3'  : ['DDR3','1', ' DDR3_2133M', 'DDR3_8Gb_x4'], 
                     'DDR4'  : ['DDR4','8', 'DDR4_2400R', 'DDR4_8Gb_x8'],
                     'HBM'   : ['HBM','8', 'HBM_1Gbps', 'HBM_4Gb'] }]

    return rules

def select_rules():
    ''' Rules are selected here '''
    sr = []
    #sr.append(['vec', ['128', '256']])
    #sr.append(['core', ['001', '008', '016']])
    #sr.append(['ctype', ['med', 'big']])
    #sr.append(['mem', ['HBM']])

    sr.append(['core', ['001', '008', '016', '064']])
    sr.append(['ctype', ['small', 'med', 'big']])
    sr.append(['cache', ['thx2', 'postk']])
    sr.append(['freq', ['2.0','3.2']])
    sr.append(['mem', ['DDR3','DDR4','HBM']])
    sr.append(['l3shr', ['med','big']])

    return sr 

def default_options(template):
    tmp_text = template
    d = {  
         'mode_selector' : 'MEMORY',
         'vec_length'    : '512',  
         'l1_mshr'       : '16',
         'l2_mshr'       : '16',
         'prv_name'      : './sim_trace.prv',
         'prv_policy'    : 'DISABLED',
         'mem_trace'     : 'on',
	     #'cpu_freq'      : '2.0',
	     #'l3_mshr'       : '32'
        }
    tmp_text = tmp_text.format_map(SafeDict(d))
    return tmp_text


def replace_values_in_cfg(param, values, template):
    ''' parameters: ts parameters that will be replaced in the template
        values: new values for those parameters ''' 
    new_text = template

    new_dict = dict(zip(param , values))
    new_text = template.format_map(SafeDict(new_dict))

    return new_text

def apply_rules(level, all_rules, selected_rules, ts_cfg_template, suffix):
    '''
        Recursive function that generates and saves to disk all possible Tasksim config combinations.
        Takes the selected set of rules and applies them to the template config.abs
    '''

    if level == len(selected_rules):
        # If I finished all rule levels: output to disk

        fname = '{0}/{1}.conf'.format(config_store_folder, suffix)
        f = open(fname, 'w+')
        f.write(ts_cfg_template)
        f.close()
    else:
        rule_tag = selected_rules[level][0]
        #print('Rule tag: {0}'.format(rule_tag))

        options_tag = [k for k in all_rules[rule_tag][1].keys()]
        options_tag = [ k for k in selected_rules[level][1] ]
        #print('Options: {0}'.format(options_tag))

        for opt in options_tag:
            tmp_suffix = '{0}_{1}-{2}'.format(suffix, rule_tag, opt)

            # apply changes to tmp_template
            tmp_template = replace_values_in_cfg(all_rules[rule_tag][0], all_rules[rule_tag][1][opt],
                                                 ts_cfg_template)

            # Recursivity
            apply_rules(level + 1, all_rules, selected_rules, tmp_template, tmp_suffix)


def main(args):
    ts_template_filepath = args.filepath[0]
    suffix = 'ts' # ts_template_filepath ( name of file as prefix)

    if not os.path.isdir(config_store_folder):
        os.makedirs(config_store_folder)

    all_rules = define_all_rules()
    selected_rules = select_rules()

    with open(ts_template_filepath, 'r') as file :
        ts_cfg_template = file.read()

    # 1.- APPLY DEFAULT OPTIONS
    ts_cfg_template = default_options(ts_cfg_template)

    # 1.- Combine rules all to all & and write to disk
    apply_rules(0, all_rules, selected_rules, ts_cfg_template, suffix)

    
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('template', metavar='template', type=str, nargs=1,
                        help='Path to the Tasksim configuration template file.')
    args = parser.parse_args()
    main(args)

    
