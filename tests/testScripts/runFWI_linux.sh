#!/bin/bash
echo "Script to run the FWI"
cd ../../../../FWIInstall
mkdir input
mkdir output
cp ../FWI/parallelized-fwi/inputFiles/* input/
cd bin
./FWI_PreProcess ../input/ ../output/ default
./FWI_Process ../input/ ../output/ default
cd ../..
cp FWI/parallelized-fwi/pythonScripts/postProcessing-python3.py  FWIInstall/
cd FWIInstall
python postProcessing-python3.py output