#!groovy

def sendEmail(String osName = "") {

        mail from: "noreply-jenkins-FWI@alten.nl", \

        to: "${COMITTER_EMAIL}", \

        subject: osName + ": " + "${env.JOB_NAME} returned status " + currentBuild.currentResult, \

        body: "Dear ${AUTHOR_NAME},\n\nYour commit: ${SHORT_COMMIT_CODE} \nBranch: ${env.JOB_NAME}\nRan with status: " + currentBuild.currentResult + " on" + osName

        echo "Email sent"

}

def sendEmailFailure(String osName = "") {

        mail from: "noreply-jenkins-FWI@alten.nl", \

        to: "${COMITTER_EMAIL}", \

        subject: osName + ": " +" ${env.JOB_NAME} returned status " + currentBuild.currentResult, \

        body: "Dear ${AUTHOR_NAME},\n\nYour commit: ${SHORT_COMMIT_CODE}\nBranch: ${env.JOB_NAME}\nRan with status: FAILURE\nStage where failure occurred: ${env.MYSTAGE_NAME} on ${osName},\nPlease check the Jenkins server console output to diagnose the problem: ${BUILD_URL}."

        echo "Email sent"

}

return this
