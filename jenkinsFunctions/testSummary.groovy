#!groovy

@NonCPS

def getTestSummary(testResultAction) {
            echo 'testSummary-1'
            summary = ""
            echo 'testSummary-2'
        if (testResultAction != null) {
                echo 'testSummary-3'
                total = testResultAction.getTotalCount()
                echo 'testSummary-4'
                //skipped = testResultAction.getSkipCount()
                echo 'testSummary-5'
                //failed = testResultAction.getFailCount()
                echo 'testSummary-6'
                failDiffString = testResultAction.getFailureDiffString()
                echo 'testSummary-7'
                failedTestList = testResultAction.getFailedTests()
                echo 'testSummary-8'
                failedTestString = ""

                echo 'reached before summary'

                summary = "Test results:\n\t"
                summary = summary + ("Total: " + total)
                summary = summary + (", Passed: " + (total - testResultAction.getFailCount() - testResultAction.getSkipCount()))
                summary = summary + (", Failed: " + testResultAction.getFailCount() + " " + failDiffString)
                summary = summary + (", Skipped: " + testResultAction.getSkipCount())
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
        return summary
}
return this
