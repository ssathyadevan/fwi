#!/usr/bin/env python3
# Adding json specific read out values
# Add variable hendling to Processs
# split executionMethod in importValues and ExecutionMethod

import os, sys, shutil, csv, datetime, time
from datetime import datetime as dt

def makeExecutionFolder():
    print('\n------Creating a new execution folder')
    now = dt.now()
    execution_stamp = 'Execution_' + now.strftime("%Y%m%d_%H%M%S")
    os.mkdir(execution_stamp)
    print("Name: " + execution_stamp)
    return execution_stamp

def makeTestFolder(tempTestPath):
    print('\n------Coping default test folder')
    shutil.copytree(tempDefaultPath, tempTestPath)
    shutil.copy(os.path.join(basename, testData), tempTestPath)

def executionMethod():
    number_current = n - int(rowStart) + 1 # The current test cycle
    number_total = int(rowEnd) - int(rowStart) + 1 # The total amout of tests to be executed
    print("""
##################################
      Start Test(s) {} of {}
##################################""".format(number_current, number_total))

    ### Read-in values from csv file
    ### Dir information
    sequence = data[n][0]
    title = data[n][1]
    sequence = sequence.zfill(2) # Fill the number with 3 digits
    testSeqenceFolder = sequence + title # mkdir name
    tempTestPath = os.path.join(basename, executionFolder, testSeqenceFolder)  # /home/user/FWITest/Execution_yyyyMMdd-hhmmss/sequence+title

    ### CGInput.json
    Iter1_n = data[n][2]
    Iter1_tolerance = data[n][3]
    DeltaAmplification_start = data[n][4]
    DeltaAmplification_slope = data[n][5]
    n_max = data[n][6]
    do_reg = data[n][7]

    ### GenericInput.json
    c_0 = data[n][8]
    Freq_max = data[n][9]
    Freq_min = data[n][10]
    Freq_nTotal = data[n][11]
    reservoirTopLeft_x = data[n][12]
    reservoirTopLeft_y = data[n][13]
    reservoirBottomRight_x = data[n][14]
    reservoirBottomRight_y = data[n][15]
    sourcesTopLeft_x = data[n][16]
    sourcesTopLeft_y = data[n][17]
    sourcesBottomRight_x = data[n][18]
    sourcesBottomRight_y = data[n][19]
    receiverTopLeft_x = data[n][20]
    receiverTopLeft_y = data[n][21]
    receiverBottomRight_x = data[n][22]
    receiverBottomRight_y = data[n][23]
    nSources = data[n][24]
    nReceivers = data[n][25]
    ngrid_x = data[n][26]
    ngrid_y = data[n][27]
    verbosity = data[n][28]
    fileName = data[n][29]

    ### IntergralFMIinput.json
    Iter2_n = data[n][30]
    Iter2_tolerance = data[n][31]
    Iter2_calcAlpha = data[n][32]

    ### Mode
    InversionMethod = data[n][33]
    Direction = data[n][34]
    ForwardModel = data[n][35]

    makeTestFolder(tempTestPath)

    print('\n------Changing input values')
    ### Replace input values of CGInput.json file
    find_Iter1_n = '10'
    replace_Iter1_n = Iter1_n
    find_Iter1_tolerance = '1.0e-8'
    replace_Iter1_tolerance = Iter1_tolerance
    find_DeltaAmplification_start = '100.0'
    replace_DeltaAmplification_start = DeltaAmplification_start
    find_DeltaAmplification_slope = '10.0'
    replace_DeltaAmplification_slope = DeltaAmplification_slope
    find_n_max = '5'
    replace_n_max = n_max
    find_do_reg = 'true'
    replace_do_reg = do_reg

    input_file_CGInput = os.path.join(tempTestPath + '/input/ConjugateGradientInversionInput.json')

    with open(input_file_CGInput, 'r') as rf:
        rf_contents = rf.readlines() # Read all lines as a list
    # Replace values
    rf_contents[2] = rf_contents[2].replace(find_Iter1_n, replace_Iter1_n)
    rf_contents[3] = rf_contents[3].replace(find_Iter1_tolerance, replace_Iter1_tolerance)
    rf_contents[6] = rf_contents[6].replace(find_DeltaAmplification_start, replace_DeltaAmplification_start)
    rf_contents[7] = rf_contents[7].replace(find_DeltaAmplification_slope, replace_DeltaAmplification_slope)
    rf_contents[9] = rf_contents[9].replace(find_n_max, replace_n_max)
    rf_contents[10] = rf_contents[10].replace(find_do_reg, replace_do_reg)

    with open(input_file_CGInput, 'w') as input_file_CGInput:
        input_file_CGInput.write(''.join(rf_contents)) # Joining list as a string

    ### Replace input values of GenericInput.json file
    find_c_0 = '2000.0'
    replace_c_0 = c_0
    find_Freq_max = '10.0'
    replace_Freq_max = Freq_max
    find_Freq_min = '40.0'
    replace_Freq_min = Freq_min
    find_Freq_nTotal = '15'
    replace_Freq_nTotal = Freq_nTotal
    find_reservoirTopLeft_x = '-300.0'
    replace_reservoirTopLeft_x = reservoirTopLeft_x
    find_reservoirTopLeft_y = '0.0'
    replace_reservoirTopLeft_y = reservoirTopLeft_y
    find_reservoirBottomRight_x = '300.0'
    replace_reservoirBottomRight_x = reservoirBottomRight_x
    find_reservoirBottomRight_y = '300.0'
    replace_reservoirBottomRight_y = reservoirBottomRight_y
    find_sourcesTopLeft_x = '-480.0'
    replace_sourcesTopLeft_x = sourcesTopLeft_x
    find_sourcesTopLeft_y = '-5.0'
    replace_sourcesTopLeft_y = sourcesTopLeft_y
    find_sourcesBottomRight_x = '480.0'
    replace_sourcesBottomRight_x = sourcesBottomRight_x
    find_sourcesBottomRight_y = '-5.0'
    replace_sourcesBottomRight_y = sourcesBottomRight_y
    find_receiverTopLeft_x = '-480.0'
    replace_receiverTopLeft_x = receiverTopLeft_x
    find_receiverTopLeft_y = '-5.0'
    replace_receiverTopLeft_y = receiverTopLeft_y
    find_receiverBottomRight_x = '480.0'
    replace_receiverBottomRight_x = receiverBottomRight_x
    find_receiverBottomRight_y = '-5.0'
    replace_receiverBottomRight_y = receiverBottomRight_y
    find_nSources = '17'
    replace_nSources = nSources
    find_nReceivers = '17'
    replace_nReceivers = nReceivers
    find_ngrid_x = '64'
    replace_ngrid_x = ngrid_x
    find_ngrid_y = '32'
    replace_ngrid_y = ngrid_y
    find_verbosity = 'false'
    replace_verbosity = verbosity
    find_fileName = 'temple'
    replace_fileName = fileName

    input_file_GenericInput = os.path.join(tempTestPath + '/input/GenericInput.json')

    with open(input_file_GenericInput, 'r') as rf:
        rf_contents = rf.readlines() # Read all lines as a list
    # Replace values
    rf_contents[1] = rf_contents[1].replace(find_c_0, replace_c_0)
    rf_contents[3] = rf_contents[3].replace(find_Freq_max, replace_Freq_max)
    rf_contents[4] = rf_contents[4].replace(find_Freq_min, replace_Freq_min)
    rf_contents[5] = rf_contents[5].replace(find_Freq_nTotal, replace_Freq_nTotal)
    rf_contents[8] = rf_contents[8].replace(find_reservoirTopLeft_x, replace_reservoirTopLeft_x)
    rf_contents[9] = rf_contents[9].replace(find_reservoirTopLeft_y, replace_reservoirTopLeft_y)
    rf_contents[12] = rf_contents[12].replace(find_reservoirBottomRight_x, replace_reservoirBottomRight_x)
    rf_contents[13] = rf_contents[13].replace(find_reservoirBottomRight_y, replace_reservoirBottomRight_y)
    rf_contents[16] = rf_contents[16].replace(find_sourcesTopLeft_x, replace_sourcesTopLeft_x)
    rf_contents[17] = rf_contents[17].replace(find_sourcesTopLeft_y, replace_sourcesTopLeft_y)
    rf_contents[20] = rf_contents[20].replace(find_sourcesBottomRight_x, replace_sourcesBottomRight_x)
    rf_contents[21] = rf_contents[21].replace(find_sourcesBottomRight_y, replace_sourcesBottomRight_y)
    rf_contents[24] = rf_contents[24].replace(find_receiverTopLeft_x, replace_receiverTopLeft_x)
    rf_contents[25] = rf_contents[25].replace(find_receiverTopLeft_y, replace_receiverTopLeft_y)
    rf_contents[28] = rf_contents[28].replace(find_receiverBottomRight_x, replace_receiverBottomRight_x)
    rf_contents[29] = rf_contents[29].replace(find_receiverBottomRight_y, replace_receiverBottomRight_y)
    rf_contents[31] = rf_contents[31].replace(find_nSources, replace_nSources)
    rf_contents[32] = rf_contents[32].replace(find_nReceivers, replace_nReceivers)
    rf_contents[34] = rf_contents[34].replace(find_ngrid_x, replace_ngrid_x)
    rf_contents[35] = rf_contents[35].replace(find_ngrid_y, replace_ngrid_y)
    rf_contents[37] = rf_contents[37].replace(find_verbosity, replace_verbosity)
    rf_contents[38] = rf_contents[38].replace(find_fileName, replace_fileName)

    with open(input_file_GenericInput, 'w') as input_file_GenericInput:
        input_file_GenericInput.write(''.join(rf_contents)) # Joining list as a string

    ### Replace input values of IntegralFMInput.json file
    find_Iter2_n = '15'
    replace_Iter2_n = Iter2_n
    find_Iter2_tolerance = '5.0e-5'
    replace_Iter2_tolerance = Iter2_tolerance
    find_Iter2_calcAlpha = 'false'
    replace_Iter2_calcAlpha = Iter2_calcAlpha

    input_file_IntegralFMInput = os.path.join(tempTestPath + '/input/IntegralFMInput.json')

    with open(input_file_IntegralFMInput, 'r') as rf:
        rf_contents = rf.readlines() # Read all lines as a list
    # Replace values
    rf_contents[2] = rf_contents[2].replace(find_Iter2_n, replace_Iter2_n)
    rf_contents[3] = rf_contents[3].replace(find_Iter2_tolerance, replace_Iter2_tolerance)
    rf_contents[4] = rf_contents[4].replace(find_Iter2_calcAlpha, replace_Iter2_calcAlpha)

    with open(input_file_IntegralFMInput, 'w') as input_file_IntegralFMInput:
        input_file_IntegralFMInput.write("".join(rf_contents)) # Joining list as a string
    return tempTestPath, InversionMethod, Direction, ForwardModel

def preProcess(tempTestPath, forwardModel):
    print('\n------Start PreProcess')
    forwardModelArgument = ""
    if(forwardModel == 'IntegralForwardModel'):
        forwardModelArgument = 'Integral'
    elif(forwardModel == 'FiniteDifferenceForwardModel'):
        forwardModelArgument = 'Finite_Differnce'
    else:
        print('Invalid Forward Model')
        exit()
    start_preTime = time.time()
    tempFWIInstallBin = os.path.join(root + '/FWIInstall/bin/')
    os.chdir(tempFWIInstallBin)
    
    os.system('./FWI_PreProcess_Integral ' + tempTestPath)
    end_preTime = time.time()
    return datetime.timedelta(seconds=(end_preTime - start_preTime))

def Process(tempTestPath, Inversion, ForwardModel):
    print('\n------Start Process')
    start_processTime = time.time()
    print('./FWI_UnifiedProcess ' + tempTestPath + ' ' + Inversion + ' ' + ForwardModel)
    os.system('./FWI_UnifiedProcess ' + tempTestPath + ' ' + Inversion + ' ' + ForwardModel)
    end_processTime = time.time()
    return datetime.timedelta(seconds=(end_processTime - start_processTime))

def postProcess(tempTestPath):
    print('\n------Start PostProcess')
    start_postTime = time.time()
    os.chdir(tempFWIInstall)
    os.system('python3 postProcessing-python3.py ' + tempTestPath + ' 0')
    end_postTime = time.time()
    return datetime.timedelta(seconds=(end_postTime - start_postTime))


if __name__ == "__main__":
    if len(sys.argv) == 3:
        start_totalTime = time.time()

        rowStart = sys.argv[1]
        rowEnd = sys.argv[2]
        testData = "inputParameters.csv"
        n = int(rowStart)
        executionFolder = makeExecutionFolder()

        # Define paths
        root = os.path.dirname(os.getcwd())  # /home/user
        basename = os.getcwd()  # /home/user/FWITest
        input_file = os.path.join(basename, testData)  #/home/user/FWITest/inputParameters.csv
        tempFWIInstall = os.path.join(root, 'FWIInstall')  #/home/user/FWIInstall
        tempDefaultPath = os.path.join(basename, 'default/')  #/home/user/FWITest/default
        tempfileNameTestPath = os.path.join(basename, executionFolder, 'input')  #/home/user/FWITest/Execution_yyyyMMdd-hhmmss/input

        # Methods
        with open(input_file, 'r') as csvfile:
            data = list(csv.reader(csvfile, delimiter=","))
        while n <= int(rowEnd):  # row[0] is the header of the colomns
            result = executionMethod()
            print('printing result')
            print(result)
            preTime = preProcess(result[0], result[3])
            processTime = Process(result[0], result[1], result[3])
            #processTime = StepDefinitionProcess(result[0], result[1], result[2], result[3])
            #postTime = postProcess(result[0])
            print('\nPreProcess time   {}'.format(preTime))
            print('Process time      {}'.format(processTime))
            #print('PostProcess time  {}'.format(postTime))
            n += 1

        end_totalTime = time.time()
        print('\nTotal execution time {}\n'.format(datetime.timedelta(seconds=(end_totalTime - start_totalTime))))
    else:
        print("Argument Syntax:")
        print("    row_start   Give the start range argument (the headers are ignored)")
        print("    row_end     Give the end range argument (if you want to run 1 test, give the same input value as 'row_start'")
        print("\nExample: python <executionFile.py> <row_start> <row_end>")
