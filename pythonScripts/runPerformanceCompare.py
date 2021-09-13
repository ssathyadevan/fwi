# This script runs created cases and saves performance data for comparison purposes

import os
import platform
import re
import numpy as np
import time
import csv
import argparse
from pathlib import Path
import json

# Configure argument parser
parser = argparse.ArgumentParser(description="Execute all Forward and Inversion models in one call.",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)


# Input Arguments
# forward model 1
# forward model 2
# inversion method 1
# inversion method 2
# # if parallel: num threads


parser.add_argument("-d", "--dir", type=str, required=False, default="./cases/",
                    help="Path to the folder containing input/output folders")


# Parse the input arguments
arguments = parser.parse_args()

#configure environment
execName = 'python3'
if platform.system() == 'Windows':
    execName = 'python'


cpu_time = []
wall_time = []
python_time = []
avg_error = []
mean_error = []
cores = []

#configure functions
def searchLogFilesForNumber(filename,word):
    important = ""
    with open(filename) as f:
        f = f.readlines()

    for line in f:
        if word in line:
            important = line
            break
    found = re.findall("\d+\.\d+", important)
    return found[-1]

def compareChiEst():
    chi_est = np.genfromtxt(location_chi_est)
    chi_ref = np.genfromtxt(location_chi_input)

    dif = chi_ref - chi_est
    mse = (np.square(dif)).mean()
    square_mean_original = (np.square(chi_ref)).mean()
    avg_relative_error = np.sqrt(mse) / np.sqrt(square_mean_original) * 100

    return mse,avg_relative_error

def setVariables(time,thread="1"):
    [mse, avg] = compareChiEst()
    avg_error.append(float(avg))
    mean_error.append(float(mse))
    python_time.append(float(time))
    cpu_time.append(float(searchLogFilesForNumber(location_log, "CPU")))
    cores.append(float(thread))
    wall_time.append(float(searchLogFilesForNumber(location_log, "Wall")))

casedir = arguments.dir

# change to different file input structure later
for root, dirs, files in os.walk(casedir):
#print(root)
    #print(dirs)
    #print(files)
    #print('-------------------------------')

    if re.search("/case[1-9]$", root):
    #    print('do_stuff')

        # read json input file
        inputFile = json.load(open(root + "/input/GenericInput.json"))

        forward = inputFile['forward']
        inversion = inputFile['inversion']
        model = inputFile['fileName']
        
        # Configure the arguments
        number_of_threads = inputFile['threads']
        location_input = root
        location_exe = "./bin"
        location_log = root + "/output/defaultProcess.log"
        print("location log: ", location_log)
        location_chi_input = root + "/output/chi_ref_default.txt"
        location_chi_est = root + "/output/chi_est_default.txt"
        final_location_csv = root + "./output/" + inversion + "_" + forward + "_" + model + ".csv"
        reference = ["FiniteDifference", "ConjugateGradient"]

        standard_args = execName + " runUtility.py -d " + location_input + " -b " + location_exe + " "  

        print(root, forward, inversion, model)

        #Start PreProcess
        print("Reference PreProcess with ForwardModel:" + reference[0])
        args = "--skip-post --skip-process -p " + reference[0]
        os.system(standard_args + args)

        #Start Process Reference
        print("Reference Process with ForwardModel: " + reference[0] + ' and Inversion: ' + reference[1])
        args = "--skip-pre --skip-post -f " + reference[0] + " -i " + reference[1]
        start = time.time()
        os.system(standard_args + args)
        end = time.time()
        setVariables(str(round(end-start,2)))

        for thread in number_of_threads:
            #set variable of threads
            parallel_command = "export OMP_NUM_THREADS=" + thread
            os.environ["OMP_NUM_THREADS"] = thread
            print(parallel_command)
            start = time.time()
            argsp = "--skip-pre --skip-post -f " + forward + " -i " + inversion
            if arguments.method == "MPI":
                argsp = argsp + " --MPI --threads " + str(thread)
            os.system(standard_args + argsp)
            end = time.time()
            setVariables(round(end-start,2),thread)

        #save variables
        final_array = np.asarray([cpu_time, wall_time,python_time,avg_error,mean_error,cores])
        np.savetxt(final_location_csv,final_array.transpose(),header="CPU,WALL,TIME,E_AVG,E_MEAN,CORES",delimiter=',')




