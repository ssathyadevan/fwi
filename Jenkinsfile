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
								stage('Preparing Ubuntu Workspce'){
										steps {
												deleteDir()
												checkout scm
												script{
														functions = evaluate readTrusted('jenkinsFunctions/functions.groovy')
														functions.setEnvironment()
												}
										}
								}
								stage('Building on Ubuntu'){
										steps{
												script{
														functions.buildAll()
												}
										}
								}
								stage('Testing on Ubuntu') {
										steps{
												script{
														functions.testAll()
												}
										}
								}
								stage('Regression Testing on Ubuntu'){
										steps{
												script{
														functions.regressiontest()
												}
										}
								}   
								stage('Deploying on Ubuntu') {
									   steps{
												script {
														functions.deploy()

												}
										}
								}	 
						}
						post {
							always {
								script {
								   functions.unitTestSummary()
								}
								echo 'Sending email'
								script {
									functions.sendEmail("Ubuntu")
								}						
							}
						}
					}
					stage('Connecting to Windows Agent') {
						agent {
							label 'Windows'
						}
						stages {
							stage( 'Preparing Windows Workspce' ){
								 steps {
									ws("C:\\BuildFolder\\workspace") {
										echo "Preparing windows workspace"
										deleteDir()
										checkout scm
									}
								}
							}
							stage( 'Building on Windows' ){
								 steps {
									ws("C:\\BuildFolder\\workspace") {
										script {
											echo "Building on windows"
											bat(script:'mkdir build \n cd build \n cmake -G "Ninja" .. -DCMAKE_BUILD_TYPE=Release \n ninja')
										}
									}

								}
							}
							stage('Testing on Windows') {
								 steps {
								 	ws("C:\\BuildFolder\\workspace") {
										script {
											echo "Testing on windows"
											bat(script:"cd build \n cd tests \n ctest ")
										}
									 }
								}
							}
						}
						post {
							always {
								echo 'Sending email'
								script {
									functions.sendEmail("Windows")
								}						
							}
						}
					}
				 }
			}
	}	
}