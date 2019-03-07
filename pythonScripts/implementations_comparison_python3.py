#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Script to compare different implementations of the forward and inversion model by 
# running different PreProcess and Process applications on a given test case
# This script should be called in the ~/FWIInstall folder
# 

import matplotlib
matplotlib.use('Agg')
import difflib, filecmp, matplotlib, shutil, matplotlib.pyplot as plt
from datetime import datetime

import argparse, subprocess, os, sys, csv, numpy, shutil

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
	print("Testcase exists")
else:
	print("Testcase does not exist!")
	sys.exit()
if newpreprocess_exists:
	print("New PreProcess application exists")
else:
	print("New PreProcess application does not exist!")
	sys.exit()
if newprocess_exists:
	print("New Process application exists")
else:
	print("New Process application does not exist!")
	sys.exit()
if refpreprocess_exists:
	print("Reference PreProcess application exists")
else:
	print("Reference PreProcess application does not exist!")
	sys.exit()
if refprocess_exists:
	print("Reference Process application exists")
else:
	print("Reference Process application does not exist!")
	sys.exit()

print("")

try:
	shuignore = shutil.ignore_patterns('*fast*')
	shutil.copytree(testcase +"/", testcase + "NEW/",ignore = shuignore)
except: 
	pass


##################################################################################
		# RUN BOTH PREPROCESS APPLICATIONS ON THE SAME TESTCASE
##################################################################################

print("Running: " + refpreprocess + " " + testcase)
run_ref_preprocess = subprocess.Popen(refpreprocess + " " + testcase, shell = True)
run_ref_preprocess.wait()

print("Running: " + newpreprocess + " " + testcase)
run_new_preprocess = subprocess.Popen(newpreprocess + " " + testcasenew + "/", shell = True)
run_new_preprocess.wait()

print("")
print("##################################################################################")
print("#                         PreProcess Analysis                                    #")
print("##################################################################################")
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

# CALCULATE MEAN ABSOLUTE DEVIATION AND ROOT MEAN SQUARED ERROR
preprocess_mad  = numpy.absolute(ref_preprocess_array - new_preprocess_array).mean()
preprocess_rmse = numpy.sqrt(numpy.absolute(numpy.square(ref_preprocess_array-new_preprocess_array)).mean())
preprocess_infnorm = numpy.linalg.norm(ref_preprocess_array - new_preprocess_array, numpy.inf) \
					/ numpy.linalg.norm(ref_preprocess_array, numpy.inf)
preprocess_2norm = numpy.linalg.norm(ref_preprocess_array - new_preprocess_array, 2) \
					/ numpy.linalg.norm(ref_preprocess_array, 2)
preprocess_1norm = numpy.linalg.norm(ref_preprocess_array - new_preprocess_array, 1) \
					/ numpy.linalg.norm(ref_preprocess_array, 1)

print("The MAD between reference and new :          " + str(preprocess_mad))
print("The RMSE between reference and new:          " + str(preprocess_rmse))
print("Maximum relative deviation compared to ref:  " + str(preprocess_infnorm))
print("Relative 2-norm deviation compared to ref:   " + str(preprocess_2norm))
print("Relative 1-norm deviation compared to ref:   " + str(preprocess_1norm))

preprocess_outputfile = open("RegressionTest_Tolerance.txt","w+")
preprocess_outputfile.write(str(preprocess_mad))
preprocess_outputfile.close()

print("")
print("########################### PERFORMANCE COMPARISON ##############################")
def find(substr,whichin):
    lines           = [x for x in open(whichin) if substr in x]
    line            = lines[0]
    manip           = line.replace(substr,'').replace("\n",'')
    start_or_finish = (datetime.strptime(manip,'%c'))
    return start_or_finish 

datetime_bench_start  = find("Starting at ", testcase + "/output/" + testcase + "PreProcess.out")
datetime_bench_finish = find("Finished at ", testcase + "/output/" + testcase + "PreProcess.out")
bench_total_seconds   = (datetime_bench_finish - datetime_bench_start).seconds 
datetime_new_start    = find("Starting at ", testcasenew + "/output/"+ testcasenew + "PreProcess.out")
datetime_new_finish   = find("Finished at ", testcasenew + "/output/" +testcasenew + "PreProcess.out")
new_total_seconds   = (datetime_new_finish - datetime_new_start).seconds 

if (bench_total_seconds > new_total_seconds):
    increased_performance_test_passed = True

print("Time in seconds it took to do reference run:     "+str(bench_total_seconds))
print("Time in seconds it took to do new run:           "+str(new_total_seconds))
print("Speedup of new run compared to reference run :   "+ \
    str(float(bench_total_seconds)/float(new_total_seconds)))

##################################################################################
			# RUN BOTH PROCESS APPLICATIONS ON THE SAME TESTCASE
##################################################################################

sys.exit(0)

print("")
print("Running: " + refprocess + " " + testcase)
run_ref_process = subprocess.Popen(refprocess + " " + testcase, shell = True)
run_ref_process.wait()

print("Running: " + newprocess + " " + testcase)
run_new_process = subprocess.Popen(newprocess + " " + testcasenew + "/", shell = True)
run_new_process.wait()

print("")
print("##################################################################################")
print("#                            Process Analysis                                    #")
print("##################################################################################")
print("")
print("############################# OUTPUT COMPARISON #################################")
print("")
print("########################### PERFORMANCE COMPARISON ##############################")

sys.exit(0)

tolerance = 0.01
increased_performance_test_passed = False
increased_precision_test_passed = False
regression_test_passed = False

def print_regression_test_passed_message(regression_passed):
    print("\nREGRESSION TEST PASSED:       " + (str(regression_passed)).upper())

# IMPORTANT COMMENT MELISSEN 2018 11 21
# basically I add the following lines because the .pythonIn is by its nature...
# more readable than the more flexible, more "user-piloted" input card.
# Because the .in contains all the variables, it is good to have the above...
# "diff" where the user of the regression test can see *for himself*...
# what the differences are, with the "diff" below really judging whether a
# comparison is possible...
# Essentially I encourage future programmers to write more information...
# into the .pythonIn files, the .in being user friendly and the .pythonIn...
# being parser friendly (...and used in such places as this...)

benchPyFile = open(bench + "/" + bench+".pythonIn","r") 
newPyFile = open(new + "/" + new+".pythonIn","r")
benchPyContent=benchPyFile.readlines()
newPyContent=newPyFile.readlines()

def extractValueFromPythonIn(line):
    varname,issymbol,value  = line.split()
    value = int(value)
    return value 

nxtbench                 = extractValueFromPythonIn(benchPyContent[1])
nxtnew                   = extractValueFromPythonIn(newPyContent[1]) 
nztbench                 = extractValueFromPythonIn(benchPyContent[2])
nztnew                   = extractValueFromPythonIn(newPyContent[2]) 

if(nxtbench != nxtnew):
    print("Your nxt values are not identical, exiting")
    print("")
    sys.exit()
if(nztbench != nztnew):
    print("Your nzt values are not identical, exiting")
    print("")
    sys.exit()

#nztbench = benchPyContent[3]



chi_ref_bench_txt = bench + "/" + "chi_ref_"       + bench + ".txt"
chi_ref_new_txt   = new + "/" + "chi_ref_"       + new   + ".txt"
chi_ref_bench_csv = bench + "/" + "chi_reference_" + bench + ".csv"
chi_ref_new_csv   = new + "/" + "chi_ref_bench_" + new   + ".csv"

if (filecmp.cmp(chi_ref_bench_txt, chi_ref_new_txt)):
    print("You used the same reservoir in both instances !")
    rsrvrdiff = False
    regression_test_passed = True
else:
    print("You used different reservoirs, exiting")
    rsrvrdiff = True
    sys.exit()
print("")

chi_est_bench_txt = bench + "/" + "chi_est_" + bench + ".txt"
chi_est_new_txt =   new + "/" + "chi_est_" + new   + ".txt"
chi_est_bench_csv = bench + "/" + "chi_est_" + bench + ".csv"
chi_est_new_csv =   new + "/" + "chi_est_" + new   + ".csv"

if (filecmp.cmp(chi_est_bench_txt, chi_est_new_txt)):
    print("It appears that you have the same output in terms of chi,\n\
so I exit, although this part of the code is extended later \n\
on formal grounds I will report that:")
    print_regression_test_passed_message(regression_test_passed)

    s = 'True'
    f= open("RegressionTest_Passed.txt","w+")
    f.write(str(s))
    f.close()

    sys.exit()
else:
    print("You have different output in terms of chi,\n\
we will study them in more detail")

print("\n************************************************************")
print("                      GENERAL ANALYSIS")
print("************************************************************")

reader_prfct_rsrvr = csv.reader(open(chi_ref_bench_csv),delimiter=',')
dummy_variable_reader_pr = list(reader_prfct_rsrvr)
prfct_rsrvr_chi_array = numpy.array(dummy_variable_reader_pr).astype("float")

resolution_of_rsrvr = \
numpy.max(prfct_rsrvr_chi_array) - numpy.min(prfct_rsrvr_chi_array) 

ncols = prfct_rsrvr_chi_array.shape[0]
nrows = prfct_rsrvr_chi_array.shape[1]

print("\nThe difference between the max and min value\n\
in your INPUT reservoir chi is: " + str(resolution_of_rsrvr)) 
print("The number of columns is " + str(ncols))
print("The number of rows is "    + str(nrows) +"\n")

read_bench_est_chi = csv.reader(open(chi_est_bench_csv),delimiter=',')
read_new_est_chi   = csv.reader(open(chi_est_new_csv),delimiter=',')

dummy_variable_reader_bn = list(read_bench_est_chi)
bench_est_chi_array = numpy.array(dummy_variable_reader_bn).astype("float")

resolution_of_bench = \
numpy.max(bench_est_chi_array) - numpy.min(bench_est_chi_array)

dummy_variable_reader_nw = list(read_new_est_chi)
new_est_chi_array = numpy.array(dummy_variable_reader_nw).astype("float")

diff_prfct_and_bench = numpy.empty([ncols,nrows], dtype=float)
diff_prfct_and_new = numpy.empty([ncols,nrows], dtype=float)
diff_new_and_bench = numpy.empty([ncols,nrows], dtype=float)

# We introduce a function to print numbers with only 2 decimals: more pleasant
float_formatter = lambda x : "%.2f" % x

for i in range (0,prfct_rsrvr_chi_array.shape[0]):
    for j in range(0, prfct_rsrvr_chi_array.shape[1]):
        diff_prfct_and_bench[i,j] = float_formatter \
        (
        (bench_est_chi_array[i,j] - prfct_rsrvr_chi_array[i,j]) \
        / resolution_of_rsrvr * 100
        )
        diff_prfct_and_new[i,j] = float_formatter \
        (
        (new_est_chi_array[i,j] - prfct_rsrvr_chi_array[i,j]) \
        / resolution_of_rsrvr * 100
        )
        diff_new_and_bench[i,j] = float_formatter \
        (
        (new_est_chi_array[i,j] - bench_est_chi_array[i,j]) \
        / resolution_of_bench * 100
        )
        if (abs(diff_new_and_bench[i,j]) > tolerance):
            regression_test_passed = False
            


numerics_filename_bench = "diff_chi_perfect_and_"+bench+".csv"
numerics_filename_new = "diff_chi_perfect_and_"+new+".csv"
numpy.savetxt(
numerics_filename_bench,diff_prfct_and_bench,fmt='%.2f',delimiter=', '
)
numpy.savetxt(
numerics_filename_new,diff_prfct_and_new,fmt='%.2f',delimiter=', '
)


matplotlib.rcParams.update({'font.size':6})

plt.clf
plt.tight_layout()
plt.subplots_adjust(
left = 0.10, right = 0.95, bottom = 0.1, top = 0.9, wspace=0.3, hspace=0.35
)
stringDescribingColorbar = "Diff(%)"
for i in range (1,7):
    plt.subplot(3,2,i)
    # MELISSEN 2018 11 19 has to be changed to relative coords:
    if (i >= 4):
        stringDescribingColorbar = "Chi (-)"
    plt.text(64.,38.,stringDescribingColorbar)
    plt.xlabel("xgrid point", labelpad = 1)
    plt.ylabel("ygrid point")
    if (i == 1):
        plt.imshow(diff_prfct_and_bench)
        plt.title("Diff. between perfect reservoir and benchmark")
    elif (i == 2):                
        plt.imshow(diff_prfct_and_new)
        plt.title("Diff. between perfect reservoir and new test")
    elif (i == 3):
        plt.imshow(diff_new_and_bench)
        plt.title("Diff. between benchmark and new test")
    elif (i == 4):
        plt.imshow(prfct_rsrvr_chi_array)
        plt.title("Chi values of actual reservoir")
    elif (i == 5):
        plt.imshow(bench_est_chi_array)
        plt.title("Chi values of benchmark calculation")
    else:
        plt.imshow(new_est_chi_array)
        plt.title("Chi values of new calculation")
    myBar = plt.colorbar()
imagefilename=bench+"_"+new+"_compare.png"
plt.savefig(imagefilename,dpi=400)

print("Please look at the image file: ") 
print(imagefilename)
print("for a visualization of the results and the following files")
print(numerics_filename_bench)
print(numerics_filename_new)

print("\n\n************************************************************")
print("                    STATISTICAL ANALYSIS")
print("************************************************************\n")

mse_bench = (numpy.square(prfct_rsrvr_chi_array - bench_est_chi_array)).mean()
print("The MSE of the benchmark calculation: "+str(mse_bench))
mse_new = (numpy.square(prfct_rsrvr_chi_array - new_est_chi_array)).mean()
print("The MSE of the new calculation:       "+str(mse_new))

k=0
sum_of_squares_during_loop = 0.0
square_mean_counter_during_loop = 0.0
for i in range (0,prfct_rsrvr_chi_array.shape[0]):
    for j in range(0, prfct_rsrvr_chi_array.shape[1]):
        k+=1
        sum_of_squares_during_loop+= \
        numpy.square(bench_est_chi_array[i,j]-prfct_rsrvr_chi_array[i,j])
        square_mean_counter_during_loop += \
        numpy.square(prfct_rsrvr_chi_array[i,j])

mse2 = sum_of_squares_during_loop/k
square_mean = numpy.sqrt(square_mean_counter_during_loop/k)

print("The mean value of chi is:             " + str(prfct_rsrvr_chi_array.mean()))
print("The square mean value of chi is:      " + str(square_mean))

print("\nWe use the latter to determine a percentage for the MSE:")
print("Bench: " + str(float_formatter(numpy.sqrt(mse_bench)/square_mean*100)) + "%")
print("New:   " + str(float_formatter(numpy.sqrt(mse_new)/square_mean*100)) + "%\n")

bench_max = numpy.max(diff_prfct_and_bench)
new_max = numpy.max(diff_prfct_and_new)
print("\nMaximum deviation")
print("Bench: " + str(float_formatter(bench_max)) + "%")
print("New:   " + str(float_formatter(new_max)) + "%\n")

bench_min = numpy.min(diff_prfct_and_bench)
new_min = numpy.min(diff_prfct_and_new)
print("\nMinimum deviation")
print("Bench: " + str(float_formatter(bench_min)) + "%")
print("New:   " + str(float_formatter(new_min)) + "%")

if ((mse_bench > mse_new) \
and (bench_max>new_max) \
and (abs(bench_min) > abs(new_min))):
    increased_precision_test = True
print("\n\n************************************************************")
print("                    PERFORMANCE ANALYSIS                          ")
print("************************************************************\n")

def find(substr,whichin):
    from datetime import datetime
    lines           = [x for x in open(whichin+"Process.out") if substr in x]
    line            = lines[0]
    manip           = line.replace(substr,'').replace("\n",'')
    start_or_finish = (datetime.strptime(manip,'%c'))
    return start_or_finish 

datetime_bench_start  = find("Starting at ", bench + "/" +bench)
datetime_bench_finish = find("Finished at ", bench + "/" +bench)
bench_total_seconds   = (datetime_bench_finish - datetime_bench_start).seconds 
datetime_new_start    = find("Starting at ", new + "/"+ new)
datetime_new_finish   = find("Finished at ", new + "/" +new)
new_total_seconds   = (datetime_new_finish - datetime_new_start).seconds 

if (bench_total_seconds > new_total_seconds):
    increased_performance_test_passed = True

print("Time in seconds it took to do benchmark run: "+str(bench_total_seconds))
print("Time in seconds it took to do new run:       "+str(new_total_seconds))
print("Ratio of bench run time to new run time:     "+ \
    str(float(bench_total_seconds)/float(new_total_seconds)))

print("\n\n************************************************************")
print("                      OVERALL ANALYSIS                          ")
print("************************************************************\n")
print("Increased overall precision:  " + str(increased_precision_test_passed))
print("Increased performance:        " + str(increased_performance_test_passed))

if regression_test_passed is True:
    s = 'True'
else:
    s = 'False'

f= open("RegressionTest_Passed.txt","w+")
f.write(str(s))
f.close()

print_regression_test_passed_message(regression_test_passed)

if (len(sys.argv) == 2):
    os.system("rm hiquality.*")
    os.system("rm chi*hiquality*")

