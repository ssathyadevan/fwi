import os
import sys
import json

home = os.path.expanduser("~")

def checking_for_errors(err):
    if err != 0:
        print('An error was found, we will not continue with the Build and Run script')
        sys.exit()


def install_gtest():
    if os.path.isdir('/usr/src/gtest'):
        print('Google tests is already installed')
    else:
        print('First we install Google Test:')
        os.chdir(home)

        os.system('sudo apt-get install libgtest-dev')
        os.chdir('/usr/src/gtest')

        os.system('sudo cmake CMakeLists.txt')
        os.system('sudo make install')
        os.system('cp *.a /usr/lib')

if sys.platform.startswith('linux'):
    os.system('sudo apt install python3-skimage')
    
    print('Build project:')
    os.chdir(home)
    if not os.path.isdir(home + '/Build'):
        os.mkdir('Build')
    os.chdir(home + '/Build')
    check = os.system('sudo cmake -DCMAKE_BUILD_TYPE=Release ' +
                        '-DCMAKE_INSTALL_PREFIX='+ home +'/FWIInstall '+ home +'/parallelized-fwi/')
    checking_for_errors(check)
    check = os.system('sudo make install')
    checking_for_errors(check)

    if not os.path.isdir(home + '/FWIInstall'):
        os.mkdir(home + '/FWIInstall')
    os.system('sudo cp -r '+ home + '/parallelized-fwi/inputFiles/default/ '+ home +'/FWIInstall')
    os.system('sudo cp -r '+ home +'/Build/runtime/bin/ '+ home +'/FWIInstall')
    