#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Script to compare different implementations of the forward and inversion model by 
# running different PreProcess and Process applications on a given test case
# This script should be called in the ~/FWIInstall folder
# 

import matplotlib
matplotlib.use('Agg')
import argparse, subprocess, os, sys, csv, numpy, shutil, matplotlib.pyplot as plt, filecmp
from datetime import datetime


# Parse command line arguments
parser = argparse.ArgumentParser(description="Script to compare different implementations of the \
forward and inversion model by running different PreProcess and Process applications on a given test case",\
formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("newpreprocess",help="Path of new PreProcess application")
parser.add_argument("newprocess",help="Path of new Process application")
parser.add_argument("testcase",help="Path of test case")
parser.add_argument("-rpp","--refpreprocess",help="Path of reference PreProcess application", default="bin/FWI_PreProcess")
parser.add_argument("-rp","--refprocess",help="Path of reference Process application", default="bin/FWI_Process")
args=parser.parse_args()

newpreprocess = args.newpreprocess
newprocess    = args.newprocess
testcase 	  = args.testcase
refpreprocess = args.refpreprocess
refprocess	  = args.refprocess

if (testcase.endswith("/")):
	testcase = testcase[:-1]

testcasenew   = testcase + "NEW"

testcase_exists	 		= os.path.exists(testcase+"/")
newpreprocess_exists 	= os.path.isfile(newpreprocess)
newprocess_exists 		= os.path.isfile(newprocess)
refpreprocess_exists 	= os.path.isfile(refpreprocess)
refprocess_exists 		= os.path.isfile(refprocess)

if testcase_exists:
	print("Testcase                                          OK")
else:
	print("Testcase                                          ERROR")
	sys.exit()
if newpreprocess_exists:
	print("New PreProcess application                        OK")
else:
	print("New PreProcess application                        ERROR")
	sys.exit()
if newprocess_exists:
	print("New Process application                           OK")
else:
	print("New Process application                           ERROR")
	sys.exit()
if refpreprocess_exists:
	print("Reference PreProcess application                  OK")
else:
	print("Reference PreProcess application                  ERROR")
	sys.exit()
if refprocess_exists:
	print("Reference Process application                     OK")
else:
	print("Reference Process application                     ERROR")
	sys.exit()

print("")

try:
	shuignore = shutil.ignore_patterns('*fast*')
	shutil.copytree(testcase +"/", testcase + "NEW/",ignore = shuignore)
except: 
	pass

print("##################################################################################")
print("#                         PreProcess Analysis                                    #")
print("##################################################################################")
print("")

print("Running: " + refpreprocess + " " + testcase)
run_ref_preprocess = subprocess.Popen("/usr/bin/time -o timeoutput.txt -f '%M' " + refpreprocess + \
	                                  " " + testcase, shell = True, stdout=subprocess.PIPE)
run_ref_preprocess.wait()

# OBTAIN RAM USAGE
time_output_file = open("timeoutput.txt")
ref_preprocess_ram = float(time_output_file.readline())
time_output_file.close()

print("Running: " + newpreprocess + " " + testcasenew)
run_new_preprocess = subprocess.Popen("/usr/bin/time -o timeoutput.txt -f '%M' " + newpreprocess + \
	                                  " " + testcasenew, shell = True, stdout=subprocess.PIPE)
run_new_preprocess.wait()

# OBTAIN RAM USAGE
time_output_file = open("timeoutput.txt")
new_preprocess_ram = float(time_output_file.readline())
time_output_file.close()

print("")
print("########################### PERFORMANCE COMPARISON ##############################")
def findWallClocktime(substr,whichin):
    lines           = [x for x in open(whichin) if substr in x]
    line            = lines[0]
    manip           = line.replace(substr,'').replace("\n",'')
    start_or_finish = (datetime.strptime(manip,'%c'))
    return start_or_finish 

def findCPUtime(substr,whichin):
    lines           = [x for x in open(whichin) if substr in x]
    line            = lines[0]
    manip           = line.replace(substr,'').replace("\n",'')
    manip = manip.split()[0]
    manip = float(manip)
    return manip

ref_CPU_time = findCPUtime("CPU time: ", testcase + "/output/" + testcase + "PreProcess.out")
new_CPU_time = findCPUtime("CPU time: ", testcasenew + "/output/" + testcasenew + "PreProcess.out")

datetime_bench_start  = findWallClocktime("Starting at ", testcase + "/output/" + testcase + "PreProcess.out")
datetime_bench_finish = findWallClocktime("Finished at ", testcase + "/output/" + testcase + "PreProcess.out")
bench_total_seconds   = (datetime_bench_finish - datetime_bench_start).seconds 
datetime_new_start    = findWallClocktime("Starting at ", testcasenew + "/output/"+ testcasenew + "PreProcess.out")
datetime_new_finish   = findWallClocktime("Finished at ", testcasenew + "/output/" +testcasenew + "PreProcess.out")
new_total_seconds   = (datetime_new_finish - datetime_new_start).seconds 

print("Wall clock time it took to do reference run:      %0.0i seconds" % bench_total_seconds)
print("Wall clock time it took to do new run:            %0.0i seconds" % new_total_seconds)
print("Speedup of new run compared to reference run:     %0.2f" % \
       (float(bench_total_seconds)/float(new_total_seconds)))
print("CPU time it took to do reference run:             %0.1f seconds" % ref_CPU_time)
print("CPU time it took to do new run:                   %0.1f seconds" % new_CPU_time)
print("RAM usage for reference run:                      %0.1f Mb" % (ref_preprocess_ram/1000))
print("RAM usage for new run:                            %0.1f Mb" % (new_preprocess_ram/1000))

print("")
print("############################# OUTPUT COMPARISON #################################")
ref_preprocess_csv = testcase + "/output/" + testcase + "InvertedChiToPressure.txt"
new_preprocess_csv = testcasenew + "/output/" + testcasenew + "InvertedChiToPressure.txt"

ref_preprocess_ictp = csv.reader(open(ref_preprocess_csv), delimiter=',')
new_preprocess_ictp = csv.reader(open(new_preprocess_csv), delimiter=',')

ref_preprocess_array = numpy.array(list(ref_preprocess_ictp)).astype("float")
new_preprocess_array = numpy.array(list(new_preprocess_ictp)).astype("float")
	
ref_preprocess_array = ref_preprocess_array[...,0] + 1j * ref_preprocess_array[...,1]
new_preprocess_array = new_preprocess_array[...,0] + 1j * new_preprocess_array[...,1]

diff_preprocess_array = ref_preprocess_array - new_preprocess_array

# CALCULATE MEAN ABSOLUTE DEVIATION AND ROOT MEAN SQUARED ERROR
preprocess_mad  = numpy.absolute(diff_preprocess_array).mean()
preprocess_rmse = numpy.sqrt(numpy.square(numpy.absolute(diff_preprocess_array)).mean())
preprocess_infnorm = numpy.linalg.norm(diff_preprocess_array, numpy.inf) \
					/ numpy.linalg.norm(ref_preprocess_array, numpy.inf)
preprocess_2norm = numpy.linalg.norm(diff_preprocess_array, 2) \
					/ numpy.linalg.norm(ref_preprocess_array, 2)
preprocess_1norm = numpy.linalg.norm(diff_preprocess_array, 1) \
					/ numpy.linalg.norm(ref_preprocess_array, 1)

print("The MAD between reference and new:                %0.2f" % preprocess_mad)
print("The RMSE between reference and new:               %0.2f" % preprocess_rmse)
print("Relative Inf-norm w.r.t. ref:                     %0.2f" % preprocess_infnorm)
print("Relative 2-norm w.r.t. ref:                       %0.2f" % preprocess_2norm)
print("Relative 1-norm w.r.t. ref:                       %0.2f" % preprocess_1norm)

print("")
print("##################################################################################")
print("#                            Process Analysis                                    #")
print("##################################################################################")
print("")

print("Running: " + refprocess + " " + testcase)
run_ref_process = subprocess.Popen("/usr/bin/time -o timeoutput.txt -f '%M' " + refprocess + \
	                                  " " + testcase, shell = True, stdout=subprocess.PIPE)
run_ref_process.wait()

# OBTAIN RAM USAGE
time_output_file = open("timeoutput.txt")
ref_process_ram = float(time_output_file.readline())
time_output_file.close()

print("Running: " + newprocess + " " + testcase)
run_new_process = subprocess.Popen("/usr/bin/time -o timeoutput.txt -f '%M' " + newprocess + \
	                                  " " + testcasenew, shell = True, stdout=subprocess.PIPE)
run_new_process.wait()

# OBTAIN RAM USAGE AND DELETE TEMP FILE
time_output_file = open("timeoutput.txt")
new_process_ram = float(time_output_file.readline())
time_output_file.close()
os.remove("timeoutput.txt")

print("")
print("########################### PERFORMANCE COMPARISON ##############################")
print("")

ref_CPU_time = findCPUtime("CPU time: ", testcase + "/output/" + testcase + "Process.out")
new_CPU_time = findCPUtime("CPU time: ", testcasenew + "/output/" + testcasenew + "Process.out")

datetime_bench_start  = findWallClocktime("Starting at ", testcase + "/output/" + testcase + "Process.out")
datetime_bench_finish = findWallClocktime("Finished at ", testcase + "/output/" + testcase + "Process.out")
bench_total_seconds   = (datetime_bench_finish - datetime_bench_start).seconds 
datetime_new_start    = findWallClocktime("Starting at ", testcasenew + "/output/"+ testcasenew + "Process.out")
datetime_new_finish   = findWallClocktime("Finished at ", testcasenew + "/output/" +testcasenew + "Process.out")
new_total_seconds   = (datetime_new_finish - datetime_new_start).seconds 

print("Wall clock time it took to do reference run:      %0.0i seconds" % bench_total_seconds)
print("Wall clock time it took to do new run:            %0.0i seconds" % new_total_seconds)
print("Speedup of new run compared to reference run:     %0.2f" % \
       (float(bench_total_seconds)/float(new_total_seconds)))
print("CPU time it took to do reference run:             %0.1f seconds" % ref_CPU_time)
print("CPU time it took to do new run:                   %0.1f seconds" % new_CPU_time)
print("RAM usage for reference run:                      %0.1f Mb" % (ref_preprocess_ram/1000))
print("RAM usage for new run:                            %0.1f Mb" % (new_preprocess_ram/1000))

print("")
print("############################# OUTPUT COMPARISON #################################")

chi_perfect = testcase + "/output/chi_reference_" + testcase + ".csv"
perfect_reservoir = csv.reader(open(chi_perfect),delimiter=',')
perfect_reservoir = numpy.array(list(perfect_reservoir)).astype("float")

chi_ref = testcase + "/output/chi_est_" + testcase + ".csv"
ref_reservoir = csv.reader(open(chi_ref),delimiter=',')
ref_reservoir = numpy.array(list(ref_reservoir)).astype("float")

chi_new = testcasenew + "/output/chi_est_" + testcasenew + ".csv"
new_reservoir = csv.reader(open(chi_new),delimiter=',')
new_reservoir = numpy.array(list(new_reservoir)).astype("float")

diff_ref_new = ref_reservoir - new_reservoir
diff_per_ref = perfect_reservoir - ref_reservoir
diff_per_new = perfect_reservoir - new_reservoir

ref_new_mad  = numpy.absolute(diff_ref_new).mean()
ref_new_rmse = numpy.sqrt(numpy.square(diff_ref_new).mean())
ref_new_infnorm = numpy.linalg.norm(diff_ref_new, numpy.inf) \
					/ numpy.linalg.norm(ref_reservoir, numpy.inf)
ref_new_2norm = numpy.linalg.norm(diff_ref_new, 2) \
					/ numpy.linalg.norm(ref_reservoir, 2)
ref_new_1norm = numpy.linalg.norm(diff_ref_new, 1) \
					/ numpy.linalg.norm(ref_reservoir, 1)

print("----------------------------- Reference vs. New ---------------------------------")
print("The MAD between reference and new:                %0.2f" % ref_new_mad)
print("The RMSE between reference and new:               %0.2f" % ref_new_rmse)
print("Relative Inf-norm w.r.t. ref:                     %0.2f" % ref_new_infnorm)
print("Relative 2-norm w.r.t. ref:                       %0.2f" % ref_new_2norm)
print("Relative 1-norm w.r.t. ref:                       %0.2f" % ref_new_1norm)

per_ref_mad  = numpy.absolute(diff_per_ref).mean()
per_ref_rmse = numpy.sqrt(numpy.square(diff_per_ref).mean())
per_ref_infnorm = numpy.linalg.norm(diff_per_ref, numpy.inf) \
					/ numpy.linalg.norm(perfect_reservoir, numpy.inf)
per_ref_2norm = numpy.linalg.norm(diff_per_ref, 2) \
					/ numpy.linalg.norm(perfect_reservoir, 2)
per_ref_1norm = numpy.linalg.norm(diff_per_ref, 1) \
					/ numpy.linalg.norm(perfect_reservoir, 1)

print("--------------------------- Perfect vs. Reference --------------------------------")
print("The MAD between perfect and reference             %0.2f" % per_ref_mad)
print("The RMSE between perfect and reference            %0.2f" % per_ref_rmse)
print("Relative Inf-norm w.r.t. perfect                  %0.2f" % per_ref_infnorm)
print("Relative 2-norm w.r.t. perfect:                   %0.2f" % per_ref_2norm)
print("Relative 1-norm w.r.t. perfect:                   %0.2f" % per_ref_1norm)

per_new_mad  = numpy.absolute(diff_per_new).mean()
per_new_rmse = numpy.sqrt(numpy.square(diff_per_new).mean())
per_new_infnorm = numpy.linalg.norm(diff_per_new, numpy.inf) \
					/ numpy.linalg.norm(perfect_reservoir, numpy.inf)
per_new_2norm = numpy.linalg.norm(diff_per_new, 2) \
					/ numpy.linalg.norm(perfect_reservoir, 2)
per_new_1norm = numpy.linalg.norm(diff_per_new, 1) \
					/ numpy.linalg.norm(perfect_reservoir, 1)

print("------------------------------ Perfect vs. New -----------------------------------")
print("The MAD between perfect and new                   %0.2f" % per_new_mad)
print("The RMSE between perfect and new                  %0.2f" % per_new_rmse)
print("Relative Inf-norm w.r.t. new                      %0.2f" % per_new_infnorm)
print("Relative 2-norm w.r.t. new:                       %0.2f" % per_new_2norm)
print("Relative 1-norm w.r.t. new:                       %0.2f" % per_new_1norm)

numerics_filename_per_ref = "diff_chi_perfect_and_ref_"+testcase+".csv"
numerics_filename_per_new = "diff_chi_perfect_and_new_"+testcase+".csv"
numerics_filename_ref_new = "diff_chi_ref_and_new_"+testcase+".csv"
numpy.savetxt(
numerics_filename_per_ref,diff_per_ref,fmt='%.8f',delimiter=', '
)
numpy.savetxt(
numerics_filename_per_new,diff_per_new,fmt='%.8f',delimiter=', '
)
numpy.savetxt(
numerics_filename_ref_new, diff_ref_new,fmt='%.8f',delimiter=', '
)

print("------------------------------- Visualisation ------------------------------------")

imagefilename="ref_new_"+testcase+"_compare.png"

print("Generating plot...                                " + imagefilename)

matplotlib.rcParams.update({'font.size':6})

plt.clf
plt.tight_layout()
plt.subplots_adjust(
left = 0.10, right = 0.95, bottom = 0.1, top = 0.9, wspace=0.3, hspace=0.35
)
stringDescribingColorbar = "Diff(%)"
for i in range (1,7):
    plt.subplot(3,2,i)
    if (i >= 4):
        stringDescribingColorbar = "Chi (-)"
    plt.text(64.,38.,stringDescribingColorbar)
    plt.xlabel("xgrid point", labelpad = 1)
    plt.ylabel("ygrid point")
    if (i == 1):
        plt.imshow(diff_per_ref)
        plt.title("Diff. between perfect reservoir and benchmark")
    elif (i == 2):                
        plt.imshow(diff_per_new)
        plt.title("Diff. between perfect reservoir and new test")
    elif (i == 3):
        plt.imshow(diff_ref_new)
        plt.title("Diff. between benchmark and new test")
    elif (i == 4):
        plt.imshow(perfect_reservoir)
        plt.title("Chi values of actual reservoir")
    elif (i == 5):
        plt.imshow(ref_reservoir)
        plt.title("Chi values of benchmark calculation")
    else:
        plt.imshow(new_reservoir)
        plt.title("Chi values of new calculation")
    myBar = plt.colorbar()
plt.savefig(imagefilename,dpi=400)

print("Plot generated                                    OK")

sys.exit(0)
