import os, sys, shutil, csv, datetime, time
import platform
import argparse
import re
import json
import numpy as np
from datetime import datetime as dt
from pathlib import Path
import junit_xml_output

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

def filename(path, name, prefix = "", suffix = ".csv"):
    sequence = tableElementByKey(key = "sequence", seq = n, params = csv_data)
    title = tableElementByKey(key = "title", seq = n, params = csv_data )
    return os.path.join(path,prefix + name + suffix)

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
    now = dt.now()
    execution_stamp = 'Execution_' + now.strftime("%Y%m%d_%H%M%S")
    os.mkdir(execution_stamp)
    return execution_stamp

def makeTestFolder(testPath):
    shutil.copytree(os.path.join(root, 'inputFiles', 'default'), testPath)
    shutil.copy(os.path.join(basename, testParams), testPath)

def testMessage(name, current, total):
    lineWidth = 35
    print('\n'+'#'*lineWidth)
    print("    Start Test(s) {} of {}".format(current, total), flush = True)
    print("    Test Name: {}".format(name), flush = True)
    print('#'*lineWidth)

def regressionTest(name, path, tolerance):

    passed = False

    regression_data_path = os.path.join(root, 'tests', 'regression_data')
    test_data_path = os.path.join(path, 'output')

    # Read regression data
    fileName = filename(regression_data_path, name, 'chi_est_')
    if os.path.exists(fileName):
        with open(fileName) as csvfile:
            regression_data = np.array(list(csv.reader(csvfile, delimiter=","))).astype("float")
    else:
        print("Could not open/read the regression data file: {}".format(fileName), flush = True)
        return passed

    # Read current data
    with open(filename(test_data_path, name, 'chi_est_')) as csvfile:
        test_data = np.array(list(csv.reader(csvfile, delimiter=","))).astype("float")

    # Compare regression data with current data
    mean_square_error = np.square(test_data - regression_data).mean()
    mean_square_original = np.square(regression_data).mean()

    # Relative error in [%]
    relative_error = 0
    if (mean_square_original > 0):
        relative_error = np.sqrt(mean_square_error/mean_square_original)*100

    passed = relative_error < tolerance

    return passed

def generate_xml_report(test_cases):
    junit_xml = junit_xml_output.JunitXml("RegressionTests", test_cases)
    junit_xml_path = os.path.join(root,'build')
    with open(os.path.join(junit_xml_path, 'regressionTestResults.xml'), 'w') as f:
        f.write(junit_xml.dump())

def executionMethod():
    number_current = n - int(rowStart) + 1 # The current test cycle
    number_total = int(rowEnd) - int(rowStart) + 1 # The total amount of tests to be executed

    ### Read some meta data from csv file
    sequence = tableElementByKey(key = "sequence", seq = n, params = csv_data)
    title = tableElementByKey(key = "title", seq = n, params = csv_data )
    testName = str(sequence).zfill(2) + title

    testMessage(name = testName, current = number_current, total = number_total)

    testPath = os.path.join(basename, executionFolder, testName)

    makeTestFolder(testPath)

    inputTypes = { "CG": "ConjugateGradientInversionInput.json",
                   "GD": "GradientDescentInversionInput.json",
                   "GEN": "GenericInput.json" ,
                   "IFM": "IntegralFMInput.json" ,
                   "FDFM": "FiniteDifferenceFMInput.json" } 

    # Create input json's
    createInput(types = inputTypes, seq = n, params = csv_data, path = testPath)

    # Model and inversion types
    ForwardModel = tableElementByKey(key="ForwardModel", seq = n, params = csv_data)
    InversionMethod = tableElementByKey(key="InversionMethod", seq = n, params = csv_data)

    return testName, testPath, InversionMethod, ForwardModel

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

    test_cases = []
    while n <= rowEnd:
        testName, testPath, inversionMethod, forwardModel = executionMethod()

        preTime = preProcess(testPath, forwardModel)
        processTime = Process(testPath, inversionMethod, forwardModel)
        postTime = postProcess(testPath)

        if (tableElementByKey(key = "doRegression", seq = n, params = csv_data)):
            print('\n------Start Regression Test', flush = True)
            tolerance = tableElementByKey(key="regressionTolerance", seq = n, params = csv_data)
            passed = regressionTest(name = testName, path = testPath, tolerance  = tolerance)
            status = ""
            if (not passed): status = "failure"
            test_cases.append(junit_xml_output.TestCase("regression." + testName, "regression_test", status))

            print("PASSED: " + str(passed), flush = True)

        print('\nPreProcess time   {}'.format(preTime), flush = True)
        print('Process time      {}'.format(processTime), flush = True)
        print('PostProcess time  {}'.format(postTime), flush = True)
        n += 1

    generate_xml_report(test_cases)

    end_totalTime = time.time()
    print('\nTotal execution time {}\n'.format(datetime.timedelta(seconds=(end_totalTime - start_totalTime))))