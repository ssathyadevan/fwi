#include <iostream>
#include <vector>
#include "HelpTextProcessing.h"
#include "log.h"
#include "genericInputCardReader.h"
#include "argumentReader.h"
#include "cpuClock.h"
#include "factory.h"
#include "FiniteDifferenceForwardModelParallelOpenCL.h"
#include "chiIntegerVisualisation.h"
#include "cpuClock.h"
#include "createChiCSV.h"
#include "csvReader.h"
#include "factory.h"
#include "genericInputCardReader.h"
#include "log.h"
#include <iostream>
#include <vector>


void printHelpOrVersion(fwi::io::argumentReader &fwiOpts);
void executeFullFWI(const fwi::io::argumentReader &fwiOpts);
void doProcessOpenCL( const fwi::io::genericInput &gInput);
void writePlotInput(const fwi::io::genericInput &gInput, std::string msg);
std::string readKernelCode(std::string kernelFileName);


int main(int argc, char *argv[])
{
    {
         int num_el = 100000000;
    std::vector<float> vec(num_el); 
    std::vector<float> vec2(num_el);
    std::fill(vec.begin(), vec.end(), 1);
    std::fill(vec2.begin(), vec2.end(), 1);
    
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    
    cl::Platform platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU,&devices);
    cl::Device device = devices.front();
    std::string kernelPath = "../../applications/processingOpenCL/";
    std::string kernelFileName = "kernels";
    std::string kernelSource = readKernelCode(kernelPath + kernelFileName + ".cl");
    std::cout << kernelSource << std::endl;
    cl::Context context(device);
    cl::Program program(context,kernelSource);    
    cl_int err = program.build("-cl-std=CL1.2");
    
    // create memory buffer for input data
    cl::Buffer buf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float)*vec.size(), vec.data(), &err);
    // create kernel:
    cl::Kernel kern1(program, "vectorMultiplication");
    // set the first argument of the kernel
    err = kern1.setArg(0, buf);
    // command queue
    cl::CommandQueue queue(context, device, 0, &err);

    clock_t startTime = clock();
    err = queue.enqueueNDRangeKernel(kern1, cl::NullRange, cl::NDRange(vec.size()));
    err = queue.enqueueReadBuffer(buf, CL_TRUE, 0, sizeof(float)*vec.size(), vec.data());
    clock_t endTime = clock();

    clock_t clockTicksTaken = endTime - startTime;
    double timeInSecondsPar = clockTicksTaken / (double) CLOCKS_PER_SEC;

    std::cout << "parallel: " << timeInSecondsPar << " seconds\n" << "result: "<< vec[0] << std::endl;
    
    startTime = clock();
    for(int i = 0; i<num_el; i++){
        vec2[i] = sin(vec2[i])*sin(vec2[i]);
    }
     endTime = clock();
     clockTicksTaken = endTime - startTime;
     double timeInSecondsLin = clockTicksTaken / (double) CLOCKS_PER_SEC;

    std::cout << "regular: " << timeInSecondsLin << " seconds\n" << "result: "<< vec2[0] << std::endl;;
    std::cout << "speedup factor: " << timeInSecondsLin/timeInSecondsPar << std::endl;
    }
    
    // try
    // {
    //     std::vector<std::string> arguments = {argv + 1, argv + argc};
    //     fwi::io::argumentReader fwiOpts(arguments);
    //     printHelpOrVersion(fwiOpts);
        
    //     fwi::io::genericInputCardReader genericReader(fwiOpts);
    //     const fwi::io::genericInput gInput = genericReader.getInput();
    //     doProcessOpenCL(gInput);
    // }
    // catch(const std::exception &e)
    // {
    //     std::cerr << e.what() << std::endl;
    //     std::cout << "Use -h for help on parameter options and values." << std::endl;
    //     L_(fwi::io::lerror) << e.what() << std::endl;
    //     std::exit(EXIT_FAILURE);
    // }

}

void printHelpOrVersion(fwi::io::argumentReader &fwiOpts)
{
    if(fwiOpts.help)
    {
        std::cout << HELP_TEXT_PROCESSING << std::endl;
        std::exit(EXIT_SUCCESS);
    }

    if(fwiOpts.version)
    {
        std::cout << VERSION_PROCESSING << std::endl;
        std::exit(EXIT_SUCCESS);
    }

}

void doProcessOpenCL(const fwi::io::genericInput& gInput)
{    std::cout << "Inversion Processing Started" << std::endl; 

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);    
    cl::Platform platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU,&devices);
    cl::Device device = devices.front();
    std::string kernelPath = "../../applications/processingOpenCL/";
    std::string kernelFileName = "kernels";
    std::string kernelSource = readKernelCode(kernelPath + kernelFileName + ".cl");
    cl::Context context(device);
    cl::Program program(context, kernelSource);    
    cl_int err = program.build("-cl-std=CL1.2");

    fwi::OpenCLParallelized::setProgram(program);
    fwi::OpenCLParallelized::setContext(context);
    fwi::OpenCLParallelized::setDevice(device);

    // initialize the clock, grid sources receivers, grouped frequencies
    fwi::performance::CpuClock clock;
    fwi::core::grid2D grid(gInput.reservoirTopLeftCornerInM, gInput.reservoirBottomRightCornerInM, gInput.nGrid);
    fwi::core::Sources source(gInput.sourcesTopLeftCornerInM, gInput.sourcesBottomRightCornerInM, gInput.nSources);
    fwi::core::Receivers receiver(gInput.receiversTopLeftCornerInM, gInput.receiversBottomRightCornerInM, gInput.nReceivers);
    fwi::core::FrequenciesGroup freq(gInput.freq, gInput.c0);

    // initialize logging
    std::string logFileName = gInput.outputLocation + gInput.runName + "Process.log";

    if(!gInput.verbose)
    {
        std::cout << "Printing the program output onto a file named: " << logFileName << std::endl;
        fwi::io::initLogger(logFileName.c_str(), fwi::io::ldebug);
    }

    // Logging the setup
    L_(fwi::io::linfo) << "Starting Inversion Process";
    source.Print();
    receiver.Print();
    freq.Print(gInput.freq.nTotal);

    // Logging chi from input
    L_(fwi::io::linfo) << "Visualisation of input chi (to be reconstructed)";
    fwi::io::chi_visualisation_in_integer_form(gInput.inputFolder + gInput.fileName + ".txt", gInput.nGridOriginal[0]);
    fwi::io::createCsvFilesForChi(gInput.inputFolder + gInput.fileName + ".txt", gInput, "chi_reference_");
    
    // Start inversion
    clock.Start();

    // read referencePressureData from a CSV file format
    std::string fileLocation = gInput.outputLocation + gInput.runName + "InvertedChiToPressure.txt";
    std::ifstream file(fileLocation);
    fwi::io::CSVReader row;
    

    if(!file.is_open())
    {
        L_(fwi::io::linfo) << "Could not open file at " << fileLocation;
        throw std::runtime_error("Could not open file at " + fileLocation);
    }

    int magnitude = freq.count * source.count * receiver.count;
    std::vector<std::complex<double>> referencePressureData(magnitude);
    int i = 0;
    L_(fwi::io::linfo) << "Read reference data" << fileLocation;
    while(file >> row)
    {
        if(i < magnitude)
        {
            referencePressureData[i] = {atof(row[0].c_str()), atof(row[1].c_str())};
        }
        ++i;
    }
    
    // Create model
    fwi::Factory factory;
    L_(fwi::io::linfo) << "Create ForwardModel";
    fwi::forwardModels::ForwardModelInterface *model;
    model = factory.createForwardModel(gInput.caseFolder, gInput.forward + "ForwardModel", grid, source, receiver, freq);

    L_(fwi::io::linfo) << "Create inversionModel";
    fwi::inversionMethods::inversionInterface *inverse;

    std::cout << "Creating InversionProcess with:" << std::endl 
            << "  forward =" << gInput.forward << std::endl
            << "  inversion =" << gInput.inversion << std::endl;

    L_(fwi::io::linfo) << "Create UnifiedInversionReconstructor";
    inverse = factory.createInversion(gInput.inversion + "Inversion", model, gInput);

    std::cout << "Calculating..." << std::endl;
    L_(fwi::io::linfo) << "Estimating Chi...";
    fwi::core::dataGrid2D chiEstimate = inverse->reconstruct(referencePressureData, gInput);

    L_(fwi::io::linfo) << "Writing to file";
    chiEstimate.toFile(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt");

    clock.End();

    L_(fwi::io::linfo) << "Visualisation of the estimated chi using FWI";
    fwi::io::chi_visualisation_in_integer_form(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput.nGrid[0]);
    fwi::io::createCsvFilesForChi(gInput.outputLocation + "chi_est_" + gInput.runName + ".txt", gInput, "chi_est_");

    std::string msg = clock.OutputString();
    writePlotInput(gInput, msg);
    fwi::io::endLogger();

    std::cout << "InversionProcess completed" << std::endl;
}

void writePlotInput(const fwi::io::genericInput &gInput, std::string msg)
{
    // This part is needed for plotting the chi values in postProcessing.py
    std::ofstream outputfwi;
    std::string runName = gInput.runName;
    outputfwi.open(gInput.outputLocation + runName + ".pythonIn");
    outputfwi << "This run was parametrized as follows:" << std::endl;
    outputfwi << "nxt   = " << gInput.nGrid[0] << std::endl;
    outputfwi << "nzt   = " << gInput.nGrid[1] << std::endl;
    outputfwi << "nxt_original   = " << gInput.nGridOriginal[0] << std::endl;
    outputfwi << "nzt_original   = " << gInput.nGridOriginal[1] << std::endl << msg;
    outputfwi.close();

    // This part is needed for plotting the chi values in postProcessing.py
    std::ofstream lastrun;
    lastrun.open(gInput.outputLocation + "/lastRunName.txt");
    lastrun.close();
    lastrun << runName;

}

std::string readKernelCode(std::string kernelFileName){
	std::ifstream file(kernelFileName, std::ios::binary);
    std::string fileStr;

    std::istreambuf_iterator<char> inputIt(file), emptyInputIt;
    std::back_insert_iterator<std::string> stringInsert(fileStr);

    copy(inputIt, emptyInputIt, stringInsert);

    return fileStr;
}