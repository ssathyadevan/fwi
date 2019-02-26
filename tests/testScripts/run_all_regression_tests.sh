#!/bin/bash

FWI_INSTALL_PATH=~/FWIInstall
FWI_SOURCE_PATH=~/parallelized-fwi
#FWI_INSTALL_PATH=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}/FWIInstall
#FWI_SOURCE_PATH=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}

# Run all regression tests
cd $FWI_SOURCE_PATH/tests/regression_data
TESTS=$(find . -maxdepth 1 -type d ! -path . -printf '%P\n')
cd $FWI_INSTALL_PATH/
mkdir -p input output test
cp $FWI_SOURCE_PATH/inputFiles/* input/
cp $FWI_SOURCE_PATH/pythonScripts/postProcessing-python3.py .
NEW="NEW"
for TEST in $TESTS
do
	echo "Running test:" $TEST

	cp -r $FWI_SOURCE_PATH/tests/testScripts/* test/
	cp $FWI_SOURCE_PATH/tests/regression_data/$TEST/$TEST.in input/"$TEST$NEW.in"
	cp $FWI_SOURCE_PATH/tests/regression_data/$TEST/* test/
	cp input/"$TEST$NEW.in" test/

	$FWI_INSTALL_PATH/bin/FWI_PreProcess input/ output/ $TEST$NEW
	$FWI_INSTALL_PATH/bin/FWI_Process input/ output/ $TEST$NEW

	cp output/* test/

	python3 postProcessing-python3.py output/

	cd test/
	python3 regressionTestPreProcessing_python3.py $TEST $TEST$NEW
	python3 regressionTestProcessing_python3.py $TEST $TEST$NEW
	python3 -m pytest python_unittest.py --junitxml results.xml
	cp results.xml $FWI_SOURCE_PATH/build/
	cd ..	

	rm -r output/* test/*

	exit
done	

exit
# Clean up
rm -r input/ output/ test/
