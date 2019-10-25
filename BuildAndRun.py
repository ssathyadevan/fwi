import os
import sys
import json


def checking_for_errors(err):
    if err != 0:
        print('An error was found, we will not continue with the Build and Run script')
        sys.exit()


def install_gtest():
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


def print_run(running_table):
    print('The Build and run is set to run:')
    row_format = "|{:^20}" * 4
    headers_list = ['Method', 'Conditions', 'Input Image', 'Output resolution']
    print(row_format.format(*headers_list))
    for ind_run in running_table:
        print(row_format.format(*ind_run[:-1]))


def ask_options(running_table):
    answer = '10'
    while int(answer) != 0:
        print('\nDo you want to (0)-run, (1)-edit, or (2)-add?')
        answer = input('~:')
        while int(answer) not in [0, 1, 2]:
            print('You introduced an invalid command, maybe try again.')
            answer = input('~: ')
        if int(answer) == 1:
            running_table = edit(running_table)
            print_run(running_table)
        elif int(answer) == 2:
            running_table = add_to(running_table)
            print_run(running_table)
    return running_table


def add_to(table):
    os.chdir(current_directory + '/libraries/inversion/')
    available_methods = os.popen('ls -d */').read().split('\n')
    valid_numbers = []
    print("Which method do you want to try?")
    for i, method in enumerate(available_methods[:-1]):
        if method != 'inversionFactory/':
            print(i, method[:-1])
            valid_numbers.append(i)
    answer = input('~: ')
    while int(answer) not in valid_numbers:
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    method = available_methods[int(answer)][:-1]

    print('Do you want to change the default conditions of the method? (0)-No (1)-Yes')
    answer = input('~: ')
    while int(answer) not in [0, 1]:
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    if answer == '1':
        os.chdir(current_directory + '/inputFiles/default/input/')
        os.system('nano ' + method[:1].upper() + method[1:] + 'Input.json')
        conditions = 'Edited'
    else:
        conditions = 'Default'

    os.chdir(current_directory + '/inputFiles/')
    available_images = os.popen('ls *.txt').read().split('\n')
    print("Which image do you want?")
    for i, im in enumerate(available_images[:-1]):
        print(i, im[:-1])
        valid_numbers.append(i)
    answer = input('~: ')
    while int(answer) not in valid_numbers:
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    image = available_images[int(answer)][:-4]

    print('What output resolution do you want?')
    answer = input('~: ')
    temp = answer.split('x')
    while (not temp[0].isdigit()) and (not temp[1].isdigit()):
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    out_resolution = answer
    table.append([method, conditions, image, out_resolution, table[-1][4] + 1])
    os.chdir(current_directory + '/inputFiles/default/input/')
    os.system('cp -r ' + method[:1].upper() + method[1:] +
              'Input.json temp/temp' + str(running_table[-1][4]) + '.json')
    return table


def edit(table):
    if len(table) != 1:
        print('Which run do you want to edit?')
        answer = input('~: ')
        while int(answer) > len(table):
            print('You introduced an invalid command, maybe try again.')
            answer = input('~: ')
        index = int(answer) - 1
    else:
        index = 0
    os.chdir(current_directory + '/libraries/inversion/')
    available_methods = os.popen('ls -d */').read().split('\n')
    valid_numbers = []
    print("Which method do you want to try?")
    for i, method in enumerate(available_methods[:-1]):
        if method != 'inversionFactory/':
            print(i, method[:-1])
            valid_numbers.append(i)
    answer = input('~: ')
    while int(answer) not in valid_numbers:
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    table[index][0] = available_methods[int(answer)][:-1]

    print('Do you want to change the default conditions of the method? (0)-No (1)-Yes')
    answer = input('~: ')
    while int(answer) not in [0, 1]:
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    if answer == '1':
        os.chdir(current_directory + '/inputFiles/default/input/')
        os.system('nano ' + table[index][0][:1].upper() + table[index][0][1:] + 'Input.json')
        table[index][1] = 'Edited'
    else:
        table[index][1] = 'Default'

    os.chdir(current_directory + '/inputFiles/')
    available_images = os.popen('ls *.txt').read().split('\n')
    print("Which image do you want?")
    for i, method in enumerate(available_images[:-1]):
        print(i, method[:-4])
        valid_numbers.append(i)
    answer = input('~: ')
    while int(answer) not in valid_numbers:
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    table[index][2] = available_images[int(answer)][:-4]

    print('What output resolution do you want?')
    answer = input('~: ')
    temp = answer.split('x')
    while (not temp[0].isdigit()) and (not temp[1].isdigit()):
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    table[index][3] = answer

    os.chdir(current_directory + '/inputFiles/default/input/')
    os.system('cp -r ' + table[index][0][:1].upper() + table[index][0][1:] +
              'Input.json temp/temp' + str(running_table[index][4]) + '.json')
    return table


def change_json(input_image, output_resolution):
    os.chdir(current_directory + '/inputFiles/default/input/')
    x, y = check_file_res(input_image)
    with open('GenericInput.json', 'r+') as jsonFile:
        data = json.load(jsonFile)

        data['ngrid']['x'] = int(output_resolution[0])
        data['ngrid']['z'] = int(output_resolution[1])
        data['fileName'] = input_image
        data['ngrid_original']['x'] = x
        data['ngrid_original']['z'] = y

        jsonFile.seek(0)
        json.dump(data, jsonFile, indent=4)
        jsonFile.truncate()


def check_file_res(image):
    count = len(open('../../' + image + '.txt').readlines())
    if count == 2048:
        return 64, 32
    elif count == 1250:
        return 50, 25
    elif count == 8192:
        return 128, 64
    else:
        print('The resolution is not known, please tell me the resolution.')
        answer = input('~: ')
        temp = answer.split('x')
        while (not temp[0].isdigit()) and (not temp[1].isdigit()):
            print('You introduced an invalid command, maybe try again.')
            answer = input('~: ')
            temp = answer.split('x')
        return int(temp[0]), int(temp[1])


if sys.platform.startswith('linux'):
    current_directory = os.getcwd()

    os.chdir('libraries/inversion/')
    available_methods = os.popen('ls -d */').read().split('\n')

    running_table = [['conjugateGradientInversion', 'Default', 'temple', '50x25', 0]]
    os.chdir(current_directory + '/inputFiles/default/input/')
    if not os.path.isdir('temp'):
        os.mkdir('temp')
    os.system('cp -r ' + running_table[0][0][:1].upper() + running_table[0][0][1:] +
              'Input.json temp/temp' + str(running_table[0][4]) + '.json')

    print_run(running_table)
    ask_options(running_table)

    install_gtest()

    for ind_run in running_table:
        print('We start with the ' + ind_run[0] + ' method.')
        change_json(ind_run[2], ind_run[3].split('x'))

        print('Build project:')
        os.chdir(current_directory[:current_directory.rfind('/')])
        if not os.path.isdir(current_directory[:current_directory.rfind('/')] + '/Build'):
            os.mkdir('Build')
        os.chdir(current_directory[:current_directory.rfind('/')] + '/Build')
        check = os.system('sudo cmake -DCMAKE_BUILD_TYPE=Release ' +
                          '-DCMAKE_INSTALL_PREFIX= ../FWIInstall ../parallelized-fwi/')
        checking_for_errors(check)
        check = os.system('sudo make install')
        checking_for_errors(check)

        print('Running project:')
        if not os.path.isdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall'):
            os.mkdir('../FWIInstall')
        os.system('cp -r ../parallelized-fwi/inputFiles/default/ ../FWIInstall')
        os.system('cp -r ../Build/runtime/bin/ ../FWIInstall')
        os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall/default/input/')
        os.system('cp -r temp/temp' + str(ind_run[4]) + '.json ' + ind_run[0][:1].upper() + ind_run[0][1:] +
                  'Input.json')
        os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall/bin')
        check = os.system('./FWI_PreProcess ../default/')
        checking_for_errors(check)
        check = os.system('./FWI_UnifiedProcess ../default/ ' + ind_run[0])
        checking_for_errors(check)

        print('Now post processing')
        os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall')
        os.system('cp ../parallelized-fwi/pythonScripts/postProcessing-python3.py .')
        check = os.system('python3 postProcessing-python3.py default/')
        checking_for_errors(check)
        # os.chdir(current_directory[:current_directory.rfind('/')] + '/FWIInstall/default/output')
        # os.system('eog defaultResult.png')
    os.chdir(current_directory + '/inputFiles/default/input/')
    os.system('rm -r temp')
