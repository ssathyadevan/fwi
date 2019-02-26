#!/bin/bash
cd ../../..

# Run all regression tests
cd ./parallelized-fwi/tests/regression_data
TESTS=$(find . -maxdepth 1 -type d ! -path . -printf '%P\n')
cd ../../../FWIInstall
mkdir -p input output test
cp ../parallelized-fwi/inputFiles/* input/
cp ../parallelized-fwi/pythonScripts/postProcessing-python3.py .
cp ../parallelized-fwi/tests/testScripts/* test/ 2>/dev/null
NEW="NEW"
for TEST in $TESTS
do
	rm output/* test/* 2>/dev/null
	echo "Running test:" $TEST
	cp ../parallelized-fwi/tests/regression_data/$TEST/$TEST.in input/"$TEST$NEW.in"
	cp ../parallelized-fwi/tests/regression_data/$TEST/* test/
	cp input/"$TEST$NEW.in" test/

	./bin/FWI_PreProcess ../input/ ../output/ $TEST$NEW
	./bin/FWI_Process ../input/ ../output/ $TEST$NEW
	break

	python3 postProcessing-python3.py output/
	python3 test/regressionTestPreProcessing_python3.py "$TEST$NEW" $TEST
	python3 test/regressionTestProcessing_python3.py "$TEST$NEW" $TEST	
done	

exit
# Run unit tests
python3 -m pytest test/python_unittest.py --junitxml results.xml
cp results.xml ../build
rm -r input/ output/ test/ 

#cp inputFiles/* input/
#cd FWIInstall/bin
#./FWI_PreProcess ../../input/ ../../output/ default
#./FWI_Process ../../input/ ../../output/ default
#cd ../../pythonScripts
#cp postProcessing-python3.py ../
#cd ..
#python3 postProcessing-python3.py output/
#mkdir test
#cp tests/regression_data/fast/* test/
#cp tests/testScripts/* test/
#cp input/default.in test/
#cp output/* test/
#cd test
#python3 regressionTestPreProcessing_python3.py fast default
#python3 regressionTestProcessing_python3.py fast default
#python3 -m pytest python_unittest.py --junitxml results.xml 
#cp results.xml ../build/
