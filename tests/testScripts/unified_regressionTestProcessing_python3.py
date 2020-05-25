#!/usr/bin/python
# -*- coding: utf-8 -*-
import matplotlib
matplotlib.use('Agg')
import sys, os, difflib, filecmp, csv, numpy, matplotlib, shutil, matplotlib.pyplot as plt
from datetime import datetime

tolerance = 0.01
increased_performance_test_passed = False
increased_precision_test_passed = False
regression_test_passed = False

def print_regression_test_passed_message(regression_passed):
    print("\nREGRESSION TEST PASSED:       " + (str(regression_passed)).upper())

print("************************************************************")
print("                      PARSING ANALYSIS")
print("************************************************************\n")

if (len(sys.argv) != 5):
    print("please supply two or three arguments after python compare_tests.py\n\
the first argument the benchmark, the second argument the new test\n")
    sys.exit()
else:
    bench   = sys.argv[1]
    new     = sys.argv[2]
    inversionmodel = sys.argv[3]
    forwardmodel = sys.argv[4]

if (new.endswith("/")):
    new = new[:-1]

if (bench.endswith("/")):
    bench = bench[:-1]

if forwardmodel == "integralForwardModel":
    forwardinputfile = "IntegralFMInput.json"
elif forwardmodel == "finiteDifferenceForwardModel":
    forwardinputfile = "FiniteDifferenceFMInput.json"
else:
    print("Forward model is not recognized")
    sys.exit()

if inversionmodel == "conjugateGradientInversion":
    inversioninputfile = "ConjugateGradientInversionInput.json"
elif (inversionmodel == "gradientDescentInversion" or inversionmodel == "OpenmpgradientDescentInversion"):
    inversioninputfile = "GradientDescentInversionInput.json"
else:
    print("Inversion model is not recognized")
    sys.exit()

inputfiles = [inversioninputfile, forwardinputfile, "GenericInput.json"]

for i in range(0,len(inputfiles)):
    tempbench = bench +  "/input/" + inputfiles[i]
    tempnew =  new + "/input/" + inputfiles[i]
    bench_dot_in_input_exists = os.path.isfile(tempbench)
    new_dot_in_input_exists = os.path.isfile(tempnew)

    if bench_dot_in_input_exists:
        print("Your benchmark file " + inputfiles[i] + " exists")
    else:
        print("Your benchmark " + inputfiles[i] + " does not exist")
        sys.exit()
    
    if new_dot_in_input_exists:
        print("Your new " + inputfiles[i] + " test also exists")
    else:
        print("")
        print("You lack a new " + inputfiles[i] + " test to compare")
        sys.exit()
    print("")
    
    if (filecmp.cmp(tempbench,tempnew)):
        print("Your benchmark and test " + inputfiles[i] + " are identical,\n\
        but your reservoirs could be different\n")
        inputfilediff = False
    else:
        print("There is a difference between how you parametrized" + inputfiles[i] + ",\n\
                 but your reservoirs could be identical. First we see how they differ:\n")
        inputfilediff = True
        text1=open(tempbench).readlines()
        text2=open(tempnew).readlines()
        print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \
    - - - - - - - - - - - -")
        for line in difflib.unified_diff(text1,text2,fromfile=tempbench,tofile=tempnew):
            if (line != "\r"):
                sys.stdout.write(line)
        print(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \
    - - - - - - - - - - - -")
	
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

benchPyFile = open(bench + "/output/" + bench+".pythonIn","r") 
newPyFile = open(new + "/output/" + new+".pythonIn","r")
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



chi_ref_bench_txt =  bench + "/output/" + "chi_ref_"       + bench + ".txt"
chi_ref_new_txt   = new + "/output/" + "chi_ref_"       + new   + ".txt"
chi_ref_bench_csv = bench + "/output/" + "chi_reference_" + bench + ".csv"
chi_ref_new_csv   = new + "/output/" + "chi_ref_bench_" + new   + ".csv"

if (filecmp.cmp(chi_ref_bench_txt, chi_ref_new_txt)):
    print("You used the same reservoir in both instances !")
    rsrvrdiff = False
    regression_test_passed = True
else:
    print("You used different reservoirs, exiting")
    rsrvrdiff = True
    sys.exit()
print("")

chi_est_bench_txt = bench + "/output/" + "chi_est_" + bench + ".txt"
chi_est_new_txt =   new + "/output/" + "chi_est_" + new   + ".txt"
chi_est_bench_csv = bench + "/output/" + "chi_est_" + bench + ".csv"
chi_est_new_csv =   new + "/output/" + "chi_est_" + new   + ".csv"

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

resolution_of_rsrvr = numpy.max(prfct_rsrvr_chi_array) - numpy.min(prfct_rsrvr_chi_array)

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

resolution_of_bench = numpy.max(bench_est_chi_array) - numpy.min(bench_est_chi_array)

dummy_variable_reader_nw = list(read_new_est_chi)
new_est_chi_array = numpy.array(dummy_variable_reader_nw).astype("float")

diff_prfct_and_bench = numpy.empty([ncols,nrows], dtype=float)
diff_prfct_and_new = numpy.empty([ncols,nrows], dtype=float)
diff_bench_and_new = numpy.empty([ncols, nrows], dtype=float)

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
        diff_bench_and_new[i, j] = float_formatter \
        (
        (new_est_chi_array[i,j] - bench_est_chi_array[i,j]) \
        / resolution_of_bench * 100
        )
        if (abs(diff_bench_and_new[i, j]) > tolerance):
            regression_test_passed = False
            


numerics_filename_bench = "diff_chi_perfect_and_"+bench+".csv"
numerics_filename_new = "diff_chi_perfect_and_"+new+".csv"
numpy.savetxt(numerics_filename_bench,diff_prfct_and_bench,fmt='%.2f',delimiter=', ')
numpy.savetxt(numerics_filename_new,diff_prfct_and_new,fmt='%.2f',delimiter=', ')


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
        plt.imshow(diff_bench_and_new)
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

bench_est_chi_array_diff = numpy.square(prfct_rsrvr_chi_array - bench_est_chi_array)
new_est_chi_array_diff = numpy.square(prfct_rsrvr_chi_array - new_est_chi_array)
prfct_rsrvr_chi_array_squared = numpy.square(prfct_rsrvr_chi_array)

bench_est_chi_array_diff_mean = bench_est_chi_array_diff.mean()
new_est_chi_array_diff_mean = new_est_chi_array_diff.mean()
prfct_rsrvr_chi_array_mean = prfct_rsrvr_chi_array_squared.mean()

mse_bench = bench_est_chi_array_diff_mean
print("The MSE of the benchmark calculation: " + str(mse_bench))
mse_new = new_est_chi_array_diff_mean
print("The MSE of the new calculation:       " + str(mse_new))

vaf_bench = ((1 - (bench_est_chi_array_diff_mean / prfct_rsrvr_chi_array_mean)) * 100)
print("\nThe VAF of the benchmark calculation: " + str(vaf_bench) + "%")
vaf_new = ((1 - (new_est_chi_array_diff_mean / prfct_rsrvr_chi_array_mean)) * 100)
print("The VAF of the new calculation:       " + str(vaf_new) + "%")

fit_bench = (1 - (numpy.sqrt(bench_est_chi_array_diff_mean / prfct_rsrvr_chi_array_mean)) * 100)
print("\nThe FIT of the benchmark calculation: " + str(fit_bench) + "%")
fit_new = (1 - (numpy.sqrt(new_est_chi_array_diff_mean / prfct_rsrvr_chi_array_mean)) * 100)
print("The FIT of the new calculation:       " + str(fit_new) + "%")

# This piece of code is commented at it seems irrelevant to display.
# It show's the maximum and minimum relative deviation of the benchmark and new estimation
# relatively to the perfect data array.
# Decided not to delete this, as it might be useful in the future when we know what this data represents,
# and how to effectively use it.
#
# bench_max = numpy.max(diff_prfct_and_bench)
# new_max = numpy.max(diff_prfct_and_new)
# print("\nMaximum deviation")
# print("Bench: " + str(float_formatter(bench_max)) + "%")
# print("New:   " + str(float_formatter(new_max)) + "%\n")
#
# bench_min = numpy.min(diff_prfct_and_bench)
# new_min = numpy.min(diff_prfct_and_new)
# print("\nMinimum deviation")
# print("Bench: " + str(float_formatter(bench_min)) + "%")
# print("New:   " + str(float_formatter(new_min)) + "%")

if (1.001* mse_bench > mse_new):
    increased_precision_test_passed = True
print("\n\n************************************************************")
print("                    PERFORMANCE ANALYSIS                          ")
print("************************************************************\n")

def find(substr,whichin,date_format, file_type):
    from datetime import datetime
    lines           = [x for x in open(whichin+"Process."+file_type) if substr in x]
    line            = lines[0]
    manip = line.replace(substr, '').replace("\n", '')
    start_or_finish = (datetime.strptime(manip, date_format))
    return start_or_finish 

datetime_bench_start    = find(" INFO: Starting", bench + "/output/"+ bench, "%X.%f", "log")
datetime_bench_finish   = find(" INFO: Finished", bench + "/output/" +bench, "%X.%f", "log")
bench_total_seconds   = (datetime_bench_finish - datetime_bench_start).seconds 
datetime_new_start    = find(" INFO: Starting", new + "/output/"+ new, "%X.%f", "log")
datetime_new_finish   = find(" INFO: Finished", new + "/output/" +new, "%X.%f", "log")
new_total_seconds   = (datetime_new_finish - datetime_new_start).seconds 

if (1.01*bench_total_seconds > new_total_seconds):
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

if (regression_test_passed is True):
    s = 'True'
else:
    s = 'False'

f= open("RegressionTest_Passed.txt","w+")
f.write(str(s))
f.close()

print_regression_test_passed_message(s)

if (len(sys.argv) == 2):
    os.system("rm hiquality.*")
    os.system("rm chi*hiquality*")

