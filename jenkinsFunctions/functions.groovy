#!groovy

import hudson.tasks.test.AbstractTestResultAction

import hudson.model.Actionable

import com.cloudbees.groovy.cps.NonCPS


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
    cd build
    cp -r ../inputFiles ../FWIInstall/input
    cp ../inputFiles/test/ForwardModelInput.in ../build/
    make test
    ctest -T test --no-compress-output
    cp Testing/`head -n 1 Testing/TAG`/Test.xml ./CTestResults.xml
    '''
}

def regressiontest() {
        echo 'Running regression tests'
#        sh '''
#        mkdir input output
#        cp inputFiles/* input/
#        cd FWIInstall/bin
#        ./FWI_PreProcess ../../input/ ../../output/ default
#        ./FWI_Process ../../input/ ../../output/ default
#        cd ../../pythonScripts
#        cp postProcessing-python3.py ../
#        cd ..
#        python3 postProcessing-python3.py output/
#        mkdir test
#        cp tests/regression_data/fast/* test/
#        cp tests/testScripts/* test/
#        cp input/default.in test/
#        cp output/* test/
#        cd test
#        python3 regressionTestPreProcessing_python3.py fast default
#        python3 regressionTestProcessing_python3.py fast default
#        python3 -m pytest python_unittest.py --junitxml results.xml 
#        cp results.xml ../build/
#        '''
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
        if(currentBuild.currentResult == "UNSTABLE" || currentBuild.currentResult == "SUCCESS") {
                email.sendEmail()
        }
        if(currentBuild.currentResult == "FAILURE") {
                email.sendEmailFailure()
        }
}
return this

