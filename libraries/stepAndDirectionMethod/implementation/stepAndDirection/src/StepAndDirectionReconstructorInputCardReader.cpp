#include <iostream>

#include "StepAndDirectionReconstructorInputCardReader.h"
#include "json.h"

StepAndDirectionReconstructorInputCardReader::StepAndDirectionReconstructorInputCardReader(const std::string &caseFolder) : inputCardReader()
{
    readCard(caseFolder);
}

StepAndDirectionReconstructorInput StepAndDirectionReconstructorInputCardReader::getInput() { return _input; }

void StepAndDirectionReconstructorInputCardReader::readCard(const std::string &caseFolder)
{
    (void)caseFolder;
    // nlohmann::json j = readFile(caseFolder + "/input/GradientDescentInversionInput.json");
    // structorInput input{j["gamma0"], j["x0"], j["h"], j["iter"]};

    readReconstructorParameters();
    readStepSizeParameters();
    readDirectionParameters();

    const bool doRegularisation = false;
    _input.doConjugateGradientRegularisation = doRegularisation;
}

void StepAndDirectionReconstructorInputCardReader::readReconstructorParameters()
{
    ReconstructorParameters reconstructorInput;

    const double tolerance = 0.01;
    if(tolerance <= 0.0)
    {
        throw std::invalid_argument("Invalid tolerance in .....json");
    }
    reconstructorInput.tolerance = tolerance;

    const double startingChi = 0.5;
    reconstructorInput.startingChi = startingChi;

    const int maxIterationsNumber = 10;
    if(maxIterationsNumber <= 0)
    {
        throw std::invalid_argument("Invalid number of iterations in .....json");
    }
    reconstructorInput.maxIterationsNumber = maxIterationsNumber;

    _input.reconstructorParameters = reconstructorInput;
}

void StepAndDirectionReconstructorInputCardReader::readStepSizeParameters()
{
    StepSizeParameters stepsizeInput;

    const double initialStepSize = 1.0;
    if(initialStepSize <= 0.0)
    {
        throw std::invalid_argument("Invalid initial stepsize in ...json");
    }
    stepsizeInput.initialStepSize = initialStepSize;

    const double slope = -0.01;
    if(initialStepSize + slope * _input.reconstructorParameters.maxIterationsNumber < 0.0)
    {
        throw std::invalid_argument("Error: linear stepSize will become negative during the reconstruction.");
    }
    if(slope > 0.0)
    {
        throw std::invalid_argument("Invalid positive slope, the stepsize will increase over time.");
    }
    stepsizeInput.slope = slope;

    _input.stepSizeParameters = stepsizeInput;
}

void StepAndDirectionReconstructorInputCardReader::readDirectionParameters()
{
    DirectionParameters directionInput;
    const double derivativeStepSize = 1.0;
    if(derivativeStepSize <= 0.0)
    {
        throw std::invalid_argument("Invalid derivativeStepSize in ...json");
    }
    directionInput.derivativeStepSize = derivativeStepSize;

    _input.directionParameters = directionInput;
}
