#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, filecmp, csv, numpy

print ""
if (len(sys.argv) != 3):
    print " please supply two arguments after python compare_tests.py\n \
the first argument the benchmark, the second argument the new test"
    sys.exit() # Back to the terminal, we are done
else:
    print " You supplied two arguments, well done"
    print ""
    print " Your benchmark: " + sys.argv[1]
    print " Your new test:  " + sys.argv[2]
    bench   = sys.argv[1]
    new     = sys.argv[2]
    benchin = sys.argv[1]+".in"
    newin   = sys.argv[2]+".in"
shouldexist1 = os.path.isfile(benchin)#sys.argv[1] + ".in")
shouldexist2 = os.path.isfile(newin)#sys.argv[2] + ".in")
print ""

if shouldexist1:
    print " Your benchmark file exists"
else:
    print " Your benchmark does not exist"
    sys.exit()
print ""

if shouldexist2:
    print " Your new test also exists"
else:
    print " You lack a new test to compare"
    sys.exit()
print ""

if (filecmp.cmp(benchin,newin)):
    print " Your benchmark and test parametrization are identical, but your reservoirs could be different"
    inputfilediff = False
else:
    print " There is a difference between how you parametrized the input, your reservoirs could be identical"
    inputfilediff = True
print ""

chi_ref_bench_txt = "chi_ref_"       + bench + ".txt"
chi_ref_new_txt   = "chi_ref_"       + new   + ".txt"
chi_ref_bench_csv = "chi_reference_" + bench + ".csv"
chi_ref_new_csv   = "chi_ref_bench_" + new   + ".csv"
if (filecmp.cmp(chi_ref_bench_txt, chi_ref_new_txt)):
    print " You used the same reservoir in both instances !"
    reservoirdiff = False
else:
    print " You used different reservoirs, exiting"
    reservoirdiff = True
    sys.exit()
print ""
chi_est_bench_txt = "chi_est_" + bench + ".txt"
chi_est_new_txt =   "chi_est_" + new   + ".txt"
chi_est_bench_csv = "chi_est_" + bench + ".csv"
chi_est_new_csv =   "chi_est_" + new   + ".csv"
if (filecmp.cmp(chi_est_bench_txt, chi_est_new_txt)):
    print " It appears that you have the same output in terms of chi, so I exit, \n \
although this part of the code is extended later "
    sys.exit()
else:
    print " You have different output in terms of chi, we will study them in more detail"

reader = csv.reader(open(chi_ref_bench_csv),delimiter=',')# map(float, fp)
x = list(reader)
resultx = numpy.array(x).astype("float")

max_chi_of_reference_input_reservoir = numpy.max(resultx)
min_chi_of_reference_input_reservoir = numpy.min(resultx)
resolution_of_bench=max_chi_of_reference_input_reservoir-min_chi_of_reference_input_reservoir
nxcols = resultx.shape[0]
nxrows = resultx.shape[1]
print ""
print " The difference between the max and min value in your INPUT reservoir chi is: " +str(resolution_of_bench) 

print ""
print " The number of columns is " +str(nxcols)
print " The number of rows is " +str(nxrows)
print ""
read_bench = csv.reader(open(chi_est_bench_csv),delimiter=',')
read_new   = csv.reader(open(chi_est_new_csv),delimiter=',')

y = list(read_bench)
resultsy = numpy.array(y).astype("float")

z = numpy.empty([nxcols,nxrows], dtype=float)

float_formatter = lambda x : "%.2f" % x
numpy.set_printoptions(formatter={'float_kind':float_formatter})
for i in range (0,resultx.shape[0]): #-1):
    for j in range(0, resultx.shape[1]): #-1):
        #z[i,j] = (resultsy[i,j] - resultx[i,j])/resolution_of_bench*100
        z[i,j] = float_formatter ( (resultsy[i,j] - resultx[i,j])/resolution_of_bench*100 )

numpy.savetxt("diff_chi.csv",z,fmt='%.2f',delimiter=', ')
   
print numpy.max(z)
print numpy.min(z)
print numpy.argmax(z)
print ""
print numpy.unravel_index(z.argmax(),z.shape)
print numpy.unravel_index(z.argmin(),z.shape)
#numpy.savetxt("diff_non_exp.csv",float_formatter(z),delimiter=',')
#print z
