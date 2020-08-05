#!/usr/bin/env python3
import os
import sys

from pythonScripts.buildandrunfunctions import (set_jsons_todefault,
                                                enter_description,
                                                change_json,
                                                checking_for_errors,
                                                print_run,
                                                ask_options,
                                                install_gtest,
                                                run_postprocessing)

if sys.platform.startswith('linux'):
    mpi = (os.path.exists('/usr/bin/mpicxx') and os.path.exists('/usr/bin/mpiexec'))
    current_directory = os.getcwd()

    os.chdir('libraries/inversion/')
    available_methods = os.popen('ls -d */').read().split('\n')

    running_table = [['ConjugateGradientInversion', 'IntegralForwardModel', 'Default', 'temple', '64x32', 'default', 0]]
    set_jsons_todefault(current_directory)
    os.chdir(current_directory + '/inputFiles/default/input/')
    if not os.path.isdir('temp'):
        os.mkdir('temp')
    check = os.system('cp -r ' + running_table[0][0][:1].upper() + running_table[0][0][1:] +
              'Input.json temp/temp' + str(running_table[0][6]) + '.json')
    checking_for_errors(check, current_directory)

    print_run(running_table)
    ask_options(running_table, current_directory)

    install_gtest()

    if running_table[0][2] == 'Default' and running_table[0][5] == 'default':
        enter_description(current_directory, running_table[0])

    for ind_run in running_table:
        print('We start with the ' + ind_run[0] + ' method.')
        change_json(ind_run[3], ind_run[4].split('x'), current_directory)

        print('Build project:')
        if not os.path.isdir(current_directory + '/build'):
            os.mkdir('build')
        os.chdir(current_directory + '/build')
        check = os.system('sudo cmake -DCMAKE_BUILD_TYPE=Release ' +
                        '-DCMAKE_INSTALL_PREFIX=../FWIInstall ../')
        checking_for_errors(check, current_directory)
        check = os.system('sudo make install')
        checking_for_errors(check, current_directory)
        
        print('Running project:')
        if not os.path.isdir(current_directory + '/FWIInstall'):
            os.mkdir('../FWIInstall')
        elif os.path.isdir(current_directory + '/FWIInstall/' + ind_run[5]):
            os.system('rm -r ../FWIInstall/' + ind_run[5])
        check = os.system('cp -R ../inputFiles/default ../FWIInstall/' + ind_run[5])
        checking_for_errors(check, current_directory)
        check = os.system('cp -r ../build/runtime/bin/ ../FWIInstall')
        checking_for_errors(check, current_directory)
        os.chdir(current_directory + '/FWIInstall/' + ind_run[5] + '/input/')
        check = os.system('cp -r temp/temp' + str(ind_run[6]) + '.json ' + ind_run[0][:1].upper() + ind_run[0][1:] +
                'Input.json')
        checking_for_errors(check, current_directory)
        os.system('rm -r temp')
        os.chdir(current_directory + '/FWIInstall/bin')    
        if ind_run[1] == 'IntegralForwardModel':
            check = os.system('./FWI_PreProcess_Integral ../' + ind_run[5])
        elif ind_run[1] == 'FiniteDifferenceForwardModel':
            check = os.system('./FWI_PreProcess_Finite_Difference ../' + ind_run[5])
        else:
            print('there is something wrong with the ForwardModel you choose. The preprocessing was done with IntegralForwardModel')
            check = os.system('./FWI_PreProcess_Integral ../' + ind_run[5])
        checking_for_errors(check, current_directory)
        if ind_run[0] != "MPIConjugateGradientInversion":
            check = os.system('./FWI_UnifiedProcess ../' + ind_run[5] + ' ' + ind_run[0] + ' ' + ind_run[1])
        else:
            if mpi:
                check = os.system('mpiexec -n 3 --allow-run-as-root ./FWI_MPIProcess ../' + ind_run[5] + ' ' + ind_run[1])#+ ' ' + ind_run[0] + ' ' + ind_run[1]) #valgrind --tool=callgrind --cache-sim=yes --separate-threads=yes
            else: 
                print("MPI is not installed, using conjugate gradient inversion on single thread instead")
                ind_run[0] = "ConjugateGradientInversion"
                check = os.system('./FWI_UnifiedProcess ../' + ind_run[5] + ' ' + ind_run[0] + ' ' + ind_run[1])
        checking_for_errors(check, current_directory)

    run_postprocessing(parameters = running_table, directory = current_directory)

    os.chdir(current_directory + '/inputFiles/default/input/')
    os.system('rm -r temp')
