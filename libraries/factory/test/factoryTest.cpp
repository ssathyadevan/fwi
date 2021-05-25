#include "factory.h"
#include "ForwardModelMock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace fwi
{
    class factoryTest : public ::testing::Test
    {
    public:
        const core::CostFunctionCalculator _costCalculator;
        const std::array<double, 2> _xMin{0.0, 0.0};
        const std::array<double, 2> _xMax{2.0, 2.0};
        const std::array<int, 2> _nX{2, 4};
        const core::freqInfo _freq{1.0, 2.0, 2};
        const core::grid2D _grid{_xMin, _xMax, _nX};
        const core::Sources _sources{_xMin, _xMax, 2};
        const core::Receivers _receivers{_xMin, _xMax, 2};
        const core::FrequenciesGroup _frequencies{_freq, 2000.0};
        const int _lengthOfPData = _sources.count * _frequencies.count * _receivers.count;
        double _errorFunctionalScalingFactor = 1.0;

        const bool _doRegression = false;
        const double _tolerance = 0.01;
        const double _startChi = 0.0;
        const int _maxIterations = 10;
        const double _slope = -0.01;
        NiceMock<forwardModels::ForwardModelMock> _forwardModel;
    };

}   // namespace fwi
