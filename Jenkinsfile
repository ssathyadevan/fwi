#!/usr/bin/env groovy
def functions

pipeline{
	agent none
	stages{
		stage('Starting Pipeline'){
			options{
				timeout( time : 60, unit: "MINUTES")
			}
			parallel{
				stage('Connecting to Ubuntu agent') {
					agent{
						dockerfile{
							customWorkspace "workspace/${env.JOB_NAME}".replace(' ', '_')
							filename 'continuousIntegration/Dockerfile'
							additionalBuildArgs  '--build-arg BM_SRC_DIR=${WORKSPACE} --build-arg BM_BUILD_DIR=${WORKSPACE}/build'
						}
					}
					stages{
						stage('Preparing Ubuntu workspace'){
							steps{
								deleteDir()
								checkout scm
								script{
									functions = evaluate readTrusted('continuousIntegration/functions.groovy')
									functions.setEnvironment("Ubuntu")
								}
							}
						}
						stage('Building on Ubuntu'){
							steps{
								script{
									functions.build("Ubuntu")
								}
							}
						}
						stage('Unit testing on Ubuntu'){
							steps{
								script{
									functions.unitTest("Ubuntu")
								}
							}
						}
						stage('Regression testing on Ubuntu'){
							steps{
								script{
									functions.regressionTest("Ubuntu")
								}
							}
						}
						stage('Deploying on Ubuntu'){
							steps{
								script{
									functions.deploy("Ubuntu")
								}
							}
						}
					}
					post{
						always{
							echo 'Recording test summary'
							script{
								functions.unitTestSummary()
							}
							echo 'Sending email'
							script{
								functions.sendEmail("Ubuntu")
							}
						}
					}
				}
				stage('Connecting to Windows agent'){
					agent{
						node{
							label 'Windows'
							customWorkspace "workspace\\FWI\\" + env.BRANCH_NAME.take(10)
						}
					}
					stages{
						stage( 'Preparing Windows workspace' ){
							steps{
								echo "Preparing windows workspace"
								deleteDir()
								checkout scm
								script{
									functions = evaluate readTrusted('continuousIntegration/functions.groovy')
									functions.setEnvironment("Windows")
								}
							}
						}
						stage( 'Building on Windows' ){
							steps{
								script{
									functions.build("Windows")
								}
							}
						}
						stage('Unit testing on Windows'){
							steps{
								script{
									functions.unitTest("Windows")
								}
							}
						}
						stage('Regression testing on Windows'){
							steps{
								script{
									functions.regressionTest("Windows")
								}
							}
						}
						stage('Deploying on Windows'){
							steps{
								script{
									functions.deploy("Windows")
								}
							}
						}
					}
					post{
						always{
							echo 'Recording test summary'
							script{
								functions.unitTestSummary()
							}
							echo 'Sending email'
							script{
								functions.sendEmail("Windows")
							}
						}
					}
				}
			}
		}
	}
}
