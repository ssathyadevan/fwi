#include <gtest/gtest.h>
#include <../include/forwardModelContainer.h>

TEST(FMContainerTest, TestDummy)
{
    std::string caseFolder = "/home/valesca/Exceptions_test/default";
    std::string inputFolder = "/home/valesca/Exceptions_test/default/input";
    std::string outputLocation = "/home/valesca/Exceptions_test/default/output";
    std::string runName = "default";
    double c_0 = 2000.0;
    Freq freq = {10.0, 40.0, 5};
    std::array<double,2> reservoirTopLeftCornerInM = {-3.0, 0.0};
    std::array<double,2> reservoirBottomRightCornerInM = {3.0, 3.0} ;
    std::array<double,2> sourcesTopLeftCornerInM = {-4.0, -1.0} ;
    std::array<double,2> sourcesBottomRightCornerInM = {4.0, -1.0};
    std::array<double,2> receiversTopLeftCornerInM = {-4.0, -1.0};
    std::array<double,2> receiversBottomRightCornerInM = {4.0, -1.0};
    std::array<int,2> ngrid_original = {64, 32};
    std::array<int,2> ngrid = {3, 3};
    NSourcesReceivers nSourcesReceivers = {2, 2};
    std::string fileName = "temple";
    bool verbose = false;

    GenericInput input = {caseFolder, inputFolder, outputLocation, runName, c_0, freq, reservoirTopLeftCornerInM, reservoirBottomRightCornerInM, sourcesTopLeftCornerInM, sourcesBottomRightCornerInM,
                         receiversTopLeftCornerInM, receiversBottomRightCornerInM, ngrid_original, ngrid, nSourcesReceivers, fileName, verbose};


    Grid2D grid(input.reservoirTopLeftCornerInM, input.reservoirBottomRightCornerInM, input.ngrid);
    Sources src(input.sourcesTopLeftCornerInM, input.sourcesBottomRightCornerInM, input.nSourcesReceivers.nsources);
    Receivers recv(input.receiversTopLeftCornerInM, input.receiversBottomRightCornerInM, input.nSourcesReceivers.nreceivers);
    FrequenciesGroup frequenties(input.freq, input.c_0);

    ForwardModelContainer my_container(input,"integralForwardModel" , grid, src, recv, frequenties);
    (void)my_container;


    my_container.calculateKappaParallel();
    std::complex<double> complex1;
    std::complex<double> mycomplex(1.0,0.0);
    std::vector<std::complex<double>> pdata(nSourcesReceivers.nsources * nSourcesReceivers.nreceivers * freq.nTotal  ,mycomplex);
    complex1 = {3.0,0.0};
    pdata[11] = complex1;
    complex1 = {4.0,0.0};
    pdata[12] = complex1;
    PressureFieldSerial chiEst(grid);
    chiEst.Zero();
    chiEst += 10;
    std::vector<std::complex<double>> residuals= my_container.calculateResidualParallel(chiEst, pdata);
    (void)residuals;
    std::cout<< "Print Residuals: ";
    for (std::complex<double> & residual : residuals){
        std::cout<< " (" << residual.real() << ", " << residual.imag() << " i ) ";
    }
    std::cout << std::endl;

    double resNormSQ = my_container.calculateResidualNormSqParallel(residuals);
    std::cout<< "Total residual norm Square: " << resNormSQ << std::endl;

    EXPECT_EQ(1,1);
}
