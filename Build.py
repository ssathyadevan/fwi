#!/usr/bin/env python3
import os
import sys
import json
from pythonScripts.buildandrunfunctions import *

if sys.platform.startswith('linux'):
    current_directory = os.getcwd()

    install_gtest()

    print('Build project:')
    if not os.path.isdir(current_directory + '/build'):
        os.mkdir('build')
    os.chdir(current_directory + '/build')
    check = os.system('sudo cmake -DCMAKE_BUILD_TYPE=Release ' +
                    '-DCMAKE_INSTALL_PREFIX=../FWIInstall ../')
    checking_for_errors(check, current_directory)
    check = os.system('sudo make install')
    checking_for_errors(check, current_directory)

