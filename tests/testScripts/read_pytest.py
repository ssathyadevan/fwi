#reads resutls from pytest
f=open("testname.txt","r")
tname=f.readlines()
f.close()
tnamestring=str(tname)
newnewnew=tnamestring.strip('[')
newnew=newnewnew.strip(']')
different=newnew.strip("'")
new=different.strip("\\n")
filename=new+"results.xml"
f=open(filename,"r")
result=f.readlines()
f.close()
resultstring=str(result)
index = resultstring.find('errors')
res=resultstring[index+8]
f=open("Regression_results.txt","a")
if res=='0':
	f.write("True\n")
else:
	f.write("False\n")
f.close()
