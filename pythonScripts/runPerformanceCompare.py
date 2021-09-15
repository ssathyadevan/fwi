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

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('Agg')
from scipy.ndimage.measurements import label
#import seaborn as sn

# Configure argument parser
parser = argparse.ArgumentParser(description="Execute all Forward and Inversion models in one call.",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)


# Input Arguments
# -d direct input to a folder containing case folders
# -c point to a csv file which contains input

parser.add_argument("-d", "--dir", type=str, required=False, default="./cases/",
                    help="Path to the folder containing input/output folders")

parser.add_argument("-c", "--casefile", type=str, required=False, default="./cases/casepaths.csv",
                    help="Path to csv file containg paths to cases")

# Parse the input arguments
arguments = parser.parse_args()

#configure environment
execName = 'python3'
if platform.system() == 'Windows':
    execName = 'python'

#prepare results csv

time_dir = time.strftime("%Y%m%d_%H:%M") + '_comparerun'
path = './' + time_dir + '/'
os.makedirs(time_dir, exist_ok=True)

results_csv = path + 'all_results.csv'
with open(results_csv, 'w+', newline='') as out_file:
    csv_writer = csv.writer(out_file)
    header = ["CASE","CPU","WALL","TIME","E_AVG","E_MEAN","CORES"]
    csv_writer.writerow(header)

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
    avg_error .append(float(avg))
    mean_error.append(float(mse))
    python_time.append(float(time))
    cpu_time.append(float(searchLogFilesForNumber(location_log, "CPU")))
    cores.append(float(thread))
    wall_time.append(float(searchLogFilesForNumber(location_log, "Wall")))

casedir = arguments.dir
caselist = []


def createCompareImages():

    results_df = pd.read_csv(results_csv)
    print(results_df)

    cpu_plot = plt.scatter(results_df.CASE, results_df.CPU)
    plt.xlabel("case")
    plt.ylabel("cpu_time")
    plt.savefig((path + "CPUImage.png"), dpi=400, bbox_inches='tight')
    # plt.show()
    plt.clf()

    wall_plot = plt.scatter(results_df.CASE, results_df.WALL)
    plt.xlabel("case")
    plt.ylabel("wall_time")
    plt.savefig((path + "WALLImage.png"), dpi=400, bbox_inches='tight')
    # plt.show()
    plt.clf()

    time_plot = plt.scatter(results_df.CASE, results_df.TIME)
    plt.xlabel("case")
    plt.ylabel("time")
    plt.savefig((path + "TIMEImage.png"), dpi=400, bbox_inches='tight')
    # plt.show()
    plt.clf()

    e_avg_plot = plt.scatter(results_df.CASE, results_df.E_AVG)
    plt.xlabel("case")
    plt.ylabel("e_avg")
    plt.savefig((path + "AVGImage.png"), dpi=400, bbox_inches='tight')
    # plt.show()
    plt.clf()

    e_mean_plot = plt.scatter(results_df.CASE, results_df.E_MEAN)
    plt.xlabel("case")
    plt.ylabel("e_mean")
    plt.savefig((path + "MEANImage.png"), dpi=400, bbox_inches='tight')
    # plt.show()
    plt.clf()

    core_plot = plt.scatter(results_df.CASE, results_df.CORES)
    plt.xlabel("case")
    plt.ylabel("cores")
    plt.savefig((path + "COREImage.png"), dpi=400, bbox_inches='tight')
    # plt.show()
    plt.clf()


with open(arguments.casefile) as csv_file:
    csv_reader = csv.reader(csv_file)
    line_count = 0
    for row in csv_reader:
        caselist.append(row[0])
        
print("Computing for: ")
for file in caselist:
    print(file)
print()

for root in caselist:
    cpu_time = []
    wall_time = []
    python_time = []    
    avg_error = []
    mean_error = []
    cores = []

    print("root: ", root)
    caseName = os.path.basename(os.path.normpath(root))

    # read json input file
    inputFile = json.load(open(root + "/input/GenericInput.json"))

    forward = inputFile['forward']
    inversion = inputFile['inversion']
    model = inputFile['fileName']
    
    print(root, forward, inversion, model)

    # Configure the arguments
    number_of_threads = inputFile['threads']
    location_input = root
    location_exe = "./bin"
    location_log = location_input + "/output/" + caseName +"Process.log"
    print("location log: ", location_log)
    location_chi_input = location_input + "/output/chi_ref_" + caseName + ".txt"
    location_chi_est = location_input + "/output/chi_est_" + caseName + ".txt"
    final_location_csv = location_input + "/output/" + inversion + "_" + forward + "_" + model + ".csv"
    #reference = ["FiniteDifference", "ConjugateGradient"]

    standard_args = execName + " runUtility.py -d " + location_input + " -b " + location_exe + " "  

    #for thread in number_of_threads:
    #set variable of threads
    parallel_command = "export OMP_NUM_THREADS=" + str(number_of_threads)
    os.environ["OMP_NUM_THREADS"] = str(number_of_threads)
    print(parallel_command)
    start = time.time()
    argsp = " -p" + forward + " -f " + forward + " -i " + inversion
    os.system(standard_args + argsp)
    end = time.time()
    setVariables(round(end-start,2),number_of_threads)

    #save variables
    final_array = np.asarray([cpu_time[0],wall_time[0],python_time[0],avg_error[0],mean_error[0],cores[0]])
    np.savetxt(final_location_csv,final_array.transpose(),header="CPU,WALL,TIME,E_AVG,E_MEAN,CORES",delimiter=',')

    with open(results_csv, 'a', newline='') as out_file:
        csv_writer = csv.writer(out_file)
        csv_writer.writerow([caseName, cpu_time[0], wall_time[0],python_time[0],avg_error[0],mean_error[0],cores[0]])

    createCompareImages()
