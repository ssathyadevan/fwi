#!/usr/bin/python
# -*- coding: utf-8 -*-
import matplotlib
matplotlib.use('Agg')
import sys, os, difflib, filecmp, csv, numpy, matplotlib, shutil, matplotlib.pyplot as plt
from datetime import datetime


if (len(sys.argv) != 2):
    print("Please provide the folder with the results that need to be analysed")
    sys.exit()
else: #if (sys.argv==3):
    new     = sys.argv[1]

output_route= "./default/output/"
newPyFile = open(output_route + new+".pythonIn","r")
newPyContent=newPyFile.readlines()

def extractValueFromPythonIn(line):
    varname,issymbol,value  = line.split()
    value = int(value)
    return value 

nxtnew                   = extractValueFromPythonIn(newPyContent[1])
nztnew                   = extractValueFromPythonIn(newPyContent[2]) 


chi_ref_new_txt   = output_route + "chi_ref_"       + new   + ".txt"
chi_ref_new_csv   = output_route + "chi_reference_default"+ ".csv"

#here parse the input image name

chi_est_new_txt =   output_route + "chi_est_" + new   + ".txt"
chi_est_new_csv =   output_route + "chi_est_" + new   + ".csv"

print("\n************************************************************")
print("                      GENERAL ANALYSIS")
print("************************************************************")

reader_prfct_rsrvr = csv.reader(open(chi_ref_new_csv),delimiter=',')
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

read_new_est_chi   = csv.reader(open(chi_est_new_csv),delimiter=',')

dummy_variable_reader_nw = list(read_new_est_chi)
new_est_chi_array = numpy.array(dummy_variable_reader_nw).astype("float")

diff_prfct_and_new = numpy.empty([ncols,nrows], dtype=float)

# We introduce a function to print numbers with only 2 decimals: more pleasant
float_formatter = lambda x : "%.2f" % x

for i in range (0,prfct_rsrvr_chi_array.shape[0]):
    for j in range(0, prfct_rsrvr_chi_array.shape[1]):
        diff_prfct_and_new[i,j] = float_formatter \
        (
        (new_est_chi_array[i,j] - prfct_rsrvr_chi_array[i,j]) \
        / resolution_of_rsrvr * 100
        )


numerics_filename_new = "diff_chi_perfect_and_"+new+".csv"
numpy.savetxt(
numerics_filename_new,diff_prfct_and_new,fmt='%.2f',delimiter=', '
)

min_value=min(prfct_rsrvr_chi_array.min(),new_est_chi_array.min())
max_value=max(prfct_rsrvr_chi_array.max(),new_est_chi_array.max())
matplotlib.rcParams.update({'font.size':6})

plt.clf
#plt.tight_layout()
plt.subplots_adjust(hspace=0.5
)
stringDescribingColorbar = "Chi (-)"

plt.subplot(3,1,1)
plt.text(64.,38.,stringDescribingColorbar)
plt.xlabel("xgrid point", labelpad = 1)
plt.ylabel("ygrid point")
plt.imshow(prfct_rsrvr_chi_array)
plt.clim(min_value, max_value)
plt.title("Chi values of actual reservoir")
plt.colorbar()

plt.subplot(3,1,2)
plt.text(64.,38.,stringDescribingColorbar)
plt.xlabel("xgrid point", labelpad = 1)
plt.ylabel("ygrid point")
plt.imshow(new_est_chi_array)
plt.clim(min_value, max_value)
plt.title("Chi values of new calculation")
plt.colorbar()

plt.subplot(3,1,3)
stringDescribingColorbar = "Diff(%)"
plt.text(64.,38.,stringDescribingColorbar)
plt.xlabel("xgrid point", labelpad = 1)
plt.ylabel("ygrid point")
plt.imshow(diff_prfct_and_new)
plt.title("Diff. between perfect reservoir and new test")
plt.colorbar()


imagefilename=output_route + new+"_compare.png"
plt.savefig(imagefilename,dpi=400, bbox_inches='tight')

print("Please look at the image file: ") 
print(imagefilename)
print("for a visualization of the results and the following files")
print(numerics_filename_new)

print("\n\n************************************************************")
print("                    STATISTICAL ANALYSIS")
print("************************************************************\n")

mse_new = (numpy.square(prfct_rsrvr_chi_array - new_est_chi_array)).mean()
print("The MSE of the new calculation:       "+str(mse_new))

k=0
sum_of_squares_during_loop = 0.0
square_mean_counter_during_loop = 0.0
for i in range(0, prfct_rsrvr_chi_array.shape[0]):
    for j in range(0, prfct_rsrvr_chi_array.shape[1]):
        k += 1
        square_mean_counter_during_loop += \
        numpy.square(prfct_rsrvr_chi_array[i,j])


square_mean = numpy.sqrt(square_mean_counter_during_loop/k)

print("The mean value of chi (original image) is:             " + str(prfct_rsrvr_chi_array.mean()))
print("The square mean value of chi (original image) is:      " + str(square_mean))

print("\nAverage relative error (sqrt(sum_i^n(error_i^2)/sum_i^n(chi_i^2))):   " + str(float_formatter(numpy.sqrt(mse_new)/square_mean*100)) + "%\n")

new_max = numpy.max(diff_prfct_and_new)
print("\nMaximum deviation:   " + str(float_formatter(new_max)) + "%\n")

new_min = numpy.min(diff_prfct_and_new)
print("\nMinimum deviation:   " + str(float_formatter(new_min)) + "%")

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

datetime_new_start    = find("Starting at ", output_route+ new)
datetime_new_finish   = find("Finished at ", output_route +new)
new_total_seconds   = (datetime_new_finish - datetime_new_start).seconds

print("Time in seconds it took to do new run:       "+str(new_total_seconds))


##Residuals
residualLogFileName = output_route + new + "Residual.log"
reader_residual = csv.reader(open(residualLogFileName),delimiter=',')
dummy_variable_reader_pr = list(reader_residual)
reader_residual_array = np.array(dummy_variable_reader_pr).astype("float")

plt.clf()
plt.plot(reader_residual_array[:,0])
plt.ylabel("Residual")
plt.grid(True)
plt.savefig(outputPath+"/output/"+runName+"Residual.png", dpi=400)
