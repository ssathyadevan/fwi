import sys, os, shutil

#
# 	This script automatically runs all regression tests and writes the results to
# 	a file in directory $HOMEPATH/FWIIntsall/test{}.format(inversionmodel)/Regression_results.txt.
#	This script should be in the directory $HOMEPATH (to run it locally),
#       together with folders parallelized-fwi and FWIInstall, 
#       When running from terminal, type python3 unified_run_all_regressions_python.py, followed by 1 to run it locally
#       or 0 to run on Jenkins, forwardmodel (integralForwardModel or finiteDifferenceForwardModel) and inversion model (conjugateGradientInversion or gradientDescentInversion)
#       If you want to run one single regression test, add the name of the
#       testcase you want. For example: "python3 run_all_regressions_python.py 1 integralForwardModel conjugateGradientInversion fast" will run locally only the
#       regression for the testcase "fast"

cwd = os.getcwd()
ft=cwd

if len(sys.argv) < 4 or len(sys.argv) > 5:
    print("Please enter 1 for running locally or 0 for running on Jenkins, forwardmodel (integralForwardModel or finiteDifferenceForwardModel) "
          "and inversion model (conjugateGradientInversion or gradientDescentInversion)")
    sys.exit(1)

if sys.argv[1]=='1':
    print("You selected to run locally") 
    FWI_INSTALL_PATH=os.path.join(ft,"FWIInstall")        #for locally
    FWI_SOURCE_PATH=os.path.join(ft,"parallelized-fwi")   #for locally
    BUILD_PATH = os.path.join(os.path.join(ft, "Build"))

    if not os.path.exists(BUILD_PATH):
        print("Can not build project in {} because it does not exist, make sure it exists".format(BUILD_PATH))
        sys.exit(1)

    ### Make sure you use the current state of the project
    # Build project
    print("Build project in {}".format(BUILD_PATH))
    os.chdir(os.path.join(BUILD_PATH))
    os.system("make install")
    # Copy Build to FWIInstall
    if os.path.exists(os.path.join(FWI_INSTALL_PATH, "bin")):
        shutil.rmtree(os.path.join(FWI_INSTALL_PATH, "bin"))
    shutil.copytree(os.path.join(BUILD_PATH, "runtime", "bin"), os.path.join(FWI_INSTALL_PATH, "bin"))
    os.chdir(ft)

elif sys.argv[1]=='0':
    print("You selected to run on Jenkins") 
    FWI_INSTALL_PATH= os.path.join(ft,"FWIInstall")   #for Jenkins
    FWI_SOURCE_PATH= os.path.join(ft)                 #for Jenkins
    BUILD_PATH = os.path.join(os.path.join(ft, "build"))
else:
    print("Please input 1 for running locally or 0 for running on Jenkins")
    sys.exit(1)

if sys.argv[2] == "integralForwardModel":
    forwardmodel = "integralForwardModel"
elif sys.argv[2] == "finiteDifferenceForwardModel":
    forwardmodel = "finiteDifferenceForwardModel"
else:
    print("Forward model in not recognized")
    sys.exit(1)

if sys.argv[3] == "conjugateGradientInversion":
    inversionmodel = "conjugateGradientInversion"
    regression_data_folder = os.path.join(FWI_SOURCE_PATH,"tests","regression_data", "ConjugateGradientTestData")
elif sys.argv[3] == "gradientDescentInversion":
    inversionmodel = "gradientDescentInversion"
    regression_data_folder = os.path.join(FWI_SOURCE_PATH, "tests", "regression_data", "GradientDescentTestData")
else:
    print("Inversion model in not recognized")
    sys.exit(1)

print("\nRun Regression test with forwardmodel {} and inversionmodel {}".format(forwardmodel, inversionmodel))

os.chdir(os.path.join(regression_data_folder))

okur = os.getcwd()

tests=list()

if len(sys.argv)==5:
    testcase=sys.argv[4]
    tests.append(testcase)
elif len(sys.argv)==4:
    for name in os.listdir("."):
        if os.path.isdir(name):
            tests.append(name)

os.chdir(os.path.join(FWI_INSTALL_PATH))               #(FWI_INSTALL_PATH+"bin/")
cwd = os.getcwd()

regressiontestName = "test{}".format(inversionmodel)

if os.path.exists(regressiontestName):
    shutil.rmtree(regressiontestName)
os.mkdir(regressiontestName)

dir_from_which=os.path.join(FWI_SOURCE_PATH,"tests","testScripts")           

for basename in os.listdir(dir_from_which):
    if basename.endswith('.py'):
        pathname = os.path.join(dir_from_which, basename)
        if os.path.isfile(pathname):
            shutil.copy(pathname,os.path.join(FWI_INSTALL_PATH,regressiontestName))
os.chdir(os.path.join(FWI_INSTALL_PATH,regressiontestName))
cwd = os.getcwd()

for test in tests:
    print("###############################################################")
    print("              Regression test: {}".format(test))
    print("###############################################################")

    os.mkdir("{}RUN".format(test))

    shutil.copytree(os.path.join(regression_data_folder,"{}".format(test)),os.path.join(FWI_INSTALL_PATH,regressiontestName,"{}".format(test)))
    shutil.copytree(os.path.join(regression_data_folder,"{}".format(test),"input"), os.path.join(FWI_INSTALL_PATH,regressiontestName,"{}RUN".format(test),"input"))

    os.mkdir(os.path.join("{}RUN".format(test),"output"))

    print()

    if forwardmodel == "integralForwardModel":
        os.system(os.path.join(FWI_INSTALL_PATH,"bin","FWI_PreProcess {}RUN".format(test)))
    elif forwardmodel == "finiteDifferenceForwardModel":
        os.system(os.path.join(FWI_INSTALL_PATH, "bin", "FWI_PreProcess_Finite_Difference {}RUN".format(test)))
    else:
        print("Forward model is not recognized")
        sys.exit(1)

    os.system(os.path.join(FWI_INSTALL_PATH,"bin","FWI_UnifiedProcess {}RUN {} {}".format(test, inversionmodel, forwardmodel)))
    os.system("python3 regressionTestPreProcessing_python3.py {} {}RUN".format(test,test))

    destdir = "Regression_results.txt"
    f=open(destdir,'a')
    f.write("Passed {} regression test: ".format(test))
    f.close()

    os.system("python3 unified_regressionTestProcessing_python3.py {} {}RUN {} {}".format(test,test, inversionmodel, forwardmodel))
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
#shutil.rmtree(os.path.join(FWI_INSTALL_PATH,regressiontestName))

