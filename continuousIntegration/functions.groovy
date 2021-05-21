#!groovy

def setEnvironment() {

	// Get commit parameters like commit code and author
	env.SHORT_COMMIT_CODE = sh(returnStdout: true, script: "git log -n 1 --pretty=format:'%h'").trim()
	env.COMMIT_MESSAGE = sh(returnStdout: true, script: "git log --format=%B -n 1 ${SHORT_COMMIT_CODE}").trim()
	env.COMITTER_EMAIL = sh(returnStdout: true, script: 'git --no-pager show -s --format=\'%ae\'').trim()
	env.AUTHOR_NAME = sh(returnStdout: true, script: 'git --no-pager show -s --format=\'%an\'').trim()
	env.MYSTAGE_NAME = 'Preparing'
	env.REGRESSION_TESTS = 'undefined'

	// Set build name and description accordingly
	currentBuild.displayName = "FWI | commit ${SHORT_COMMIT_CODE} | ${AUTHOR_NAME}"
	currentBuild.description = "${COMMIT_MESSAGE}"
}

def build(String osName = "undefined") {
	echo 'Building on ' + osName
	env.MYSTAGE_NAME = 'Build'
	if (osName == "Windows"){
		bat '''
			mkdir build 
			cd build 
			cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
			ninja install
		'''
	}
	else{
		sh '''
			mkdir build
			cd build
			cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
			ninja install
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
		'''
	}
	else{
		sh '''
			cd build
			ctest -T test --no-compress-output
		'''
	}
}

def RunStaticAnalysisCppcheck(String osName = "undefined") {
	echo 'Running cppcheck on ' + osName
	env.MYSTAGE_NAME = 'Static Analysis Cppcheck'
	if (osName == "Windows"){
		bat '''
			cd build 
			ninja cppcheck
		'''
	}
	else{
		sh '''
			cd build 
			ninja cppcheck
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

def publishUnitTestsResultsOnJenkins() {
	if (currentBuild.currentResult != "FAILURE") {
		echo 'Publish unit tests results on jenkins'
		xunit (
			tools: [ CTest (pattern: 'build/Testing/**/Test.xml') ])
	}
}

def publishRegressionTestsResultsOnJenkins() {
	if (currentBuild.currentResult != "FAILURE") {
		echo 'Publish regression tests results on jenkins'
		junit ('build/*.xml')
	}
}

def publishCppCheckResultsOnJenkins(String osName = "undefined") {
	if (currentBuild.currentResult != "FAILURE") {

		// CppCheck upload has been disabled due to missing the CppCheck plugin on the new Jenkins server.
		// CppCheck results can be converted to JUnit results using cppcheck_junit, but then the build will fail if the cppcheck finds any issues, which are quite some at the moment of writing.
		// Therefore, the CppCheck report is currently uploaded as an artifact.

		archiveArtifacts artifacts:"staticAnalysisCppcheckReport.xml"	

		// The following code can be uncommented to upload the CppCheck result as JUnit.
		// if (osName == "Windows"){
		// 	bat '''
		// 	cppcheck_junit staticAnalysisCppcheckReport.xml staticAnalysisCppcheckReport-junit.xml
		// 	'''
		// }
		// else{
		// 	sh '''
		// 		cppcheck_junit staticAnalysisCppcheckReport.xml staticAnalysisCppcheckReport-junit.xml
		// 	'''
		// }

		// echo 'Publish CppCheck results as JUnit on jenkins'
		// junit ('staticAnalysisCppcheckReport-junit.xml')
	}
}

def executeTests( String osName = "undefined", String csv = "undefined" ) {
	echo 'Running executeTests on ' + osName
	env.MYSTAGE_NAME = 'Running executeTests'
	env.REGRESSION_TESTS = csv
	if (osName == "Windows"){
		bat '''
			mkdir FWITest
			copy tests\\executeTests.py FWITest
			copy tests\\%REGRESSION_TESTS% FWITest
			cd FWITest
			python3 executeTests.py --all --input %REGRESSION_TESTS%
		'''
	}
	else{
		sh '''
			mkdir FWITest
			cp tests/executeTests.py FWITest
			cp tests/${REGRESSION_TESTS} FWITest
			cd FWITest
			python3 executeTests.py --all --input ${REGRESSION_TESTS}
		'''
	}
}

def sendEmail( String osName = "undefined" ) {
	String messageBody = "Dear ${env.AUTHOR_NAME},\n\nYour commit: ${env.SHORT_COMMIT_CODE} \nSystem: ${osName} \nBranch: ${env.JOB_NAME}\nRan with status: " + currentBuild.currentResult
	if(currentBuild.currentResult != "SUCCESS") {
		messageBody = messageBody + "\nStage where failure occurred: ${env.MYSTAGE_NAME},\nPlease check the Jenkins server console output to diagnose the problem: ${env.BUILD_URL}."
	}
	mail from: "noreply-jenkins-FWI@alten.nl", \
	to: "${env.COMITTER_EMAIL}", \
	subject: "Pipeline job: " + "${env.JOB_NAME} returned status " + currentBuild.currentResult + " on " + osName, \
	body: messageBody

	echo "Email sent"
}

return this

