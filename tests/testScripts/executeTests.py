import os, sys, shutil, csv, datetime, time
import platform
import argparse
import ast
import json
from datetime import datetime as dt
from pathlib import Path

def tableElementByKey(key, seq, params):
    value = "false"
    labels = params[0]
    if key in labels:
        column = labels.index(key)
        value = params[seq][column]
    return value

def guess_type(x):
    attempt_type_conversion = [ast.literal_eval,
                               float,
                               int, 
                               bool
                              ]

    for fn in attempt_type_conversion:
        try:
            return fn(x)
        except (ValueError, SyntaxError):
            pass
    return x

def writeJson(type, js, path):
    jsonTypes = { "CG": "ConjugateGradientInversionInput.json" }
    json_file = os.path.join(path, 'input', jsonTypes[type])
    with open(json_file, 'w') as outfile:
        json.dump(js, outfile, sort_keys = True, indent = 4)

def createJsonByType(type, seq, params, path):
    labels = params[0]
    selection = [x for x in labels if type in x ]

    js = {}
    names = []
    keys = []
    for s in selection:
        key = s.split(":")
        names.append(key[1])
        keys.append(key)

    # Extract unique names
    names = list(set(names))

    # Generate json for requested type
    for name in names:
        value = {}
        for key in keys:
            if (key[1] == name):
                if (len(key) > 2):
                    value[key[2]] = guess_type(tableElementByKey(":".join(key), seq, params))
                    js[name] = value
                else:
                    js[name] = guess_type(tableElementByKey(":".join(key), seq, params))

    writeJson(type, js, path)


def makeExecutionFolder():
    print('\n------Creating a new execution folder')
    now = dt.now()
    execution_stamp = 'Execution_' + now.strftime("%Y%m%d_%H%M%S")
    os.mkdir(execution_stamp)
    print("Name: " + execution_stamp)
    return execution_stamp

def makeTestFolder(testPath):
    print('\n------Create test folder')
    #os.makedirs(os.path.join(testPath,'input'), exist_ok=True)
    #os.makedirs(os.path.join(testPath,'output'), exist_ok=True)
    shutil.copytree(os.path.join(root, 'inputFiles', 'default'), testPath)
    shutil.copy(os.path.join(basename, testParams), testPath)

def executionMethod():
    number_current = n - int(rowStart) + 1 # The current test cycle
    number_total = int(rowEnd) - int(rowStart) + 1 # The total amount of tests to be executed
    print("""
##################################
      Start Test(s) {} of {}
##################################""".format(number_current, number_total))

    ### Read some meta data from csv file
    sequence = tableElementByKey(key = "sequence", seq = n, params = data)
    title = tableElementByKey(key = "title", seq = n, params = data )
    testSequenceFolder = sequence.zfill(2) + title
    testPath = os.path.join(basename, executionFolder, testSequenceFolder)

    makeTestFolder(testPath)

    ### ConjugateGradientInput.json
    createJsonByType(type="CG", seq = n, params = data, path = testPath)

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

    ### IntegralFMIInput.json
    Iter2_n = data[n][30]
    Iter2_tolerance = data[n][31]
    Iter2_calcAlpha = data[n][32]
    Iter2_CostFunction = data[n][33]

    ### GradientDescentInversionInput.json
    gamma = data[n][34]
    x0 = data[n][35]
    h = data[n][36]
    iter = data[n][37]

    ### FiniteDifferenceFMInput.json
    PMLWidthFactor_x = data[n][38]
    PMLWidthFactor_z = data[n][39]
    SourceParameter_r = data[n][40]
    SourceParameter_beta = data[n][41]
    CostFunction = data[n][42]
    boundaryConditionType = data[n][43]

    ### Mode
    InversionMethod = data[n][44]
    ForwardModel = data[n][45]

    #makeTestFolder(testPath)

    print('\n------Changing input values')

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

    input_file_GenericInput = os.path.join(testPath + '/input/GenericInput.json')

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
        input_file_GenericInput.write(''.join(rf_contents))

    ### Replace input values of IntegralFMInput.json file
    find_Iter2_n = '15'
    replace_Iter2_n = Iter2_n
    find_Iter2_tolerance = '5.0e-5'
    replace_Iter2_tolerance = Iter2_tolerance
    find_Iter2_calcAlpha = 'false'
    replace_Iter2_calcAlpha = Iter2_calcAlpha
    find_Iter2_CostFunction = 'leastSquares'
    replace_Iter2_CostFunction = Iter2_CostFunction

    input_file_IntegralFMInput = os.path.join(testPath + '/input/IntegralFMInput.json')

    with open(input_file_IntegralFMInput, 'r') as rf:
        rf_contents = rf.readlines() # Read all lines as a list
    # Replace values
    rf_contents[2] = rf_contents[2].replace(find_Iter2_n, replace_Iter2_n)
    rf_contents[3] = rf_contents[3].replace(find_Iter2_tolerance, replace_Iter2_tolerance)
    rf_contents[5] = rf_contents[5].replace(find_Iter2_CostFunction, replace_Iter2_CostFunction)

    with open(input_file_IntegralFMInput, 'w') as input_file_IntegralFMInput:
        input_file_IntegralFMInput.write("".join(rf_contents)) # Joining list as a string

    ### Replace input values of FiniteDifferenceFMInput.json file
    
    find_PMLWidthFactor_x = '0.0'
    replace_PMLWidthFactor_x = PMLWidthFactor_x
    find_PMLWidthFactor_z = '0.0'
    replace_PMLWidthFactor_z = PMLWidthFactor_z
    find_SourceParameter_r = '4'
    replace_SourceParameter_r = SourceParameter_r
    find_SourceParameter_beta = '6.31'
    replace_SourceParameter_beta = SourceParameter_beta
    find_CostFunction = 'leastSquares'
    replace_CostFunction = CostFunction
    find_boundaryConditionType = 'SecondOrderABC'
    replace_boundaryConditionType = boundaryConditionType

    input_file_FiniteDifferenceFMInput = os.path.join(testPath + '/input/FiniteDifferenceFMInput.json')

    with open(input_file_FiniteDifferenceFMInput, 'r') as rf:
        rf_contents = rf.readlines() # Read all lines as a list
    # Replace values
    rf_contents[2] = rf_contents[2].replace(find_PMLWidthFactor_x, replace_PMLWidthFactor_x)
    rf_contents[3] = rf_contents[3].replace(find_PMLWidthFactor_z, replace_PMLWidthFactor_z)
    rf_contents[6] = rf_contents[6].replace(find_SourceParameter_r, find_SourceParameter_r)
    rf_contents[7] = rf_contents[7].replace(find_SourceParameter_beta, replace_SourceParameter_beta)
    rf_contents[9] = rf_contents[9].replace(find_CostFunction, replace_CostFunction)
    rf_contents[10] = rf_contents[10].replace(find_boundaryConditionType, replace_boundaryConditionType)

    with open(input_file_FiniteDifferenceFMInput, 'w') as input_file_FiniteDifferenceFMInput:
        input_file_FiniteDifferenceFMInput.write("".join(rf_contents)) 

    ### Replace input values of GradientDescentInversionInput.json file
    find_gamma = '0.1'
    replace_gamma = gamma
    find_x0 = '0.001'
    replace_x0 = x0
    find_h = '0.001'
    replace_h = h
    find_iter = '20'
    replace_iter = iter
    
    input_file_GradientDescentInversionInput = os.path.join(testPath + '/input/GradientDescentInversionInput.json')

    with open(input_file_GradientDescentInversionInput, 'r') as rf:
        rf_contents = rf.readlines() # Read all lines as a list
    # Replace values
    rf_contents[1] = rf_contents[1].replace(find_gamma, replace_gamma)
    rf_contents[2] = rf_contents[2].replace(find_x0, replace_x0)
    rf_contents[3] = rf_contents[3].replace(find_h, replace_h)
    rf_contents[4] = rf_contents[4].replace(find_iter, replace_iter)
    
    with open(input_file_GradientDescentInversionInput, 'w') as input_file_GradientDescentInversionInput:
        input_file_GradientDescentInversionInput.write("".join(rf_contents)) 
    
    return testPath, InversionMethod, ForwardModel

def preProcess(tempTestPath, forwardModel):
    print('\n------Start PreProcess')
    forwardModelArgument = ""
    if(forwardModel == 'IntegralForwardModel'):
        forwardModelArgument = "FWI_PreProcess_Integral"
    elif(forwardModel == 'FiniteDifferenceForwardModel'):
        forwardModelArgument = "FWI_PreProcess_Finite_Difference"
    else:
        print('Invalid Forward Model')
        exit()
    start_preTime = time.time()
    tempFWIInstallBin = os.path.join(root,'FWIInstall','bin')

    execPath = os.path.join(tempFWIInstallBin, forwardModelArgument)
    print(forwardModelArgument + ' ' + tempTestPath)

    os.system(execPath + ' ' + Path(tempTestPath).as_posix())
    end_preTime = time.time()
    return datetime.timedelta(seconds=(end_preTime - start_preTime))

def Process(testPath, Inversion, ForwardModel):
    print('\n------Start Process')
    start_processTime = time.time()
    print('FWI_UnifiedProcess ' + testPath + ' ' + Inversion + ' ' + ForwardModel)
    execPath = os.path.join(os.path.join(root,'FWIInstall','bin'), "FWI_UnifiedProcess")
    os.system(execPath + ' ' + Path(testPath).as_posix() + ' ' + Inversion + ' ' + ForwardModel)
    end_processTime = time.time()
    return datetime.timedelta(seconds=(end_processTime - start_processTime))

def postProcess(testPath):
    print('\n------Start PostProcess')
    start_postTime = time.time()

    execName = 'python3'
    if (platform.system() == 'Windows'):
        execName = 'python'
    execPath = os.path.join(root, 'pythonScripts', 'postProcessing-python3.py')

    os.system(execName + ' ' + execPath + ' ' + '-o' + testPath)

    end_postTime = time.time()
    return datetime.timedelta(seconds=(end_postTime - start_postTime))


if __name__ == "__main__":

    # Configure the argument parser
    parser = argparse.ArgumentParser(description="Execute specified range of test cases.")

    # Input arguments
    parser.add_argument("-s", "--start", type=int, required=False,
                                default = 1, choices = range(1,51), metavar = "[1-50]", help = "Start of the range argument for the test cases.")
    parser.add_argument("-e", "--end", type=int, required=False,
                                default = 1, choices = range(1,51), metavar = "[1-50]", help = "End of the range argument for the test cases.")
    parser.add_argument("-i", "--input", type=str, required=True, help = "Input parameters file.")

    # Parse the input arguments
    arguments = vars(parser.parse_args())

    rowStart = arguments["start"]
    rowEnd = arguments["end"]
    testParams = arguments["input"]
    n = rowStart

    start_totalTime = time.time()

    executionFolder = makeExecutionFolder()

    # Define paths
    root = os.path.dirname(os.getcwd()) # parallelized-fwi/
    basename = os.getcwd()              # parallelized-fwi/FWITest/

    with open(Path(testParams), 'r') as csvfile:
        data = list(csv.reader(csvfile, delimiter=","))

    while n <= rowEnd:
        testPath, inversionMethod, forwardModel = executionMethod()
        preTime = preProcess(testPath, forwardModel)
        processTime = Process(testPath, inversionMethod, forwardModel)
        postTime = postProcess(testPath)

        if (tableElementByKey(key = "doRegression", seq = n, params = data).upper() == "TRUE"):
            print('\n------Start Regression Test')
            print("...")

        print('\nPreProcess time   {}'.format(preTime))
        print('Process time      {}'.format(processTime))
        print('PostProcess time  {}'.format(postTime))
        n += 1

    end_totalTime = time.time()
    print('\nTotal execution time {}\n'.format(datetime.timedelta(seconds=(end_totalTime - start_totalTime))))