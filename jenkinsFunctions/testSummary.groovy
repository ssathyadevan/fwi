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
                echo "total is: ${total}"
                skipped = testResultAction.getSkipCount().toString()
                echo 'testSummary-5'
                failed = testResultAction.getFailCount().toString()
                echo 'testSummary-6'
                failDiffString = testResultAction.getFailureDiffString().toString()
                echo 'testSummary-7'
                failedTestList = testResultAction.getFailedTests().toString()
                echo 'testSummary-8'
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
        return summary
}
return this
