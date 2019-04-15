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
        env.STAGE_NAME = 'Preparing'
		// Set build name and description accordingly
        currentBuild.displayName = "FWI | commit ${SHORT_COMMIT_CODE} | ${AUTHOR_NAME}"
        currentBuild.description = "${COMMIT_MESSAGE}"
}

def buildAll() {
        echo 'Building..'
        sh '''
		env.STAGE_NAME = 'Build'
        #mkdir build
        cd build
        cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${WORKSPACE}/FWIInstall ..
        make install
        '''
}

def testAll() {
	echo 'testing all'
    	sh '''
    	env.STAGE_NAME = 'Test'
		cd build
    	make test
    	ctest -T test --no-compress-output
    	cp Testing/`head -n 1 Testing/TAG`/Test.xml ./CTestResults.xml
    	'''
}

def regressiontest() {
        echo 'Running regression tests'
	sh '''
	env.STAGE_NAME = 'Regression Testing'
	cp tests/testScripts/run_all_regressions_python.py .
	python3 run_all_regressions_python.py 0	#deleted the -m
	'''
}

def deploy(){
        echo 'Deploying'
        sh '''
		env.STAGE_NAME = 'Deploy'
        cp -r inputFiles FWIInstall/
        cp -r tests FWIInstall/
        cp -r pythonScripts/* FWIInstall/
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

