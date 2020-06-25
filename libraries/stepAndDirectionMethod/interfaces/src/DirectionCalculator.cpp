#include "DirectionCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        DirectionCalculator::DirectionCalculator(double errorFunctionalScalingfactor, forwardModels::forwardModelInterface *forwardModel)
            : _errorFunctionalScalingFactor(errorFunctionalScalingfactor)
            , _forwardModel()
            , _direction(forwardModel->getGrid())
        {
            if(errorFunctionalScalingfactor < 0.0)
            {
                throw std::invalid_argument("Error functional scaling factor is negative");
            }
            _forwardModel = forwardModel;
            _direction.zero();
        }

        DirectionCalculator::~DirectionCalculator() {}
    }   // namespace inversionMethods
}   // namespace fwi
