#!groovy

import hudson.tasks.test.AbstractTestResultAction

import hudson.model.Actionable

import com.cloudbees.groovy.cps.NonCPS


def setEnvironment() {

        // Get commit parameters like commit code and author
        env.SHORT_COMMIT_CODE = sh(returnStdout: true, script: "git log -n 1 --pretty=format:'%h'").trim()
        env.COMMIT_MESSAGE = sh(returnStdout: true, script: "git log --format=%B -n 1 ${SHORT_COMMIT_CODE}").trim()
        env.COMITTER_EMAIL = sh(returnStdout: true, script: 'git --no-pager show -s --format=\'%ae\'').trim()
        env.AUTHOR_NAME = sh(returnStdout: true, script: 'git --no-pager show -s --format=\'%an\'').trim()
        // Set build name and description accordingly
        currentBuild.displayName = "FWI | commit ${SHORT_COMMIT_CODE} | ${AUTHOR_NAME}"
        currentBuild.description = "${COMMIT_MESSAGE}"
}

def buildAll() {
        echo 'Building..'
        sh '''
        mkdir build
        cd build
        cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}/FWIInstall ..
        make install
        '''
}

def testAll() {
    echo 'testing all'
    sh '''
    cd build
    make test
    ctest -T test --no-compress-output
    cp Testing/`head -n 1 Testing/TAG`/Test.xml ./CTestResults.xml
    '''
}

def regressiontest() {
        echo 'Running regression tests'
	#sh tests/testScripts/run_all_regression_tests.sh	
	
	FWI_INSTALL_PATH=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}/FWIInstall
	FWI_SOURCE_PATH=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}

	echo 'a'
	# Run all regression tests
	cd $FWI_SOURCE_PATH/tests/regression_data
	echo 'b'
	TESTS=$(find . -maxdepth 1 -type d ! -path . -printf '%P\n')
	echo $TESTS
	cd $FWI_INSTALL_PATH/
	mkdir -p input output test
	cp $FWI_SOURCE_PATH/inputFiles/* input/
	cp $FWI_SOURCE_PATH/pythonScripts/postProcessing-python3.py .
	echo 'c'
	NEW="NEW"
	for TEST in $TESTS
	do
		echo "Running test:" $TEST
	
		cp $FWI_SOURCE_PATH/tests/testScripts/* test/ 2>/dev/null
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
	
		rm output/* test/* 2>/dev/null
	done	

	exit
}

def deploy(){
                echo 'Deploying'
                sh '''
                cp -r inputFiles FWIInstall/
                cp -r tests FWIInstall/
                cp pythonScripts/* FWIInstall/
                tar -zcf FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz FWIInstall
                '''
                archiveArtifacts artifacts:"FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz"

}

def sendEmail() {
        email = evaluate readTrusted('jenkinsFunctions/email.groovy')
        if(currentBuild.currentResult == "UNSTABLE" || currentBuild.currentResult == "SUCCESS") {
                email.sendEmail()
        }
        if(currentBuild.currentResult == "FAILURE") {
                email.sendEmailFailure()
        }
}
return this

