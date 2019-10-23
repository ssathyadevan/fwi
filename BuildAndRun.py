import os
import sys

def checking_for_errors(err):
    if err != 0:
        print ('An error was found, we will not continue with the Build and Run script')
        sys.exit()

def intall_gtest():
    os.chdir('/home')

    os.system('sudo apt-get install libgtest-dev')
    os.chdir('/usr/src/gtest')

    os.system('sudo cmake CMakeLists.txt')
    os.system('sudo make install')
    os.system('cp *.a /usr/lib')

if sys.platform.startswith('linux'):
    current_directory = os.getcwd()

    os.chdir('libraries/inversion/')
    available_methods = os.popen('ls -d */').read().split('\n')
    valid_numbers = []

    print("Which method do you want to try?")
    for i, method in enumerate(available_methods[:-1]):
        if method != 'inversionFactory/':
            print(i, method[:-1])
            valid_numbers.append(i)
    answer = input('Your method: ')
    while int(answer) not in valid_numbers:
        print('You introduced an invalid command, maybe try again.')
        answer = input('Your method: ')
    method = available_methods[int(answer)][:-1]

    if os.path.isdir('/usr/src/gtest'):
        print('Google tests is already installed')
    else:
        print('First we install Google Test:')
        install_gtest()

    print('Now we start the building process:')
    os.chdir(current_directory[:current_directory.rfind('/')])
    if not os.path.isdir(current_directory[:current_directory.rfind('/')] + '/Build'):
        print('we create build folder')
        os.mkdir('Build')
    os.chdir(current_directory[:current_directory.rfind('/')] + '/Build')
    check = os.system('sudo cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX= ../FWIInstall ../parallelized-fwi/')
    checking_for_errors(check)
    check = os.system('sudo make install')
    checking_for_errors(check)

    print('Now the running time:')
    if not os.path.isdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall'):
        print('we create FWIInstall folder')
        os.mkdir('../FWIInstall')
    smt = input('pause')
    os.system('cp -r ../parallelized-fwi/inputFiles/default/ ../FWIInstall')
    smt = input('pause')
    os.system('cp -r ../Build/runtime/bin/ ../FWIInstall')
    os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall/bin')
    check = os.system('./FWI_PreProcess ../default/')
    checking_for_errors(check)
    check = os.system('./FWI_UnifiedProcess ../default/ ' + method)
    checking_for_errors(check)

    print('Now post processing')
    os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall')
    os.system('cp ../parallelized-fwi/pythonScripts/postProcessing-python3.py .')
    check = os.system('python3 postProcessing-python3.py default/')
    checking_for_errors(check)
    os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall/default/output')
    os.system('eog defaultResult.png')
