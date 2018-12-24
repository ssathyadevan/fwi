#!groovy

def sendEmail(testSummary) {

        mail from: "noreply-jenkins-FWI@alten.nl", \

        to: "${COMITTER_EMAIL}", \

        subject: " ${env.JOB_NAME} returned status " + currentBuild.currentResult, \

        body: "Dear ${AUTHOR_NAME},\n\nYour commit: ${SHORT_COMMIT_CODE} \nBranch: ${env.JOB_NAME}\nRan with status: " + currentBuild.currentResult + "\n" \

        "The following is the summary of the test results.\n" + testSummary

        echo "Email sent"

}

def sendEmailFailure() {

        mail from: "noreply-jenkins-FWI@alten.nl", \

        to: "${COMITTER_EMAIL}", \

        subject: " ${env.JOB_NAME} returned status " + currentBuild.currentResult, \

        body: "Dear ${AUTHOR_NAME},\n\nYour commit: ${SHORT_COMMIT_CODE}\nBranch: ${env.JOB_NAME}\nRan with status: " \

        + currentBuild.currentResult +  "\n\n" + \

        "Please check the Jenkins server to diagnose the problem"

        echo "Email sent"

}

return this
