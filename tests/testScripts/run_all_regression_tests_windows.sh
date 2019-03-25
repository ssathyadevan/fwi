#!/bin/bash

#
# 	This script automatically runs all regression tests and writes the results to
# 	a file in directory $HOMEPATH/Documents/FWI/build/Regression_results.txt.
#	This script should be in the directory $HOMEPATH/Documents/FWI/FWIInstall, together with
#	the scripts read_pytest.py and read_results.py.
#

#FWI_GENERAL_PATH=$HOMEPATH/Documents/FWI
FWI_INSTALL_PATH=$HOMEPATH/Documents/FWI/FWIInstall
FWI_SOURCE_PATH=$HOMEPATH/Documents/FWI/parallelized-fwi
#echo $FWI_SOURCE_PATH
#FWI_INSTALL_PATH=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}/FWIInstall
#FWI_SOURCE_PATH=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}


cd $FWI_SOURCE_PATH/tests/regression_data
TESTS=$(find . -maxdepth 1 -type d ! -path . -printf '%P\n')
#echo $TESTS
cd $FWI_INSTALL_PATH
mkdir -p test
cp -r $FWI_SOURCE_PATH/tests/testScripts/. test #*.py
cd test
RUN="RUN"
for TEST in $TESTS
do
	echo "###############################################################"
	echo "              Regression test:" $TEST
	echo "###############################################################"

	mkdir -p $TEST$RUN
	cp -r $FWI_SOURCE_PATH/tests/regression_data/$TEST/. $TEST$RUN/
	cp -r $FWI_SOURCE_PATH/tests/regression_data/$TEST/ .
	cp -r $FWI_INSTALL_PATH/read_pytest.py .
	cp -r $FWI_INSTALL_PATH/read_results.py .
	cp -r $FWI_INSTALL_PATH/read_pytest.py $TEST$RUN/
	cp -r $FWI_INSTALL_PATH/read_results.py $TEST$RUN/

	$FWI_INSTALL_PATH/bin/FWI_PreProcess $TEST$RUN
	$FWI_INSTALL_PATH/bin/FWI_Process $TEST$RUN

	destdir=Regression_results.txt
	py regressionTestPreProcessing_python3.py $TEST $TEST$RUN
	
	
	echo "Passed $TEST regression test: " >> "$destdir"
	py regressionTestProcessing_python3.py $TEST $TEST$RUN
	py read_results.py
	
	destdir2=testname.txt
	echo "$TEST" >> "$destdir2"
	echo "Passed $TEST unit test: " >> "$destdir"
	py -m pytest python_unittest.py --junitxml "${TEST}results.xml"
	py read_pytest.py
	rm -r testname.txt
	
	cp Regression_results.txt $FWI_INSTALL_PATH/
	
done	

file="Regression_results.txt"
while IFS= read -r line
do
        # display $line or do somthing with $line
	printf '%s\n' "$line"
done <"$file"
cd ..
rm -r test
cmd /k