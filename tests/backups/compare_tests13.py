#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, filecmp, csv, numpy, matplotlib, matplotlib.pyplot as plt

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

print "\nThe difference between the max and min value\n\
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

numerics_filename_bench = "diff_chi_perfect_and_"+bench+".csv"
numerics_filename_new = "diff_chi_perfect_and_"+new+".csv"
numpy.savetxt(
numerics_filename_bench,diff_prfct_and_bench,fmt='%.2f',delimiter=', '
)
numpy.savetxt(
numerics_filename_new,diff_prfct_and_new,fmt='%.2f',delimiter=', '
)


#font = {'family' : 'normal',
#        'weight' : 'bold',
#        'size' : 12}
matplotlib.rcParams.update({'font.size':6})#('font',**font)

plt.clf
plt.tight_layout()

plt.subplot(3,2,1)
# MELISSEN 2018 11 19 has to be changed to relative coords:
plt.text(64.,38.,"Diff (%)")
plt.xlabel("xgrid point", labelpad = 1)
plt.ylabel("ygrid point")
plt.subplots_adjust(
left = 0.10, right = 0.95, bottom = 0.1, top = 0.9, wspace=0.3, hspace=0.35)
im = plt.imshow(diff_prfct_and_bench)
myBar = plt.colorbar(im)
plt.title("Diff. between perfect reservoir and benchmark")

plt.subplot(3,2,2)
plt.text(64.,38.,"Diff(%)")
plt.xlabel("xgrid point", labelpad = 1)
plt.ylabel("ygrid point")
plt.imshow(diff_prfct_and_new)
myBar = plt.colorbar()
plt.title("Diff. between perfect reservoir and new test")

plt.subplot(3,2,3)
plt.text(64.,38.,"Diff(%)")
plt.xlabel("xgrid point", labelpad = 1)
plt.ylabel("ygrid point")
plt.imshow(diff_new_and_bench)
plt.colorbar()
plt.title("Diff. between benchmark and new test")

plt.subplot(3,2,4)
plt.text(64.,38.,"Chi(-)")
plt.xlabel("xgrid point", labelpad = 1)
plt.ylabel("ygrid point")
plt.imshow(prfct_rsrvr_chi_array)
myBar  = plt.colorbar()
plt.title("Chi values of actual reservoir")
#plt.title("Diff. between perfect reservoir and new test")

plt.subplot(3,2,5)
plt.text(64.,38.,"Chi(-)")
plt.xlabel("xgrid point", labelpad = 1)
plt.ylabel("ygrid point")
plt.imshow(bench_est_chi_array)
plt.colorbar()
plt.title("Chi values of benchmark calculation")

plt.subplot(3,2,6)
plt.text(64.,38.,"Chi(-)")
plt.xlabel("xgrid point", labelpad = 1)
plt.ylabel("ygrid point")
plt.imshow(new_est_chi_array)
myBar  = plt.colorbar()
plt.title("Chi values of new calculation")

imagefilename=bench+"_"+new+"_compare.png"
plt.savefig(imagefilename,dpi=400)

print "Please look at the image file: " 
print imagefilename
print "for a visualization of the results and the following files"
print numerics_filename_bench# = "diff_chi_perfect_and_"+bench+".csv"
print numerics_filename_new# = "diff_chi_perfect_and_"+new+".csv"

print "\n\n\n"
print "Time for some statistics"
mse_bench = (numpy.square(prfct_rsrvr_chi_array - bench_est_chi_array)).mean()
print "The MSE of the benchmark calculation: "+str(mse_bench)
mse_new = (numpy.square(prfct_rsrvr_chi_array - new_est_chi_array)).mean()
print "The MSE of the new calculation: "+str(mse_new)

#DO NOT UNCOMMENT BELOW, this is the proof that the above works!

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
square_mean = square_mean_counter_during_loop/k
print mse2
#DO NOT UNCOMMENT ABOVE, this is the proof that the further above works!

print "The mean value of chi is: " + str(prfct_rsrvr_chi_array.mean())
print "The square mean value of chi is: " + str(square_mean)
#print prfct_rsrvr_chi_array.mean()# = numpy.array(dummy_variable_reader_pr).astype("float")

print "\nWe use the latter to determine a percentage for the MSE:"
print "Bench: " + str(float_formatter(numpy.sqrt(mse_bench/square_mean)*100)) + "%"
print "New:   " + str(float_formatter(numpy.sqrt(mse_new/square_mean)*100)) + "%\n"

print "\nMaximum deviation"
print "Bench: " + str(float_formatter(numpy.max(diff_prfct_and_bench))) + "%"
print "New:   " + str(float_formatter(numpy.max(diff_prfct_and_new))) + "%\n"

print "\nMinimum deviation"
print "Bench: " + str(float_formatter(numpy.min(diff_prfct_and_bench))) + "%"
print "New:   " + str(float_formatter(numpy.min(diff_prfct_and_new))) + "%\n"
