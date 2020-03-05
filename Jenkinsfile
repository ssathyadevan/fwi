def functions

pipeline{

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
        post {
                always {
                        if (currentBuild.currentResult == "SUCCESS") {
				echo 'Creating unit-test Result Summary (junit)'
							xunit (
											tools: [ CTest (pattern: 'build/*.xml') ])
							junit ('build/*.xml')
                        
						echo 'Cleaning the workspace'
                        //deleteDir()
			}
			echo 'Sending email'
                        script {
                                functions.sendEmail()
                        }			


						
                }
        }


