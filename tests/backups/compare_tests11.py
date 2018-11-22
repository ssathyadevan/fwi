#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, filecmp, csv, numpy, matplotlib.pyplot as plt

print ""
if (len(sys.argv) != 3):
    print "please supply two arguments after python compare_tests.py\n\
the first argument the benchmark, the second argument the new test"
    sys.exit()
else:
    bench   = sys.argv[1]
    new     = sys.argv[2]
    print "You supplied two arguments, well done\n"
    print "Your benchmark: " + sys.argv[1]
    print "Your new test:  " + sys.argv[2]
    benchin = sys.argv[1]+".in"
    newin   = sys.argv[2]+".in"
bench_dot_in_input_exists = os.path.isfile(benchin)
new_dot_in_input_exists = os.path.isfile(newin)
print ""

if bench_dot_in_input_exists:
    print "Your benchmark file exists"
else:
    print "Your benchmark does not exist"
    sys.exit()
print ""

if new_dot_in_input_exists:
    print "Your new test also exists"
else:
    print "You lack a new test to compare"
    sys.exit()
print ""

if (filecmp.cmp(benchin,newin)):
    print "Your benchmark and test parametrization are identical,\n\
but your reservoirs could be different"
    inputfilediff = False
else:
    print "There is a difference between how you parametrized the input,\n\
but your reservoirs could be identical"
    inputfilediff = True
print ""

chi_ref_bench_txt = "chi_ref_"       + bench + ".txt"
chi_ref_new_txt   = "chi_ref_"       + new   + ".txt"
chi_ref_bench_csv = "chi_reference_" + bench + ".csv"
chi_ref_new_csv   = "chi_ref_bench_" + new   + ".csv"

if (filecmp.cmp(chi_ref_bench_txt, chi_ref_new_txt)):
    print "You used the same reservoir in both instances !"
    rsrvrdiff = False
else:
    print "You used different reservoirs, exiting"
    rsrvrdiff = True
    sys.exit()
print ""

chi_est_bench_txt = "chi_est_" + bench + ".txt"
chi_est_new_txt =   "chi_est_" + new   + ".txt"
chi_est_bench_csv = "chi_est_" + bench + ".csv"
chi_est_new_csv =   "chi_est_" + new   + ".csv"

if (filecmp.cmp(chi_est_bench_txt, chi_est_new_txt)):
    print "It appears that you have the same output in terms of chi,\n\
so I exit, although this part of the code is extended later "
    sys.exit()
else:
    print "You have different output in terms of chi,\n\
    we will study them in more detail"

reader_prfct_rsrvr = csv.reader(open(chi_ref_bench_csv),delimiter=',')
dummy_variable_reader_pr = list(reader_prfct_rsrvr)
prfct_rsrvr_chi_array = numpy.array(dummy_variable_reader_pr).astype("float")

resolution_of_rsrvr = \
numpy.max(prfct_rsrvr_chi_array) - numpy.min(prfct_rsrvr_chi_array) 

ncols = prfct_rsrvr_chi_array.shape[0]
nrows = prfct_rsrvr_chi_array.shape[1]

print "\n The difference between the max and min value\n\
in your INPUT reservoir chi is: " + str(resolution_of_rsrvr) 
print "The number of columns is " + str(ncols)
print "The number of rows is "    + str(nrows) +"\n"

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
#numpy.set_printoptions(formatter={'float_kind':float_formatter})

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

numpy.savetxt(
"diff_chi_perfect_and_bench.csv",diff_prfct_and_bench,fmt='%.2f',delimiter=', '
)

plt.clf
plt.tight_layout()
plt.subplot(2,2,1)
plt.text(64.,-18.,"Diff (%)") # MELISSEN 2018 11 19 has to be changed to relative coords
plt.xlabel("xgrid point")
plt.ylabel("ygrid point")
plt.subplots_adjust(
left = 0.10, right = 0.95, bottom = 0.1, top = 0.9, wspace=0.3, hspace=0.3)
im = plt.imshow(diff_prfct_and_bench)
myBar = plt.colorbar(im)
plt.title("Difference between\n perfect reservoir\n and benchmark")
plt.subplot(2,2,2)
plt.text(64.,-18.,"Diff(%)")
plt.xlabel("xgrid point")
plt.ylabel("ygrid point")
plt.imshow(diff_prfct_and_new)
myBar = plt.colorbar()
plt.title("Difference between\n perfect reservoir\n and new test")
plt.subplot(2,2,3)
plt.text(64.,-18.,"Diff(%)")
plt.xlabel("xgrid point")
plt.ylabel("ygrid point")
plt.imshow(diff_new_and_bench)
plt.colorbar()
plt.title("Difference between\n benchmark and\n new test")
plt.subplot(2,2,4)
plt.text(64.,-18.,"Chi(-)")
plt.xlabel("xgrid point")
plt.ylabel("ygrid point")
plt.imshow(prfct_rsrvr_chi_array)
myBar  = plt.colorbar()
plt.title("Chi values of \n actual reservoir")
plt.savefig("hi2.png",dpi=400)

#print numpy.max(z)
#print numpy.min(z)
#print numpy.argmax(z)
#print ""
#print numpy.unravel_index(z.argmax(),z.shape)
#print numpy.unravel_index(z.argmin(),z.shape)
