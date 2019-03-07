#!/bin/bash

#
# 	This script automatically runs all regression tests and writes the results to
# 	an XML file which can be used in Jenkins using Junit
#


#FWI_INSTALL_PATH=~/FWIInstall
#FWI_SOURCE_PATH=~/parallelized-fwi
FWI_INSTALL_PATH=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}/FWIInstall
FWI_SOURCE_PATH=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}

cd $FWI_SOURCE_PATH/tests/regression_data
TESTS=$(find . -maxdepth 1 -type d ! -path . -printf '%P\n')
cd $FWI_INSTALL_PATH/
cp $FWI_SOURCE_PATH/tests/testScripts/*.py .
RUN="RUN"
for TEST in $TESTS
do
	echo "###############################################################"
	echo "              Regression test:" $TEST
	echo "###############################################################"

	mkdir -p $TEST$RUN
	cp -r $FWI_SOURCE_PATH/tests/regression_data/$TEST/* $TEST$RUN/
	cp -r $FWI_SOURCE_PATH/tests/regression_data/$TEST/ .

	$FWI_INSTALL_PATH/bin/FWI_PreProcess $TEST$RUN
	$FWI_INSTALL_PATH/bin/FWI_Process $TEST$RUN

	python3 regressionTestPreProcessing_python3.py $TEST $TEST$RUN
	python3 regressionTestProcessing_python3.py $TEST $TEST$RUN
	python3 -m pytest python_unittest.py --junitxml "${TEST}results.xml"
	
	cp "${TEST}results.xml" $FWI_SOURCE_PATH/build/

	rm -rf $TEST$RUN/ $TEST/ "${TEST}results.xml" diff_*.csv $TEST*.png RegressionTest*.txt
done	

rm -r __pycache__/ python_unittest.py regressionTestPr*.py
