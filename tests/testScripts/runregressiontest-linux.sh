#!/bin/bash
echo "Script to run regresson tests" #print to screen
cd ../../../../FWIInstall
mkdir test
echo "Specify which regression test you want to run: fast, hiquality, varyingSrcRecvData"
read varname
cp ../FWI/parallelized-fwi/tests/regression_data/$varname/* test/
cp ../FWI/parallelized-fwi/pythonScripts/* test/
cp ../FWI/parallelized-fwi/tests/testScripts/* test/
cp ../FWI/parallelized-fwi/inputFiles/* test/
cd test
python regressionTestPreProcessing.py $varname default
python regressionTestProcessing.py $varname default