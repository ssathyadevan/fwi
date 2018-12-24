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

                stage('Deploy') {
                       steps{
                                script {
                                        functions.deploy()

                                }
                        }
                }
        }
}
