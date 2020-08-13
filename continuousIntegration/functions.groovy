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
		python3 unified_run_all_regressions_python.py IntegralForwardModel ConjugateGradientInversion
		'''
}

def deploy(){
        echo 'Deploying'
		env.MYSTAGE_NAME = 'Deploy'
        sh '''
        cp -r inputFiles FWIInstall/
		mkdir FWIInstall/pythonScripts
        cp -r pythonScripts/* FWIInstall/pythonScripts
        tar -zcf Ubuntu-FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz FWIInstall
        '''
        archiveArtifacts artifacts:"Ubuntu-FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz"

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



def sendEmail( String osName = "undefined" ) {
		String messageBody = "Dear ${AUTHOR_NAME},\n\nYour commit: ${SHORT_COMMIT_CODE} \nSystem: ${osName} \nBranch: ${env.JOB_NAME}\nRan with status: " + currentBuild.currentResult	
        if(currentBuild.currentResult != "SUCCESS") {
				messageBody = messageBody + "\nStage where failure occurred: ${env.MYSTAGE_NAME},\nPlease check the Jenkins server console output to diagnose the problem: ${BUILD_URL}."		
        }
		
        mail from: "noreply-jenkins-FWI@alten.nl", \

        to: "${COMITTER_EMAIL}", \

        subject: osName + ": " + "${env.JOB_NAME} returned status " + currentBuild.currentResult, \

        body: "Dear ${AUTHOR_NAME},\n\nYour commit: ${SHORT_COMMIT_CODE} \nSystem: ${osName} \nBranch: ${env.JOB_NAME}\nRan with status: " + currentBuild.currentResult

        echo "Email sent"			
}

return this

