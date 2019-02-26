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
	echo "Running test:" $TEST
	cp ../parallelized-fwi/tests/regression_data/$TEST/$TEST.in input/"$TEST$NEW.in"
	cp ../parallelized-fwi/tests/regression_data/$TEST/* test/
	cp input/"$TEST$NEW.in" test/

	./bin/FWI_PreProcess ./input/ ./output/ $TEST$NEW
	./bin/FWI_Process ./input/ ./output/ $TEST$NEW

	cp output/* test/

	python3 postProcessing-python3.py output/
	cd test/
	python3 regressionTestPreProcessing_python3.py $TEST $TEST$NEW
	python3 regressionTestProcessing_python3.py $TEST $TEST$NEW
	cd ..

	rm output/* test/* 2>/dev/null
	exit
done	

# Clean up
rm -r input/ output/ test/
