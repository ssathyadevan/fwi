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
				stage('Connecting to Ubuntu Agent') {
					agent{
						dockerfile{
							customWorkspace "workspace/${env.JOB_NAME}".replace(' ', '_')
							filename 'continuousIntegration/Dockerfile'
							additionalBuildArgs  '--build-arg BM_SRC_DIR=${WORKSPACE} --build-arg BM_BUILD_DIR=${WORKSPACE}/build'
						}
					}
					stages{
						stage('Preparing Ubuntu Workspace'){
							steps{
								deleteDir()
								checkout scm
								script{
									functions = evaluate readTrusted('continuousIntegration/functions.groovy')
									functions.setEnvironment()
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
						stage('Testing on Ubuntu'){
							steps{
								script{
									functions.unitTest("Ubuntu")
								}
							}
						}
						stage('Regression Testing on Ubuntu'){
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
				stage('Connecting to Windows Agent'){
					agent{
						node{
							label 'Windows'
							customWorkspace "workspace\\FWI\\" + env.BRANCH_NAME.take(10)
						}
					}
					stages{
						stage( 'Preparing Windows Workspace' ){
							steps{
								echo "Preparing windows workspace"
								deleteDir()
								checkout scm
								script{
									functions = evaluate readTrusted('continuousIntegration/functions.groovy')
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
						stage('Testing on Windows'){
							steps{
								script{
									functions.unitTest("Windows")
								}
							}
						}
						stage('Regression Testing on Windows'){
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
