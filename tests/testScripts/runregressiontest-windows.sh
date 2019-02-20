#!/bin/bash
echo "Script to run regresson tests" #print to screen
cd ../../../Build/msys64/FWIInstall
mkdir test
echo "Specify which regression test you want to run: fast, hiquality, varyingSrcRecvData"
read varname
cp ../../../parallelized-fwi/tests/regression_data/$varname/* test/
cp ../../../parallelized-fwi/pythonScripts/* test/
cp ../../../parallelized-fwi/tests/testScripts/* test/
cp ../../../parallelized-fwi/inputFiles/* test/
cp output/* test/
cd test
python3 regressionTestPreProcessing_python3.py $varname default
python3 regressionTestProcessing_python3.py $varname default