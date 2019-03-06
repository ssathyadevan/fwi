#!/usr/bin/python
# -*- coding: utf-8 -*-
import matplotlib
matplotlib.use('Agg')
import sys, os, difflib, filecmp, csv, numpy, matplotlib, shutil, unittest, matplotlib.pyplot as plt
from datetime import datetime


if ((len(sys.argv) > 3) or (len(sys.argv) < 2)):
    print("please supply two or three arguments after python compare_tests.py\n\
the first argument the new test, the second argument the benchmark\n")
    sys.exit()
else:
    run          = sys.argv[2]

if (len(sys.argv) == 3):
    reference   = sys.argv[1]

preProcess_ref_csv = reference + "/" + reference + "InvertedChiToPressure.txt" 
preProcess_run_csv = run + "/" + run + "InvertedChiToPressure.txt"


preProcess_ref_csv_exists = os.path.isfile(preProcess_ref_csv)
preProcess_run_csv_exists = os.path.isfile(preProcess_run_csv)

if preProcess_ref_csv_exists:
    print("Your benchmark file exists")
else:
    print("Your benchmark does not exist. Please run the FWI scripts and refer the benchmark argument to the output")
    sys.exit()

if preProcess_run_csv_exists:
    print("Your new test also exists")
else:
    print("")
    print("You lack a new test to compare")
    sys.exit()
print("")


reader_reference_preProcess = csv.reader(open(preProcess_ref_csv),delimiter=',')
dummy_variable_reader_pr = list(reader_reference_preProcess)
reader_reference_preProcess_array = numpy.array(dummy_variable_reader_pr).astype("float")

ncols_ref = reader_reference_preProcess_array.shape[0]
nrows_ref = reader_reference_preProcess_array.shape[1]



reader_run_preProcess = csv.reader(open(preProcess_run_csv),delimiter=',')
dummy_variable_reader_pr = list(reader_run_preProcess)
reader_run_preProcess_array = numpy.array(dummy_variable_reader_pr).astype("float")

ncols = reader_run_preProcess_array.shape[0]
nrows = reader_run_preProcess_array.shape[1]


if (ncols_ref != ncols) or (nrows_ref != nrows):
    print("The size of your inputs are different, hence can't be compared")
    sys.exit()
print("")


diff_run_and_reference = numpy.empty([ncols,nrows], dtype=float)

sum = 0.0
for i in range (0,reader_reference_preProcess_array.shape[0]):
    for j in range(0, reader_reference_preProcess_array.shape[1]):
        diff_run_and_reference[i,j] = \
        (
        (reader_run_preProcess_array[i,j] - reader_reference_preProcess_array[i,j])
        )
    sum += abs(diff_run_and_reference[i,j])



print("The mean value of the differences between run and benchmark:             " + str(diff_run_and_reference.mean()))
result1 = str(diff_run_and_reference.mean())

f= open("RegressionTest_Tolerance.txt","w+")
f.write(result1)
f.close()

print("The element wise summation the absolute differences between run and benchmark:             " + str(sum))

