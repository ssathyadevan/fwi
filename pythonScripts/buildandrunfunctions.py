import os
import sys
import json
import numpy as np
import re

def checking_for_errors(err, current_directory):
    if err != 0:
        print('An error was found, we will not continue with the Build and Run script')
        os.chdir(current_directory + '/inputFiles/default/input/')
        os.system('rm -r temp')
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
    row_format = "|{:^20}" * 5
    headers_list = ['Inversion Method', 'Forward Model', 'Conditions', 'Input Image', 'Output resolution']
    print(row_format.format(*headers_list))
    for ind_run in running_table:
        inv_method = ind_run[0][0].upper()
        for a in ind_run[0]:
            if a.isupper():
                inv_method += a
        fm = ind_run[1][0].upper()
        for a in ind_run[1]:
            if a.isupper():
                fm += a
        print(row_format.format(inv_method, fm, *ind_run[2:-2]))

def enter_description(current_directory, table):
    description = input('Please enter a description of the run or press enter: \n')
    if not os.path.isdir(current_directory + '/results'):
        os.mkdir(current_directory + '/results')
    os.chdir(current_directory + '/results')
    with open("description" + str(table[6]) + ".txt", "w+") as in_file:
        in_file.write(table[0] + '\n')
        in_file.write(table[1] + '\n')
        in_file.write(table[5] + '\n')
        in_file.write(description)

def ask_options(running_table, current_directory):
    answer = '10'
    while int(answer) != 0:
        print('\nDo you want to (0)-run, (1)-edit, or (2)-add?')
        answer = input('~:')
        while answer not in ['0', '1', '2']:
            print('You introduced an invalid command, maybe try again.')
            answer = input('~: ')
        if int(answer) == 1:
            running_table = edit(running_table, current_directory)
            enter_description(current_directory, running_table[-1])
            print_run(running_table)
        elif int(answer) == 2:
            running_table = add_to(running_table, current_directory)
            enter_description(current_directory, running_table[-1])
            print_run(running_table)
    return running_table

def ask_method(current_directory):
    os.chdir(current_directory + '/libraries/inversion/')
    available_methods = os.popen('ls -d */').read().split('\n')
    valid_numbers = []
    print("Which method do you want to try?")
    for i, method in enumerate(available_methods[:-1]):
        print(i, method[:-1])
        valid_numbers.append(str(i))
    answer = input('~: ')
    while answer not in valid_numbers:
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    return available_methods[int(answer)][:-1]

def ask_forwardmodel(current_directory):
    os.chdir(current_directory + '/libraries/forwardModel/')
    available_fm = os.popen('ls -d */').read().split('\n')
    valid_numbers = []
    print("Which forward model do you want to use?")
    for i, method in enumerate(available_fm[:-1]):
        print(i, method[:-1])
        valid_numbers.append(str(i))
    answer = input('~: ')
    while answer not in valid_numbers:
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    return available_fm[int(answer)][:-1] + 'ForwardModel'

def change_conditions(method, current_directory):
    print('Do you want to change the default conditions of the method? (0)-No (1)-Yes')
    answer = input('~: ')
    while answer not in ['0', '1']:
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    if answer == '1':
        os.chdir(current_directory + '/inputFiles/default/input/')
        os.system('nano ' + method[:1].upper() + method[1:] + 'Input.json')
        return 'Edited'
    return 'Default'

def imagetotxt(imagename, resolution, current_directory):
    import matplotlib.image as mplimg
    import matplotlib.pyplot as plt
    from skimage.transform import resize
    import numpy as np

    filename = imagename[imagename.rfind('/')+1:imagename.rfind('.')]
    image = mplimg.imread(imagename)
    if len(image[0][0]) > 1:
        def rgb2gray(rgb):
            return np.dot(rgb[..., :3], [0.2989, 0.5870, 0.1140])
        image = rgb2gray(image)
    rescale = resize(image, resolution)
    rescale = np.transpose(rescale)
    rescale.resize((rescale.shape[0] * rescale.shape[1]))
    rescale = ((rescale / np.amax(rescale)) - 1) * (-0.18)
    np.savetxt(current_directory + '/inputFiles/default/input/' + filename + '.txt', rescale, delimiter='\n')
    np.savetxt(current_directory + '/inputFiles/' + filename + '.txt', rescale, delimiter='\n')
    return filename

def add_image(current_directory):
    print('Absolute path to new image, with image name:')
    imagename = input('~: ')
    while not os.path.isfile(imagename):
        print("The path or name of the image is wrong, maybe try again.")
        imagename = input('~: ')
    print('Desired resolution of image:')
    answer = input('~: ')
    while not re.match(r"\d+[x]\d+", answer):
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    temp = answer.split('x')
    resolution = (int(temp[1]), int(temp[0]))
    return imagetotxt(imagename, resolution, current_directory)

def ask_input(current_directory):
    os.chdir(current_directory + '/inputFiles/default/input')
    available_images = os.popen('ls *.txt').read().split('\n')
    print("Which image do you want?")
    valid_numbers = []
    for i, im in enumerate(available_images[:-1]):
        print(i, im[:-4])
        valid_numbers.append(str(i))
    valid_numbers.append('99')
    print('99', 'Add new image.')
    answer = input('~: ')
    while answer not in valid_numbers:
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    if int(answer) == 99:
        return add_image(current_directory)
    return available_images[int(answer)][:-4]

def ask_output():
    print('What output resolution do you want (mxn)?')
    answer = input('~: ')
    while not re.match(r"\d+[x]\d+", answer):
        print('You introduced an invalid command, maybe try again.')
        answer = input('~: ')
    return answer

def ask_outputfile():
    print('Give a name for the output to be store:')
    answer = input('~: ')
    return answer

def add_to(table, current_directory):
    method = ask_method(current_directory)
    forwardmodel = ask_forwardmodel(current_directory)
    conditions = change_conditions(method, current_directory)
    image = ask_input(current_directory)
    out_resolution = ask_output()
    folder = ask_outputfile()

    table.append([method, forwardmodel, conditions, image, out_resolution, folder, table[-1][6] + 1])
    os.chdir(current_directory + '/inputFiles/default/input/')
    os.system('cp -r ' + method[:1].upper() + method[1:] +
              'Input.json temp/temp' + str(table[-1][6]) + '.json')
    return table

def edit(table, current_directory):
    if len(table) != 1:
        print('Which run do you want to edit?')
        answer = input('~: ')
        while int(answer) > len(table):
            print('You introduced an invalid command, maybe try again.')
            answer = input('~: ')
        index = int(answer) - 1
    else:
        index = 0

    table[index][0] = ask_method(current_directory)
    table[index][1] = ask_forwardmodel(current_directory)
    table[index][2] = change_conditions(table[index][0], current_directory)
    table[index][3] = ask_input(current_directory)
    table[index][4] = ask_output()
    table[index][5] = ask_outputfile()

    os.chdir(current_directory + '/inputFiles/default/input/')
    os.system('cp -r ' + table[index][0][:1].upper() + table[index][0][1:] +
              'Input.json temp/temp' + str(table[index][6]) + '.json')
    return table

def change_json(input_image, output_resolution, current_directory):
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

def set_jsons_todefault(current_directory):
    os.chdir(current_directory + '/inputFiles/default/input/')
    with open('GenericInput.json', 'r+') as jsonFile:
        data = json.load(jsonFile)
        data['ngrid']['x'] = 64
        data['ngrid']['z'] = 32
        data['fileName'] = 'temple'
        data['ngrid_original']['x'] = 64
        data['ngrid_original']['z'] = 32
        jsonFile.seek(0)
        json.dump(data, jsonFile, indent=4)
        jsonFile.truncate()
    with open('ConjugateGradientInversionInput.json', 'r+') as jsonFile:
        data = json.load(jsonFile)
        data['n_max'] = 5
        jsonFile.seek(0)
        json.dump(data, jsonFile, indent=4)
        jsonFile.truncate()


def check_file_res(image):
    count = len(open(image + '.txt').readlines())
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