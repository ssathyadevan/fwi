//#include "StepAndDirectionReconstructor.h"
//#include "FixedStepSizeCalculator.h"
//#include "GradientDescentDirectionCalculator.h"
//#include "forwardmodelinterfacemock.h"
//#include <gtest/gtest.h>

// grid2D getGrid()
//{
//    std::array<double, 2> xMin = {0.0, 0.0};
//    std::array<double, 2> xMax = {2.0, 2.0};
//    std::array<int, 2> nX = {2, 4};

//    grid2D grid(xMin, xMax, nX);
//    return grid;
//}

// genericInput getGenericInput()
//{
//    genericInput gInput;
//    gInput.c0 = 1.0;
//    gInput.freq(0.0, 1.0, 2);
//    gInput.reservoirTopLeftCornerInM = {0.0, 1.0};
//    gInput.reservoirBottomRightCornerInM = {0.0, 1.0};
//    gInput.sourcesTopLeftCornerInM = {0.0, 1.0};
//    gInput.sourcesBottomRightCornerInM = {0.0, 1.0};
//    gInput.receiversTopLeftCornerInM = {0.0, 1.0};
//    gInput.receiversBottomRightCornerInM = {0.0, 1.0};
//    gInput.nGridOriginal = {1, 1};   //??
//    gInput.nGrid = {1, 1};           //??
//    gInput.nSources = 2;
//    gInput.nReceivers = 2;

//    return gInput;
//}

// TEST(StepAndDirectionReconstructorTest, ReconstructTest)
//{
//    double eta = 1.0;
//    std::array<double, 2> xMin = {0.0, 0.0};
//    std::array<double, 2> xMax = {2.0, 2.0};
//    freqInfo freq(0.0, 1.0, 2);

//    grid2D grid = getGrid();
//    dataGrid2D dataGrid(grid);
//    DirectionInput directionInput(0.1, dataGrid, 5, 1.0);

//    sources sources(xMin, xMax, 2);
//    receivers receivers(xMin, xMax, 2);
//    frequenciesGroup frequencies(freq, 2000.0);
//    int magnitude = frequencies.nFreq * sources.nSrc * receivers.nrcv;
//    ForwardModelInterfaceMock forwardModel(dataGrid, sources, receivers, frequencies);

//    GradientDescentDirectionCalculator chosenDirection(eta, &forwardModelTest);

//    FixedStepSizeCalculator chosenStepSize(2.0);

//    StepAndDirectionReconstructor stepAndDirectionReconstructor(&chosenStepSize, &chosenDirection, &forwardModel, directionInput);

//    std::vector<std::complex<double>> pData(magnidute);
//    // dataGrid2D dataGridOnes(grid);
//    for(int i = 0; i < magnitude; ++i)
//    {
//        // dataGridOnes.setValueAtIndex(1.0, i);
//        pData[i] = {1.0, 0.0};
//    }

//    GenericInput gInput;

//    dataGrid2D ReconstructOutput = stepAndDirectionReconstructor.reconstruct(pData, gInput);
//}

// TEST(StepAndDirectionReconstructorTest, calculateNextMoveTest)
//{
//    double eta = 1.0;
//    std::array<double, 2> xMin = {0.0, 0.0};
//    std::array<double, 2> xMax = {2.0, 2.0};
//    freqInfo freq(0.0, 10.0, 5);

//    grid2D grid = getGrid();
//    dataGrid2D dataGrid(grid);
//    DirectionInput directionInput(0.1, dataGrid, 1, 1.0);

//    sources sources(xMin, xMax, 2);
//    receivers receivers(xMin, xMax, 2);
//    frequenciesGroup frequencies(freq, 2000.0);

//    ForwardModelInterfaceMock forwardModel(dataGrid, sources, receivers, frequencies);
//    GradientDescentDirectionCalculator chosenDirection(eta, &forwardModelTest);

//    FixedStepSizeCalculator chosenStepSize(2.0);

//    StepAndDirectionReconstructor stepAndDirectionReconstructor(&chosenStepSize, &chosenDirection, &forwardModel, directionInput);

//    dataGrid2D ReconstructOutput = stepAndDirectionReconstructor.reconstruct(pData, gInput);
//}
