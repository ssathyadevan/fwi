import csv
import argparse
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from skimage.transform import resize

def readRunName(path):
    parameter_file = open(path + "/output/lastRunName.txt", "r")
    return parameter_file.readline()

def readParameter(label, path):
    run_name = readRunName(path)
    parameter_file = open(path + "/output/" + run_name + ".pythonIn", "r")
    line = [x for x in parameter_file if label in x ][0]
    return line.split()[-1]

def filename(path, prefix = "", suffix = ".txt"):
    runName = readRunName(path)
    filename = path + "/output/" + prefix + runName + suffix
    return filename

# Configure the argument parser
argumentParser = argparse.ArgumentParser()

# Input arguments
argumentParser.add_argument("-o", "--output", type=str, required=True,
    help="Path to output directory.")
argumentParser.add_argument("-r", "--run_number", type=int, required=False,
    default=0, help="Run number (default: 0)")

# Parse the input arguments
arguments = vars(argumentParser.parse_args())

# Enter here the name of the case folder you want to post process
outputPath = arguments['output']
run_number = arguments['run_number']

# Read run parameters from files
nxt = int(readParameter(label="nxt", path = outputPath))
nzt = int(readParameter(label="nzt", path = outputPath))

nxt_original = int(readParameter(label="nxt_original", path = outputPath))
nzt_original = int(readParameter(label="nzt_original", path = outputPath))

# Start image set up for original image
chi1 = np.genfromtxt(filename(path = outputPath, prefix = "chi_ref_"))
chi1 = chi1.reshape((nzt_original, nxt_original))

# Start image set up for reconstructed image
chi2 = np.genfromtxt(filename(path = outputPath, prefix = "chi_est_"))
chi2 = chi2.reshape((nzt, nxt))

# We upscale the smaller image to avoid information loss
chi1_original = chi1
chi2_original = chi2
if nxt_original > nxt:
    chi2 = resize(chi2, (nzt_original, nxt_original), mode='reflect')
    nxt = nxt_original
    nzt = nzt_original
elif (nxt > nxt_original):
    chi1 = resize(chi1, (nzt, nxt), mode='reflect')
    nxt_original = nxt
    nzt_original = nzt

# We compute the error per pixel, the square mean of the original image and the mse
diff_chi = chi2-chi1
mse = (np.square(diff_chi)).mean()
square_mean_original = (np.square(chi1)).mean()
avg_relative_error = np.sqrt(mse)/np.sqrt(square_mean_original)*100
total_seconds = readParameter("CPU time", path = outputPath)

print("The MSE (mean square error) is:       " + str(mse))
print("The average relative error is:        " + str(avg_relative_error))
print("Execution time in seconds:            " + total_seconds)

# Set the minimum and maximum values to chi
v_min = chi1.min()
v_max = chi1.max()

######################################
####  Create plots of actual data ####
######################################
plt.clf
plt.subplots_adjust(hspace=0.5)

plt.subplot(3, 1, 1)
plt.title("Chi values in original reservoir")
plt.imshow(chi1_original, interpolation='nearest', vmin=v_min, vmax=v_max)
plt.colorbar()

plt.subplot(3, 1, 2)
plt.title("Chi values in reconstructed reservoir")
plt.imshow(chi2_original, interpolation='nearest', vmin=v_min, vmax=v_max)
plt.colorbar()

plt.subplot(3, 1, 3)
plt.title("Difference between chi values")
plt.imshow(np.absolute(diff_chi), interpolation='nearest')
plt.colorbar()

plt.savefig(filename(path = outputPath, suffix = "Result.png"), dpi=400, bbox_inches='tight')

#######################################
####  Create plot of the residuals ####
#######################################
reader_residual = csv.reader(open(filename(path=outputPath, suffix = "Residual.log")), delimiter=',')
dummy_variable_reader_pr = list(reader_residual)
reader_residual_array = np.array(dummy_variable_reader_pr).astype("float")

plt.clf()
residuals = []
[residuals.append(val[0]) for val in reader_residual_array]
residuals.insert(0, None)

plt.plot(residuals)
plt.ylabel("Residual")
plt.grid(True)
plt.savefig(filename(path = outputPath, suffix="Residual.png"), dpi=400)

print("The pictures have been generated with Python")