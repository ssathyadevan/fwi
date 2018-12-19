#!groovy

import hudson.tasks.test.AbstractTestResultAction

import hudson.model.Actionable

def setEnvironment() {

        // Get commit parameters like commit code and author

        env.SHORT_COMMIT_CODE = sh(returnStdout: true, script: "git log -n 1 --pretty=format:'%h'").trim()

        env.COMMIT_MESSAGE = sh(returnStdout: true, script: "git log --format=%B -n 1 ${SHORT_COMMIT_CODE}").trim()

        env.COMITTER_EMAIL = sh(returnStdout: true, script: 'git --no-pager show -s --format=\'%ae\'').trim()

        env.AUTHOR_NAME = sh(returnStdout: true, script: 'git --no-pager show -s --format=\'%an\'').trim()

        // Set build name and description accordingly

        currentBuild.displayName = "BM | commit ${SHORT_COMMIT_CODE} | ${AUTHOR_NAME}"

        currentBuild.description = "${COMMIT_MESSAGE}"

}

def buildAll() {

        echo 'Building..'

        sh '''

                mkdir -p build

                cd $BM_SRC_DIR/bin

                chmod +x *.sh

                source bmInit.sh

                bmBuildAll.sh

        '''

}

def testAll(){

        echo 'Testing..'

        sh '''

                source bmInit.sh

                mkdir -p ${BM_TEST_RESULT_DIR}

                export PATH=$PATH:${BM_INSTALL_DIR}/bin

                bmRunAllTests.sh

                GtestXMLtoJunitXML.sh

        '''

        junit 'build/testing/output/*.xml'

}

def deploy(){

        sh 'tar --exclude="build" \

                --exclude="Dockerfile" \

                --exclude="jenkinsFunctions" \

                --exclude="Jenkinsfile" \

                --exclude="Jenkinsfile_nightly" \

                -zcf BM-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz *'



        archiveArtifacts artifacts:"BM-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz"

}

def sendEmail() {

        email = evaluate readTrusted('jenkinsFunctions/email.groovy')



        if(currentBuild.currentResult == "UNSTABLE" || currentBuild.currentResult == "SUCCESS") {

                testSummaryLib = evaluate readTrusted('jenkinsFunctions/testSummary.groovy')

                testResultAction = currentBuild.rawBuild.getAction(AbstractTestResultAction.class)

                testSummary =  testSummaryLib.getTestSummary(testResultAction)

                testResultAction = null

                email.sendEmail(testSummary)

        }

        if(currentBuild.currentResult == "FAILURE") {

                email.sendEmailFailure()

        }

}

return this

