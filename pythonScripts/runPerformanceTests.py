#This script uses the runUtility.py to perform some Performance tests for Parallelization
#Run python3 runPerformanceTests.py -h for info

import os
import platform
import re
import numpy as np
import time
import csv
import argparse

parser = argparse.ArgumentParser(description="Execute runUtility with Parallelization",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)

# Input arguments
parser.add_argument("-m", "--method", type=str, required=False, default="OpenMP",
                    choices=["OpenMP", "MPI"],
                    help="Choice for comparison Parallelization")
parser.add_argument("-o", "--output", type=str, required=False,default="default",
                    help="Choice for output suffix file. Will be placed in output folder")
arguments = parser.parse_args()
#configure environment
execName = 'python3'
if platform.system() == 'Windows':
    execName = 'python'


# Configure the arguments
number_of_threads = ["1","2","4","6","8"]
location_input = "./default"
location_exe = "./bin"
location_log = "./default/output/defaultProcess.log"
location_chi_input = "./default/output/chi_ref_default.txt"
location_chi_est = "./default/output/chi_est_default.txt"
final_location_csv = location_input + "/output/parallel_" + arguments.method + "_"+arguments.output + ".csv"
reference = ["FiniteDifference", "ConjugateGradient"]

if arguments.method == "OpenMP":
    parallel = ["FiniteDifferenceParallel","ConjugateGradient"]
    standard_args = execName + " runUtility.py -d " + location_input + " -b " + location_exe + " "
if arguments.method == "MPI":
    parallel = ["FiniteDifferenceParallelMPI","ConjugateGradientMPI"]
    standard_args = execName + " runUtility.py -d " + location_input + " -b " + location_exe + " "



#Configure Empty Arrays
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
    argsp = "--skip-pre --skip-post -f " + parallel[0] + " -i " + parallel[1]
    if arguments.method == "MPI":
        argsp = argsp + " --MPI --threads " + str(thread)
    os.system(standard_args + argsp)
    end = time.time()
    setVariables(round(end-start,2),thread)

#save variables
final_array = np.asarray([cpu_time, wall_time,python_time,avg_error,mean_error,cores])
np.savetxt(final_location_csv,final_array.transpose(),header="CPU,WALL,TIME,E_AVG,E_MEAN,CORES",delimiter=',')









