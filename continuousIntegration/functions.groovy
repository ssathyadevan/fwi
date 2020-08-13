#!groovy

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

def build(String osName = "undefined") {
		echo 'Building on ' + osName
		env.MYSTAGE_NAME = 'Build'
		String buildType = 'Release'
		String installPrefix = '../FWIInstall ..'
		if (osName == "Windows"){
			bat '''
				mkdir build 
				cd build 
				cmake -G "Ninja" -DCMAKE_BUILD_TYPE=%buildType% -DCMAKE_INSTALL_PREFIX=%installPrefix%
				ninja install
			'''
		}
		else{
			sh '''
				mkdir build
				cd build
				cmake -DCMAKE_BUILD_TYPE=${buildType} -DCMAKE_INSTALL_PREFIX=${installPrefix}
				make install
			'''
		}
}

def unitTest(String osName = "undefined") {
		echo 'Running unit tests on ' + osName
		env.MYSTAGE_NAME = 'Unit Testing'
		if (osName == "Windows"){
			bat '''
				cd build 
				ctest -T test --no-compress-output
				for /f "delims=" %%i in (\'where /r %WORKSPACE%\\build Test.xml\') do set dirOutput=%%i 
				copy %dirOutput% .\\CTestResults.xml
			'''
		}
		else{
			sh '''
				cd build
				ctest -T test --no-compress-output
				cp Testing/`head -n 1 Testing/TAG`/Test.xml ./CTestResults.xml
			'''
		}
}

def regressionTest(String osName = "undefined") {
		echo 'Running regression tests on ' + osName
		env.MYSTAGE_NAME = 'Regression Testing'
		if (osName == "Windows"){
			bat '''
				copy tests\\testScripts\\unified_run_all_regressions_python.py . 
				python3 unified_run_all_regressions_python.py IntegralForwardModel ConjugateGradientInversion
			'''		
		}
		else{
			sh '''
				cp tests/testScripts/unified_run_all_regressions_python.py .
				python3 unified_run_all_regressions_python.py IntegralForwardModel ConjugateGradientInversion
			'''
		}
}

def deploy(String osName = "undefined"){
		echo 'Deploying on ' + osName
		env.MYSTAGE_NAME = 'Deploy'
		if (osName == "Windows"){
			bat '''
				xcopy inputFiles FWIInstall\\inputFiles /s /i 
				xcopy pythonScripts FWIInstall\\pythonScripts /s /i 
				7z a Windows-FWI-%GIT_BRANCH%-%SHORT_COMMIT_CODE%.zip ./FWIInstall/*
			'''
			archiveArtifacts artifacts:"Windows-FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.zip"	
		}
		else{
			sh '''
				cp -r inputFiles FWIInstall/
				mkdir FWIInstall/pythonScripts
				cp -r pythonScripts/* FWIInstall/pythonScripts
				tar -zcf Ubuntu-FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz FWIInstall
			'''
			archiveArtifacts artifacts:"Ubuntu-FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz"
		}
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

