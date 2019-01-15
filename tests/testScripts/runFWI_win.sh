#!/bin/bash
echo "Script to run the FWI"
cd ../../../Build/msys64/FWIInstall
mkdir input
mkdir output
cp ../../../parallelized-fwi/inputFiles/* input/
cd bin
./FWI_PreProcess ../input/ ../output/ default
./FWI_Process ../input/ ../output/ default
cd ../../../..
pwd
cp parallelized-fwi/pythonScripts/postProcessing-python3.py  Build/msys64/FWIInstall/
cd Build/msys64/FWIInstall
python3 postProcessing-python3.py output