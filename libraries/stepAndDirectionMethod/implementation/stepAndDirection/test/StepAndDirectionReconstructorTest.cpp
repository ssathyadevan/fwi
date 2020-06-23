#include "StepAndDirectionReconstructor.h"
#include "FixedStepSizeCalculator.h"
#include "GradientDescentDirectionCalculator.h"
#include "forwardmodelinterfacemock.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        // core::grid2D getGrid()
        //{
        //    std::array<double, 2> xMin = {0.0, 0.0};
        //    std::array<double, 2> xMax = {2.0, 2.0};
        //    std::array<int, 2> nX = {2, 4};

        //    core::grid2D grid(xMin, xMax, nX);
        //    return grid;
        //}

        // io::genericInput getGenericInput()
        //{
        //    io::genericInput gInput;
        //    gInput.c0 = 1.0;
        //    gInput.freq(0.0, 1.0, 2);
        //    gInput.reservoirTopLeftCornerInM = {0.0, 1.0};
        //    gInput.reservoirBottomRightCornerInM = {0.0, 1.0};
        //    gInput.core::sourcesTopLeftCornerInM = {0.0, 1.0};
        //    gInput.core::sourcesBottomRightCornerInM = {0.0, 1.0};
        //    gInput.core::receiversTopLeftCornerInM = {0.0, 1.0};
        //    gInput.core::receiversBottomRightCornerInM = {0.0, 1.0};
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
        //    core::freqInfo freq(0.0, 1.0, 2);

        //    core::grid2D grid = getGrid();
        //    core::dataGrid2D dataGrid(grid);
        //    DirectionInput directionInput(0.1, dataGrid, 5, 1.0);

        //    core::sources core::sources(xMin, xMax, 2);
        //    core::receivers core::receivers(xMin, xMax, 2);
        //    core::frequenciesGroup frequencies(freq, 2000.0);
        //    int magnitude = frequencies.nFreq * core::sources.nSrc * core::receivers.nrcv;
        //    forwardModels::ForwardModelInterfaceMock forwardModel(dataGrid, core::sources, core::receivers, frequencies);

        //    GradientDescentDirectionCalculator chosenDirection(eta, &forwardModelTest);

        //    FixedStepSizeCalculator chosenStepSize(2.0);

        //    StepAndDirectionReconstructor stepAndDirectionReconstructor(&chosenStepSize, &chosenDirection, &forwardModel, directionInput);

        //    std::vector<std::complex<double>> pData(magnidute);
        //    // core::dataGrid2D dataGridOnes(grid);
        //    for(int i = 0; i < magnitude; ++i)
        //    {
        //        // dataGridOnes.setValueAtIndex(1.0, i);
        //        pData[i] = {1.0, 0.0};
        //    }

        //    GenericInput gInput;

        //    core::dataGrid2D ReconstructOutput = stepAndDirectionReconstructor.reconstruct(pData, gInput);
        //}

        // TEST(StepAndDirectionReconstructorTest, calculateNextMoveTest)
        //{
        //    double eta = 1.0;
        //    std::array<double, 2> xMin = {0.0, 0.0};
        //    std::array<double, 2> xMax = {2.0, 2.0};
        //    core::freqInfo freq(0.0, 10.0, 5);

        //    core::grid2D grid = getGrid();
        //    core::dataGrid2D dataGrid(grid);
        //    DirectionInput directionInput(0.1, dataGrid, 1, 1.0);

        //    core::sources core::sources(xMin, xMax, 2);
        //    core::receivers core::receivers(xMin, xMax, 2);
        //    core::frequenciesGroup frequencies(freq, 2000.0);

        //    forwardModels::ForwardModelInterfaceMock forwardModel(dataGrid, core::sources, core::receivers, frequencies);
        //    GradientDescentDirectionCalculator chosenDirection(eta, &forwardModelTest);

        //    FixedStepSizeCalculator chosenStepSize(2.0);

        //    StepAndDirectionReconstructor stepAndDirectionReconstructor(&chosenStepSize, &chosenDirection, &forwardModel, directionInput);

        //    core::dataGrid2D ReconstructOutput = stepAndDirectionReconstructor.reconstruct(pData, gInput);
        //}
    }   // namespace inversionMethods
}   // namespace fwi
