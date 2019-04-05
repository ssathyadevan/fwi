import sys, os, shutil

#from shutil import copyfile

#
# 	This script automatically runs all regression tests and writes the results to
# 	a file in directory $HOMEPATH/Documents/FWI/build/Regression_results.txt.
#	This script should be in the directory $HOMEPATH/Documents/FWI/ (to run it locally), 
#       together with folders parallelized-fwi and FWIInstall,
#	the scripts read_pytest.py, read_results.py, regressionTestPreProcessing_python3.py 
#       and regressionTestProcessing_python3.py.
#

cwd = os.getcwd()
ft=cwd 

if sys.argv[1]=='1':
    print("You selected to run locally") 
    FWI_INSTALL_PATH=os.path.join(ft,"FWIInstall")        #for locally
    FWI_SOURCE_PATH=os.path.join(ft,"parallelized-fwi")   #for locally
elif sys.argv[1]=='0':
    print("You selected to run on Jenkins") 
    FWI_INSTALL_PATH= os.path.join(ft,"FWIInstall")   #for Jenkins
    FWI_SOURCE_PATH= os.path.join(ft)                 #for Jenkins
else:
    print("Please input 1 for running locally or 0 for running on Jenkins")
    
os.chdir(os.path.join(FWI_SOURCE_PATH,"tests","regression_data")) 

okur = os.getcwd()


tests=list()
for name in os.listdir("."):
    if os.path.isdir(name):
        tests.append(name)

os.chdir(os.path.join(FWI_INSTALL_PATH))               #(FWI_INSTALL_PATH+"bin/")
cwd = os.getcwd()

if os.path.exists("test"):
    shutil.rmtree("test")
os.makedirs("test")

dir_from_which=os.path.join(FWI_SOURCE_PATH,"tests","testScripts")           



for basename in os.listdir(dir_from_which):
    if basename.endswith('.py'):
        pathname = os.path.join(dir_from_which, basename)
        if os.path.isfile(pathname):
            shutil.copy(pathname,os.path.join(FWI_INSTALL_PATH,"test"))
os.chdir(os.path.join(FWI_INSTALL_PATH,"test"))
cwd = os.getcwd()

for test in tests:
    print("###############################################################")
    print("              Regression test: {}".format(test))
    print("###############################################################")

    os.mkdir("{}RUN".format(test))
#    print(FWI_SOURCE_PATH+ "tests/regression_data/{}".format(test))

    shutil.copytree(os.path.join(FWI_SOURCE_PATH, "tests","regression_data","{}".format(test)),os.path.join(FWI_INSTALL_PATH,"test","{}".format(test))) 
    shutil.copytree(os.path.join(FWI_SOURCE_PATH, "tests","regression_data","{}".format(test),"input"), os.path.join(FWI_INSTALL_PATH,"test","{}RUN".format(test),"input"))
    shutil.copytree(os.path.join(FWI_SOURCE_PATH, "tests","regression_data","{}".format(test),"output"), os.path.join(FWI_INSTALL_PATH,"test","{}RUN".format(test),"output"))

    os.system(os.path.join(FWI_INSTALL_PATH,"bin","FWI_PreProcess {}RUN".format(test)))
    os.system(os.path.join(FWI_INSTALL_PATH,"bin","FWI_Process {}RUN".format(test)))
    os.system("python3 regressionTestPreProcessing_python3.py {} {}RUN".format(test,test))

    destdir = "Regression_results.txt"
    f=open(destdir,'a')
    f.write("Passed {} regression test: ".format(test))
    f.close()

    os.system("python3 regressionTestProcessing_python3.py {} {}RUN".format(test,test))
    os.system("python3 read_results.py")

    destdir2 = "testname.txt"
    f=open(destdir2,'a')
    f.write("{}".format(test))
    f.close()
    f=open(destdir,'a')
    f.write("Passed {} unit test: ".format(test))
    f.close()

    os.system("python3 -m pytest python_unittest.py --junitxml={}results.xml".format(test))
    os.system("python3 read_pytest.py")
    os.remove(destdir2)

    shutil.copy("Regression_results.txt", os.path.join(ft,"Regression_results.txt"))
    if sys.argv[1]=='0':    
        shutil.copy("{}results.xml".format(test), os.path.join(ft,"build","{}results.xml".format(test)))
    else:
        shutil.copy("{}results.xml".format(test), os.path.join(ft,"{}results.xml".format(test)))

f=open(os.path.join(ft,"Regression_results.txt"),'r')
print(f.read())
f.close()
#not sure why 
os.chdir(os.path.join(FWI_INSTALL_PATH))
shutil.rmtree(os.path.join(FWI_INSTALL_PATH,"test"))

