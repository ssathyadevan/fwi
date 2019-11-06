import os
import sys
import json
from pythonScripts.buildandrunfunctions import *

if sys.platform.startswith('linux'):
    current_directory = os.getcwd()

    os.chdir('libraries/inversion/')
    available_methods = os.popen('ls -d */').read().split('\n')

    running_table = [['conjugateGradientInversion', 'integralForwardModel', 'Default', 'temple', '64x32', 'default', 0]]
    set_jsons_todefault(current_directory)
    os.chdir(current_directory + '/inputFiles/default/input/')
    if not os.path.isdir('temp'):
        os.mkdir('temp')
    os.system('cp -r ' + running_table[0][0][:1].upper() + running_table[0][0][1:] +
              'Input.json temp/temp' + str(running_table[0][6]) + '.json')

    print_run(running_table)
    ask_options(running_table, current_directory)

    install_gtest()

    if running_table[0][2] == 'Default' and running_table[0][5] == 'default':
        enter_description(current_directory)

    for ind_run in running_table:
        print('We start with the ' + ind_run[0] + ' method.')
        change_json(ind_run[3], ind_run[4].split('x'), current_directory)

        print('Running project:')
        os.chdir(current_directory)
        if not os.path.isdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall'):
            os.mkdir('../FWIInstall')
        os.system('cp -R ../parallelized-fwi/inputFiles/default ../FWIInstall/' + ind_run[5])
        os.system('cp -r ../Build/runtime/bin/ ../FWIInstall')
        os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall/' + ind_run[5] + '/input/')
        os.system('cp -r temp/temp' + str(ind_run[6]) + '.json ' + ind_run[0][:1].upper() + ind_run[0][1:] +
                  'Input.json')
        os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall/bin')
        if ind_run[1] == 'integralForwardModel':
            check = os.system('./FWI_PreProcess ../' + ind_run[5])
        elif ind_run[1] == 'finiteDifferenceForwardModel':
            check = os.system('./FWI_PreProcess_Finite_Difference ../' + ind_run[5])
        else:
            print('There is something wrong. The pre-processing will execute with integralForwardModel.')
            check = os.system('./FWI_PreProcess ../' + ind_run[5])
        checking_for_errors(check)
        check = os.system('./FWI_UnifiedProcess ../' + ind_run[5] + ' ' + ind_run[0] + ' ' + ind_run[1])
        checking_for_errors(check)

    print('Now post processing')
    for ind_run in running_table:
        print(ind_run[5], ':')
        os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall')
        os.system('cp ../parallelized-fwi/pythonScripts/postProcessing-python3.py .')
        check = os.system('python3 postProcessing-python3.py ' + ind_run[5])
        checking_for_errors(check)
        # os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall/default/output')
        # os.system('eog defaultResult.png')
    os.chdir(current_directory + '/inputFiles/default/input/')
    os.system('rm -r temp')
