#!/usr/bin/env groovy
def functions

pipeline {
	agent none
    stages {
		stage('Starting Pipeline'){
			options {
				timeout( time : 30, unit: "MINUTES")
			}
			parallel {
					stage('Connecting to Ubuntu Agent') {
						agent {
							dockerfile {
								customWorkspace "workspace/${env.JOB_NAME}".replace(' ', '_')
								filename 'Dockerfile'
								additionalBuildArgs  '--build-arg BM_SRC_DIR=${WORKSPACE} --build-arg BM_BUILD_DIR=${WORKSPACE}/build'
							}
						}
						stages{
								stage('Preparing'){
										steps {
												deleteDir()
												checkout scm
												script{
														functions = evaluate readTrusted('jenkinsFunctions/functions.groovy')
														functions.setEnvironment()
												}
										}
								}
								stage('Build'){
										steps{
												script{
														functions.buildAll()
												}
										}
								}
						/*stage('parallelAnalysis'){
							steps{
								script{
									functions.parallelAnalysis()
								}
							}		
						}*/

								stage('Test') {
										steps{
												script{
														functions.testAll()
												}
										}
								}

								stage('RegressionTesting'){
										steps{
												script{
														functions.regressiontest()
												}
										}
								}
							   
								stage('Deploy') {
								/* when {
										beforeAgent true
										branch 'master'
										} */
										// use the above code chunk to deploy only for a certain branch if needed
									   steps{
												script {
														functions.deploy()

												}
										}
								}
								 
						}
					}

					stage('Connecting to Windows Agent') {
						agent {
							label 'Windows'
						}

						stages {
							stage( 'Building in Windows' ){
								 steps {
								 	 deleteDir()
									 checkout scm
									 script {
										echo "Building on windows"
										bat(script:'mkdir C:\BuildFolder\build \n xcopy /s ${workspace} C:\BuildFolder \n cd C:\BuildFolder\build \n cmake -G "MinGW Makefiles" .. \n mingw32-make.exe')
									 }
								}
							}

							stage('Testing in Windows') {
								 steps {
									 script {
										echo "Testing on windows"
										bat(script:"cd build \n cd tests \n ctest ")
									 }
								}
							}

							stage('Deploying in Windows') {
								steps {
									echo 'Deploying on windows....'
								}
							}
						}
					}
				 }
			}
	}	
	
    post {
        always {
			/*script {
			   functions.unitTestSummary()
			}*/
			echo 'Sending email'
			script {
				functions.sendEmail()
			}						
        }
    }
}