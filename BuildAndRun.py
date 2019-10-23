import os
import sys

if sys.platform.startswith('linux'):
    current_directory = os.getcwd()

    if os.path.isdir('/usr/src/gtest'):
        print('Google tests is already installed')
    else:
        print('First we install Google Test:')
        os.chdir('/home')

        os.system('sudo apt-get install libgtest-dev')
        os.chdir('/usr/src/gtest')

        os.system('sudo cmake CMakeLists.txt')
        os.system('sudo make install')
        os.system('cp *.a /usr/lib')

    print('Now we start the building process:')
    os.chdir(current_directory[:current_directory.rfind('/')])
    if not os.path.isdir(current_directory[:current_directory.rfind('/')] + '/Build'):
        os.system('mkdir Build')
    os.chdir(current_directory[:current_directory.rfind('/')] + '/Build')
    os.system('sudo cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX= ../FWIInstall ../parallelized-fwi/')
    os.system('sudo make install')

    print('Now the running time:')
    if not os.path.isdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall/default'):
        os.system('cp -r ../parallelized-fwi/inputFiles/default/ ../FWIInstall')
    os.system('cp -r runtime/bin/ ../FWIInstall')
    os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall/bin')
    os.system('./FWI_PreProcess ../default/')
    os.system('./FWI_Process ../default/')

    print('Now post processing')
    os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall')
    os.system('cp ../parallelized-fwi/pythonScripts/postProcessing-python3.py .')
    os.system('python3 postProcessing-python3.py default/')
    os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall/default/output')
    os.system('eog defaultResult.png')
