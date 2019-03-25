#python scripts that reads result from tests
f=open("RegressionTest_Passed.txt","r")
result=f.readlines()
f.close()
f=open("Regression_results.txt","a")
f.write(" \n".join(result))
f.write("\n")
f.close()