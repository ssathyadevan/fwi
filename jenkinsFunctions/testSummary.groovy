#!groovy
import com.cloudbees.groovy.cps.NonCPS

@NonCPS
def getTestSummary(testResultAction) {
            summary = ""

/*
                if (testResultAction != null) {
                total = testResultAction.getTotalCount()
                skipped = testResultAction.getSkipCount()
                failed = testResultAction.getFailCount()
                failDiffString = testResultAction.getFailureDiffString()
                failedTestList = testResultAction.getFailedTests()
                failedTestString = ""

                echo 'reached before summary'

                summary = "Test results:\n\t"
                summary = summary + ("Total: " + total)
                summary = summary + (", Passed: " + (total - failed - skipped))
                summary = summary + (", Failed: " + failed + " " + failDiffString)
                summary = summary + (", Skipped: " + skipped)
                if (failedTestList != null && !failedTestList.isEmpty()) {
                        for (failedTests in failedTestList) {
                                failedTestString = failedTestString + "---Test Class: "+failedTests.getClassName()+" Test: "+failedTests.getName()+"\n\t" +failedTests.getErrorDetails()+"\n"
                        }

                        summary = summary + "\n\nFailed tests:\n"
                        summary = summary + failedTestString
                }
        }
        else {
                summary = "No tests found"
        }

*/
        return summary
}
return this
