import os, shutil
#from shutil import copyfile

#
# 	This script automatically runs all regression tests and writes the results to
# 	a file in directory $HOMEPATH/Documents/FWI/build/Regression_results.txt.
#	This script should be in the directory $HOMEPATH/Documents/FWI/FWIInstall (to run it locally), 
#       together with
#	the scripts read_pytest.py, read_results.py, regressionTestPreProcessing_python3.py 
#       and regressionTestProcessing_python3.py.
#

cwd = os.getcwd()
ft=cwd
print(cwd)   #/var/jenkins_home/workspace/FWI/FunctionalTests
pd=os.path.abspath(os.path.join(cwd, os.pardir))
print(pd)     #/var/jenkins_home/workspace/FWI
FWI_INSTALL_PATH =pd+"/FWIInstall/"
#FWI_SOURCE_PATH =pd+"/parallelized-fwi/" #for running locally
FWI_SOURCE_PATH =ft #for running on Jenkins


os.chdir(FWI_SOURCE_PATH+ "/tests/regression_data")
cwd = os.getcwd()

tests=list()
for name in os.listdir("."):
    if os.path.isdir(name):
        tests.append(name)

os.chdir(ft+ "/FWIInstall/bin")               #(FWI_INSTALL_PATH+"bin/")
cwd = os.getcwd()

os.makedirs("test", exist_ok=True)   #os.mkdir("test")
dir_from_which=FWI_SOURCE_PATH+ "/tests/testScripts/" 


for basename in os.listdir(dir_from_which):
    if basename.endswith('.py'):
        pathname = os.path.join(dir_from_which, basename)
        if os.path.isfile(pathname):
            shutil.copy(pathname, cwd+"/test/")
os.chdir(cwd+"/test/")
cwd = os.getcwd()
#print(cwd)
for test in tests:
    print("###############################################################")
    print("              Regression test: {}".format(test))
    print("###############################################################")

    os.mkdir("{}RUN".format(test))
#    print(FWI_SOURCE_PATH+ "tests/regression_data/{}".format(test))

    shutil.copytree(FWI_SOURCE_PATH+ "/tests/regression_data/{}".format(test), cwd+"/{}".format(test))
    shutil.copytree(FWI_SOURCE_PATH+ "/tests/regression_data/{}/input".format(test), cwd+"/{}RUN/input".format(test))
    shutil.copytree(FWI_SOURCE_PATH+ "/tests/regression_data/{}/output".format(test), cwd+"/{}RUN/output".format(test))

    os.system(FWI_INSTALL_PATH+"/bin/FWI_PreProcess.exe {}RUN".format(test))
    os.system(FWI_INSTALL_PATH+"/bin/FWI_Process.exe {}RUN".format(test))
    os.system("py regressionTestPreProcessing_python3.py {} {}RUN".format(test,test))

    destdir = "Regression_results.txt"
    f=open(destdir,'a')
    f.write("Passed {} regression test: ".format(test))
    f.close()

    os.system("py regressionTestProcessing_python3.py {} {}RUN".format(test,test))
    os.system("py read_results.py")

    destdir2 = "testname.txt"
    f=open(destdir2,'a')
    f.write("{}".format(test))
    f.close()
    f=open(destdir,'a')
    f.write("Passed {} unit test: ".format(test))
    f.close()

    os.system("py -m pytest python_unittest.py --junitxml={}results.xml".format(test))
    os.system("py read_pytest.py")
    os.remove(destdir2)

    shutil.copy("Regression_results.txt", ft+"/Regression_results.txt")

f=open(ft+"Regression_results.txt",'r')
print(f.read())
f.close()
#not sure why 
os.chdir(FWI_INSTALL_PATH)
shutil.rmtree("test")


