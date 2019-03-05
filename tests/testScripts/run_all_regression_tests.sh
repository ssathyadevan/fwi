#!/bin/bash

# 	This script automatically runs all regression tests and writes the results to
# 	an XML file which can be used in Jenkins using Junit
#
#   Note: This script finds regression tests based on folder name
#	      

FWI_INSTALL_PATH=~/FWIInstall
FWI_SOURCE_PATH=~/parallelized-fwi
#FWI_INSTALL_PATH=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}/FWIInstall
#FWI_SOURCE_PATH=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}

cd $FWI_SOURCE_PATH/tests/regression_data
TESTS=$(find . -maxdepth 1 -type d ! -path . -printf '%P\n')
cd $FWI_INSTALL_PATH/
mkdir -p input output test
cp $FWI_SOURCE_PATH/inputFiles/*.txt input/			# Copy case files
cp $FWI_SOURCE_PATH/pythonScripts/postProcessing-python3.py .	# Copy postproc script
RUN="RUN"
for TEST in $TESTS
do
	echo "Running test:" $TEST

	cp $FWI_SOURCE_PATH/tests/testScripts/*.py test/
	mkdir -p input/$TEST$RUN test/$TEST test/$TEST$RUN
	cp $FWI_SOURCE_PATH/tests/regression_data/$TEST/*.json input/$TEST$RUN
	cp input/$TEST$RUN/*.json test/$TEST$RUN/
	cp $FWI_SOURCE_PATH/tests/regression_data/$TEST/* test/$TEST

	$FWI_INSTALL_PATH/bin/FWI_PreProcess input/$TEST$RUN/ output/
	$FWI_INSTALL_PATH/bin/FWI_Process input/$TEST$RUN/ output/

	cp output/* test/$TEST$RUN

	cd test/
	python3 regressionTestPreProcessing_python3.py $TEST $TEST$RUN
	python3 regressionTestProcessing_python3.py $TEST $TEST$RUN
	python3 -m pytest python_unittest.py --junitxml "${TEST}results.xml"
	cp "${TEST}results.xml" $FWI_SOURCE_PATH/build/
	cd ..	

	rm -r output/* test/* input/$TEST$RUN/
done	

# Clean up
rm -r input/ output/ test/
