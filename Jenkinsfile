#!/usr/bin/env groovy
def functions

pipeline{
	agent none
	stages{
		stage('Starting Pipeline'){
			options{
				timeout( time : 30, unit: "MINUTES")
			}
			parallel{
				stage('Connecting to Ubuntu Agent') {
					agent{
						dockerfile{
							customWorkspace "workspace/${env.JOB_NAME}".replace(' ', '_')
							filename 'Dockerfile'
							additionalBuildArgs  '--build-arg BM_SRC_DIR=${WORKSPACE} --build-arg BM_BUILD_DIR=${WORKSPACE}/build'
						}
					}
					stages{
						stage('Preparing Ubuntu Workspace'){
							steps{
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
						stage('Testing on Ubuntu'){
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
						stage('Deploying on Ubuntu'){
							steps{
								script{
									functions.deploy()
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
									functions = evaluate readTrusted('jenkinsFunctions/functions.groovy')
								}
							}
						}
						stage( 'Building on Windows' ){
							steps{
								script{
									echo "Building on windows"
									bat(script:'mkdir build \n cd build \n cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../FWIInstall ..\n ninja install')
								}
							}
						}
						stage('Testing on Windows'){
							steps{
								script{
									echo "Testing on windows"
									bat(script:"cd build \n ctest -T test --no-compress-output")
									bat(script: 'for /f "delims=" %%i in (\'where /r %WORKSPACE%\\build Test.xml\') do set dirOutput=%%i \n cd build \n echo %dirOutput% \n copy %dirOutput% .\\CTestResults.xml')
								}
							}
						}
						stage('Regression Testing on Windows'){
							steps{
								script{
									echo "Running regression tests on windows"
									bat(script:'copy %WORKSPACE%\\tests\\testScripts\\unified_run_all_regressions_python.py . \n python3 unified_run_all_regressions_python.py IntegralForwardModel ConjugateGradientInversion')
								}
							}
						}
						stage('Deploying on Windows'){
							steps{
								script{
									echo 'Deploying on windows'
									bat(script:'xcopy inputFiles FWIInstall\\inputFiles /s /i \n xcopy pythonScripts FWIInstall\\pythonScripts /s /i \n 7z a Windows-FWI-%GIT_BRANCH%-%SHORT_COMMIT_CODE%.zip ./FWIInstall/*')
									archiveArtifacts artifacts:"Windows-FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz"
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
