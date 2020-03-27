import sys, os, shutil
import numpy as np
from datetime import datetime
import matplotlib
import matplotlib.pyplot as plt
import bisect

matplotlib.use('Agg')
#os.system("export DISPLAY=:0.0")

######### Define all functions #################################################

def _makeFolder(folder):
    if os.path.exists(folder):
        shutil.rmtree(folder)
    os.makedirs(folder)


def _getThreads():
    """ Returns the number of available threads on a posix/win based system """
    if sys.platform == 'win32':
        return (int)(os.environ['NUMBER_OF_PROCESSORS'])
    else:
        return (int)(os.popen('grep -c cores /proc/cpuinfo').read())


def _setNumThreads(threads):
    os.environ["OMP_NUM_THREADS"] = "{}".format(threads)


def _modifyOutputFileNames(oldrunName, newrunName):
    outputLocation = os.path.join(newrunName, "output")
    for filename in os.listdir(outputLocation):
        source = os.path.join(outputLocation, filename)
        newfilename = filename.replace(oldrunName, newrunName)
        destination = os.path.join(outputLocation, newfilename)
        os.rename(source, destination)


def _getTime(substring, outputLocation):
    lines = [x for x in open(outputLocation + "Process.log") if substring in x]
    line = lines[0]
    manip = line.replace(substring, '').replace("\n", '')
    start_or_finish = (datetime.strptime(manip, "%X.%f"))
    return start_or_finish


def _getResiduals(outputLocation):
    residuals = []
    for line in open(outputLocation + "Residual.log"):
        residual, iterator = line.split(",")
        residual = float(residual)
        residuals.append(residual)
    residuals = np.array(residuals)
    return residuals

#######################################################################################


cwd = os.getcwd()

# Check the input
if len(sys.argv) > 1:
    folder = sys.argv[1]
else:
    print("Not enough input")
    print("Please give the runfolder")
    sys.exit(1)
print("Start parallel analysis")

# Initialize all locations
folderlocation = os.path.join(cwd, folder)
FWIInstalLocation = os.path.join(cwd, "FWIInstall")
applicationsLocation = os.path.join(FWIInstalLocation, "bin")
newFolder = "parallelAnalysis"
copylocation = os.path.join(FWIInstalLocation, newFolder)

print("Analyse with folder {}".format(folder))

# Create a new folder in FWIInstall for analysis
os.chdir(FWIInstalLocation)
_makeFolder(newFolder)

# Copy input of the input folder
splittedFolder = folder.strip("/").split("/")
if len(splittedFolder) > 1:
  runName = splittedFolder[-1]
else:
  runName = folder

PreprocessLocation = os.path.join(copylocation, runName)
shutil.copytree(os.path.join(folderlocation, "input"), os.path.join(PreprocessLocation, "input"))
print("{} is copied to {}".format(runName, copylocation))

# Create an empty output folder
os.chdir(PreprocessLocation)
_makeFolder("output")

# Run preprocessing
os.chdir(copylocation)
print("Run preprocessing...")
os.system(os.path.join(applicationsLocation, "FWI_PreProcess {}".format(runName)))

# Determine the threads we will be using
maximumThreads = _getThreads()
print("Maximal number of threads: {}".format(maximumThreads))
allThreadList = [1, 2, 4, 5, 8, 13, 16, 18, 20]
currentThreadList = []
if (maximumThreads < 5) :
    currentThreadList = range(1, 2*maximumThreads+1, 1)
    currentThreadList = list(currentThreadList)
elif (maximumThreads > 4 and maximumThreads < 20):
    # Get all thread numbers from allThreadList until one index after the maximal number of threads
    # (so with maximum 8 threads this list becomes [1, 2, 4, 5, 8, 13]
    index = bisect.bisect(allThreadList, maximumThreads)
    currentThreadList = allThreadList[:index+1]
else:
    currentThreadList = allThreadList

print("We will use maximal {} threads".format(currentThreadList[-1]))

# Run inversion in serial:
print("Run in serial...")
serialRunName = "{}Serial".format(runName)
shutil.copytree(PreprocessLocation, os.path.join(copylocation, serialRunName))
_modifyOutputFileNames(runName, serialRunName)
os.system(os.path.join(applicationsLocation, "FWI_Process_Gradient_Descent {}".format(serialRunName)))

# Get the runtime and residuals from the serial run
starttime_serial = _getTime(" INFO: Starting", serialRunName + "/output/" + serialRunName)
endtime_serial = _getTime(" INFO: Finished", serialRunName + "/output/" + serialRunName)
totalruntime_serial = (endtime_serial - starttime_serial).seconds

serialResiduals = _getResiduals(serialRunName + "/output/" + serialRunName)

# Create texts for runtime and residualDifference files
runtimeFileText = "Run \t \t Runtime (seconds) \t standard deviation\n" \
                  "Serial \t \t \t {} \n".format(totalruntime_serial)

residualDifferencetext = "Parallel run \t final residual difference  \t standard deviation\n"

# Initialize lists and the number of runs for each thread
allRunTimes = [totalruntime_serial]
allResidualDifferences = []
runtimeErrorBar = [0.0]
ResidualDifferencesErrorBar = []
maximumRuns = 2

# Run the method maximumRuns number of times in parallel
# for each number of threads and compute the mean and deviation
for threads in currentThreadList:
    print("Run parallel with {} threads...".format(threads))
    foldernameParallel = "{}Parallel{}".format(runName, threads)
    _makeFolder(foldernameParallel)
    os.chdir(foldernameParallel)

    ThreadRunTime = []
    ThreadResidualDifference = []
    for runNr in range(1, maximumRuns+1):
        currentFolderName = "{}Parallel{}_{}".format(runName, threads, runNr)

        # Copy input and output from preprocessing folder
        shutil.copytree(PreprocessLocation, os.path.join(copylocation, foldernameParallel, currentFolderName))
        _modifyOutputFileNames(runName, currentFolderName)

        # Set max threads
        _setNumThreads(threads + 1)

        # Run inversion in Parallel and write duration (and result) to file
        os.system(os.path.join(applicationsLocation, "FWI_OpenMPGradienDescentProcess {}".format(currentFolderName)))

        # Get the run time and store this value
        starttime_parallel = _getTime(" INFO: Starting", currentFolderName + "/output/" + currentFolderName)
        endtime_parallel = _getTime(" INFO: Finished", currentFolderName + "/output/" + currentFolderName)
        totalruntime_parallel = (endtime_parallel - starttime_parallel).seconds
        runtimeFileText += "Parallel_{}_{} \t \t {} \n".format(threads, runNr, totalruntime_parallel)
        ThreadRunTime.append(totalruntime_parallel)

        # Get the final residual and store the value
        parallelResiduals = _getResiduals(currentFolderName + "/output/" + currentFolderName)
        residualsDifference = abs(parallelResiduals - serialResiduals)
        finalResidualDifference = residualsDifference[-1]
        residualDifferencetext += "Parallel_{}_{} \t \t {} \n".format(threads, runNr, finalResidualDifference)
        ThreadResidualDifference.append(finalResidualDifference)

    # Compute the mean and standard deviation for runtime and residual difference
    meanRunTime = np.mean(np.array(ThreadRunTime))
    deviationRunTime = np.std(np.array(ThreadRunTime))
    meanResidualDifference = np.mean(np.array(ThreadResidualDifference))
    deviationResidualDifference = np.std(np.array(ThreadResidualDifference))

    # Create file texts
    runtimeFileText += "Parallel_{} \t \t {}  \t {} \n\n".format(threads, meanRunTime, deviationRunTime)
    residualDifferencetext += "Parallel_{}\t \t {}  \t {} \n\n".format(threads, meanResidualDifference,
                                                                       deviationResidualDifference)

    # Store the mean values and standard deviations
    allRunTimes.append(meanRunTime)
    allResidualDifferences.append(meanResidualDifference)
    runtimeErrorBar.append(deviationRunTime)
    ResidualDifferencesErrorBar.append(deviationResidualDifference)

    # Go back to one directory higher to compute for another number of threads
    # or to create all files
    os.chdir(copylocation)

# Write all runtimes to a file
print("Write runtimes.txt")
runtimefile = open("runtimes.txt", 'w+')
runtimefile.write(runtimeFileText)
runtimefile.close()

# Write all residual differences to a file
print("Write residualDifferences.txt")
residualdifferencefile = open("residualDifferences.txt", 'w+')
residualdifferencefile.write(residualDifferencetext)
residualdifferencefile.close()

# Create plots
print("Create residual difference plot")
plt.clf()
plt.plot(currentThreadList, allResidualDifferences)
plt.errorbar(currentThreadList, allResidualDifferences, ResidualDifferencesErrorBar, fmt='o',
             ecolor='r')
plt.ylabel("Final Residual Differences")
plt.xlabel("Number of threads")
plt.grid(True)
plt.xticks(np.arange(1, currentThreadList[-1]+1, 1))
plt.savefig("FinalResidualDifferences.png", dpi=400)

currentThreadList.insert(0, 0)
print("Create run time plot")
plt.clf()
plt.plot(currentThreadList, allRunTimes)
plt.ylabel("Run time (seconds)")
plt.xlabel("Number of threads")
plt.grid(True)
plt.xticks(np.arange(0, currentThreadList[-1]+1, 1))
plt.errorbar(currentThreadList, allRunTimes, runtimeErrorBar, fmt='o', ecolor='r')
plt.savefig("AllRunTimes.png", dpi=400)

print("Analysis is finised")
# Plot the durations against grid size (percentage of original size)
