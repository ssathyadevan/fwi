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
        env.MYSTAGE_NAME = 'Preparing'
		// Set build name and description accordingly
        currentBuild.displayName = "FWI | commit ${SHORT_COMMIT_CODE} | ${AUTHOR_NAME}"
        currentBuild.description = "${COMMIT_MESSAGE}"
}

def buildAll() {
        echo 'Building..'
		env.MYSTAGE_NAME = 'Build'
        sh '''
        mkdir build
        cd build
        cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${WORKSPACE}/FWIInstall ..
        make install
        '''
        //cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}/FWIInstall ..
}

def parallelAnalysis() {
        echo 'Running parallel analysis'
		env.MYSTAGE_NAME = 'Parallel analysis'
		sh '''
		cp tests/testScripts/run_analyze_parallel.py .
		python3 run_analyze_parallel.py tests/parallelAnalyseData/default
		'''
}

def testAll() {
	    echo 'testing all'
    	env.MYSTAGE_NAME = 'Test'
    	sh '''
		cd build
    	make test
    	ctest -T test --no-compress-output
    	cp Testing/`head -n 1 Testing/TAG`/Test.xml ./CTestResults.xml
    	'''
}

def regressiontest() {
        echo 'Running regression tests'
		env.MYSTAGE_NAME = 'Regression Testing'
		sh '''
		cp tests/testScripts/unified_run_all_regressions_python.py .
		python3 unified_run_all_regressions_python.py 0	IntegralForwardModel ConjugateGradientInversion
		'''
}

def deploy(){
        echo 'Deploying'
		env.MYSTAGE_NAME = 'Deploy'
        sh '''
        cp -r inputFiles FWIInstall/
        cp -r tests FWIInstall/
        cp -r pythonScripts/* FWIInstall/
        tar -zcf FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz FWIInstall
        '''
        archiveArtifacts artifacts:"FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz"

}

def unitTestSummary() {
	if (currentBuild.currentResult != "FAILURE") {
		echo 'Creating unit-test Result Summary (junit)'
		xunit (
			tools: [ CTest (pattern: 'build/*.xml') ])
		junit ('build/*.xml')
		            
		echo 'Cleaning the workspace'
	}
	else {
		echo 'Previous steps failed, so no summary of unit-test is made'	
	}
}



def sendEmail() {
        email = evaluate readTrusted('jenkinsFunctions/email.groovy')
        if(currentBuild.currentResult == "UNSTABLE" || currentBuild.currentResult == "SUCCESS") {
                email.sendEmail()
        }
        else{          
                email.sendEmailFailure()
        }
}


return this

