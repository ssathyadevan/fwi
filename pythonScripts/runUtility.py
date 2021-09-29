# This script runs preprocessing, processing and postprocessing using the default input provided in the project
# It is meant as a quick-and-dirty run-all utility
# It can be given arguments to run on different input and with different run options


import os
import platform
import argparse
from pathlib import Path

# Configure the argument parser
parser = argparse.ArgumentParser(description="Execute preprocess, process and postprocess in one call.",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)

# Input arguments
parser.add_argument("-b", "--bin", type=str, required=False, default="./bin/",
                    help="Path to bin folder containing the app executables")

parser.add_argument("-d", "--dir", type=str, required=False, default="./default/",
                    help="Path to the folder containing input/output folders")

parser.add_argument("--post-dir", type=str, required=False, default="./",
                    help="Path to the folder containing postProcessing-python3.py")

parser.add_argument("-p", "--preprocess", type=str, required=False, default="FiniteDifference",
                    choices=["FiniteDifference", "Integral","FiniteDifferenceOpenMP", "FiniteDifferenceMPI"],
                    help="Choice of ForwardModel for PreProcess")

parser.add_argument("-f", "--forward", type=str, required=False, default="FiniteDifference",
                    choices=["FiniteDifference", "Integral","FiniteDifferenceOpenMP", "FiniteDifferenceMPI"],
                    help="Choice of ForwardModel for Process")

parser.add_argument("-i", "--inversion", type=str, required=False, default="ConjugateGradient",
                    choices=["ConjugateGradient", "GradientDescent", "Evolution", "Random", "StepAndDirection","ConjugateGradientMPI","ConjugateGradientOpenMP"],
                    help="Choice of inversion method for Process *StepAndDirection is deprecated*")

parser.add_argument("--step-dir", type=str, required=False, default="ConjugateGradient",
                    choices=["ConjugateGradient", "GradientDescent"],
                    help="Used only if -i=StepAndDirection. Desired StepDirection")

parser.add_argument("--step-size", type=str, required=False, default="ConjugateGradient",
                    choices=["ConjugateGradient", "ConjugateGradientRegularization", "BorzilaiBorwein", "Fixed"],
                    help="Used only if -i=StepAndDirection. Desired StepSize")

parser.add_argument("--threads",type=int,default=1,required=False)
parser.add_argument("--MPI",action='store_true',help="Activate mpirun ...")

parser.add_argument('--skip-pre', action='store_true', help="Skip prepocessing")
parser.add_argument('--skip-process', action='store_true', help="Skip pocessing")
parser.add_argument('--skip-post', action='store_true', help="Skip postpocessing")
parser.add_argument('--suppress-warning', action='store_true',
                    help="Suppress the warning for not running from FWIInstall/")

# Parse the input arguments
arguments = parser.parse_args()

#set number of threads omp
os.environ["OMP_NUM_THREADS"] = str(arguments.threads)

if os.path.basename(os.getcwd()) != "FWIInstall" and not arguments.suppress_warning:
    msg = "This script is used most easily from the FWIInstall folder.\n"
    msg += "Please copy the files 'runUtility.py' and 'postProcessing-python3.py' "
    msg += "from parallelized-fwi/pythonScripts into parallelized-fwi/FWIInstall\n"
    msg += "Then run 'runUtility.py' from there\n"
    msg += "If the FWIInstall folder does not exist, then the project is not (correctly) built"
    print(msg, flush=True)
    print()


if not arguments.skip_pre:
    preProcessExe = Path(os.path.join(os.path.abspath(arguments.bin), "FWI_PreProcess")).as_posix()
    preProcessArgs = " -d \"" + Path(os.path.abspath(arguments.dir)).as_posix() + "\" -f " + arguments.preprocess
    print("------->Calling PreProcessing:", flush=True)
    print(preProcessExe + preProcessArgs, flush=True)
    os.system(preProcessExe + preProcessArgs)
    print()
else:
    print("------->PreProcessing skipped")

if not arguments.skip_process:
    if not arguments.MPI:
        processExe = Path(os.path.join(os.path.abspath(arguments.bin), "FWI_Process")).as_posix()
    else:
        processExe = "mpirun --use-hwthread-cpus -np " + str(arguments.threads) + " " + Path(os.path.join(os.path.abspath(arguments.bin), "FWI_ProcessMPI")).as_posix()
    processArgs = " -d \"" + Path(os.path.abspath(arguments.dir)).as_posix() + "\"" \
                  + " -f " + arguments.forward \
                  + " -i " + arguments.inversion

    print("------->Calling Processing:", flush=True)
    print(processExe + processArgs, flush=True)
    os.system(processExe + processArgs)
    print()
else:
    print("------->Processing skipped")

if not arguments.skip_post:
    execName = 'python3'
    if platform.system() == 'Windows':
        execName = 'python'
    postProcessPy = execName + " "\
                    + Path(os.path.join(os.path.abspath(arguments.post_dir), "postProcessing-python3.py")).as_posix()
    postProcessArgs = " -o \"" + Path(os.path.abspath(arguments.dir)).as_posix() + "\""
    print("------->Calling PostProcessing:", flush=True)
    print(postProcessPy + postProcessArgs, flush=True)
    os.system(postProcessPy + postProcessArgs)
    print()
else:
    print("------->PostProcessing skipped")
