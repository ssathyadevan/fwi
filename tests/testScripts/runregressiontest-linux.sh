#!/bin/bash
echo "Script to run regresson tests"#print to screen
cd ../../../FWIInstall
mkdir test
echo "Specify which regression test you want to run: fast, hiquality, varyingSrcRecvData"
read varname
cp -r ../parallelized-fwi/tests/regression_data/$varname test/
cp ../parallelized-fwi/pythonScripts/* test/
cp ../parallelized-fwi/tests/testScripts/* test/
cp -r default test/
cd test
python regressionTestPreProcessing_python3.py $varname default
python regressionTestProcessing_python3.py $varname default
