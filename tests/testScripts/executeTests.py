import os, sys, shutil, csv, datetime, time
import platform
import argparse
import re
import json
from datetime import datetime as dt
from pathlib import Path

def tableElementByKey(key, seq, params):
    value = "false"
    labels = params[0]
    if key in labels:
        column = labels.index(key)
        value = params[seq][column]
    return guess_type(value)

def guess_type(s):
    if re.match("\A[0-9]+\Z", s):
        return int(s)
    elif re.match("[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?", s):
        return float(s)
    elif re.match("\A(true|false)\Z", s, re.IGNORECASE):
        return (s == "true")
    else:
        return str(s)

def writeJson(filename, data, path):
    json_file = os.path.join(path, 'input', filename)
    with open(json_file, 'w') as outfile:
        json.dump(data, outfile, sort_keys = True, indent = 4)

def createJsonByType(type, seq, params, path):
    labels = params[0]
    selection = [x for x in labels if type in x ]

    names = []
    keys = []
    for s in selection:
        key = s.split(":")
        names.append(key[1])
        keys.append(key)

    # Extract unique names
    names = list(set(names))

    # Generate json for requested type
    js = {}
    for name in names:
        value = {}
        for key in keys:
            if (key[1] == name):
                if (len(key) > 2):
                    value[key[2]] = tableElementByKey(":".join(key), seq, params)
                    js[name] = value
                else:
                    js[name] = tableElementByKey(":".join(key), seq, params)
    return js

def createInput(types, seq, params, path):
    for key, value in types.items():
        js = createJsonByType(key, n, params, path)
        writeJson(filename = value, data = js, path = path)

def makeExecutionFolder():
    print('\n------Creating a new execution folder', flush = True)
    now = dt.now()
    execution_stamp = 'Execution_' + now.strftime("%Y%m%d_%H%M%S")
    os.mkdir(execution_stamp)
    print("Name: " + execution_stamp)
    return execution_stamp

def makeTestFolder(testPath):
    print('\n------Create test folder', flush = True)
    shutil.copytree(os.path.join(root, 'inputFiles', 'default'), testPath)
    shutil.copy(os.path.join(basename, testParams), testPath)

def executionMethod():
    number_current = n - int(rowStart) + 1 # The current test cycle
    number_total = int(rowEnd) - int(rowStart) + 1 # The total amount of tests to be executed
    print("""
##################################
      Start Test(s) {} of {}
##################################""".format(number_current, number_total), flush = True)

    ### Read some meta data from csv file
    sequence = tableElementByKey(key = "sequence", seq = n, params = csv_data)
    title = tableElementByKey(key = "title", seq = n, params = csv_data )
    testSequenceFolder = str(sequence).zfill(2) + title
    testPath = os.path.join(basename, executionFolder, testSequenceFolder)

    makeTestFolder(testPath)

    inputTypes = { "CG": "ConjugateGradientInversionInput.json",
                   "GD": "GradientDescentInversionInput.json",
                   "GEN": "GenericInput.json" ,
                   "IFM": "IntegralFMInput.json" ,
                   "FDFM": "FiniteDifferenceFMInput.json" } 

    print('\n------Changing input values', flush = True)

    # Create input json's
    createInput(types = inputTypes, seq = n, params = csv_data, path = testPath)

    # Model and inversion types
    ForwardModel = tableElementByKey(key="ForwardModel", seq = n, params = csv_data)
    InversionMethod = tableElementByKey(key="InversionMethod", seq = n, params = csv_data)

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
        csv_data = list(csv.reader(csvfile, delimiter=","))

    while n <= rowEnd:
        testPath, inversionMethod, forwardModel = executionMethod()

        preTime = preProcess(testPath, forwardModel)
        processTime = Process(testPath, inversionMethod, forwardModel)
        postTime = postProcess(testPath)

        if (tableElementByKey(key = "doRegression", seq = n, params = csv_data)):
            print('\n------Start Regression Test', flush = True)
            print("...")

        print('\nPreProcess time   {}'.format(preTime))
        print('Process time      {}'.format(processTime))
        print('PostProcess time  {}'.format(postTime))
        n += 1

    end_totalTime = time.time()
    print('\nTotal execution time {}\n'.format(datetime.timedelta(seconds=(end_totalTime - start_totalTime))))