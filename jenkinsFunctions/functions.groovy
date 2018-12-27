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
        email = evaluate readTrusted('jenkinsFunctions/email.groovy')
        echo 'send -email-1'
        if(currentBuild.currentResult == "UNSTABLE" || currentBuild.currentResult == "SUCCESS") {
                echo 'send -email-2'
                testSummaryLib = evaluate readTrusted('jenkinsFunctions/testSummary.groovy')
                echo 'send -email-3'
                testResultAction = currentBuild.rawBuild.getAction(AbstractTestResultAction.class)
                echo 'send -email-4'
                testSummary =  testSummaryLib.getTestSummary(testResultAction)
                echo 'send -email-5'
                testResultAction = null
                echo 'send -email-6'
                email.sendEmail(testSummary)
        }

        if(currentBuild.currentResult == "FAILURE") {
                email.sendEmailFailure()
        }
}
return this

