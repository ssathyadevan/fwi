#include "ForwardModelBase.h"
#include <math.h>

namespace fwi
{
    namespace forwardModels
    {
        ForwardModelBase::ForwardModelBase(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver,
            const core::FrequenciesGroup &freq, const CostFunction costFunction)
            : _grid(grid)
            , _source(source)
            , _receiver(receiver)
            , _freq(freq)
            , _costFunction(costFunction)
        {
            _residual = std::vector<std::complex<double>>(_freq.count * _source.count * _receiver.count);
        }

        ForwardModelBase::~ForwardModelBase() {}

        const core::grid2D &ForwardModelBase::getGrid() { return _grid; }

        const core::Sources &ForwardModelBase::getSource() { return _source; }

        const core::Receivers &ForwardModelBase::getReceiver() { return _receiver; }

        const core::FrequenciesGroup &ForwardModelBase::getFreq() { return _freq; }

        CostFunction ForwardModelBase::getCostFunction() { return _costFunction; }
    }   // namespace forwardModels
}   // namespace fwi
