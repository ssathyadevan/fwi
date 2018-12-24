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
        currentBuild.displayName = "FWI | commit ${SHORT_COMMIT_CODE} | ${AUTHOR_NAME}"
        currentBuild.description = "${COMMIT_MESSAGE}"
}

def buildAll() {
        echo 'Building..'
        sh '''
        mkdir build
        cd build
        cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/var/jenkins_home/workspace/FWI/${GIT_BRANCH}/FWIInstall ..
        make install
        '''
}

def testAll() {
    echo 'testing all'
    sh '''
    cd FWIInstall/bin
    ./unittest --gtest_output="xml:FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.xml"
    cd ../../
    chmod +x *.sh
    ./GtestXMLtoJunitXML.sh
    '''

}

def deploy(){
                echo 'Deploying'
                sh '''
                cp -r inputFiles FWIInstall/
                cp -r tests FWIInstall/
                cp pythonScripts/* FWIInstall/
                tar -zcf FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz FWIInstall
                '''
                archiveArtifacts artifacts:"FWI-${GIT_BRANCH}-${SHORT_COMMIT_CODE}.tar.gz"

}

def sendEmail() {
        echo 'sendemail-1'
        email = evaluate readTrusted('jenkinsFunctions/email.groovy')
        echo 'sendemail-2'
        if(currentBuild.currentResult == "UNSTABLE" || currentBuild.currentResult == "SUCCESS") {
                echo 'sendemail-3'
                testSummaryLib = evaluate readTrusted('jenkinsFunctions/testSummary.groovy')
                echo 'sendemail-4'
                testResultAction = currentBuild.rawBuild.getAction(AbstractTestResultAction.class)
                echo 'sendemail-5'
                testSummary =  testSummaryLib.getTestSummary(testResultAction)
                echo 'sendemail-6'
                testResultAction = null
                echo 'sendemail-7'
                email.sendEmail(testSummary)
                echo 'sendemail-8'
        }

        if(currentBuild.currentResult == "FAILURE") {
                echo 'sendemail-failure-1'
                email.sendEmailFailure()
                echo 'sendemail-failure-2'
        }
}
return this

