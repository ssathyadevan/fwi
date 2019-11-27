#!/usr/bin/env python3
import os
import sys
import json
from pythonScripts.buildandrunfunctions import *

if sys.platform.startswith('linux'):
    current_directory = os.getcwd()

    install_gtest()

    print('Build project:')
    os.chdir(current_directory[:current_directory.rfind('/')])
    if not os.path.isdir(current_directory[:current_directory.rfind('/')] + '/Build'):
        os.mkdir('Build')
    os.chdir(current_directory[:current_directory.rfind('/')] + '/Build')
    check = os.system('sudo cmake -D CMAKE_BUILD_TYPE=Release ' +
                    '-DCMAKE_INSTALL_PREFIX= ../FWIInstall ../parallelized-fwi/')
    checking_for_errors(check, current_directory)
    check = os.system('sudo make install')
    checking_for_errors(check, current_directory)
        