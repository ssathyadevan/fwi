#pragma once

namespace inversionMethods
{
    struct EvolutionInversionInput
    {
        double toleranceOuter;
        int nGenerations;
        int nChildrenPerGeneration;
    };
}   // namespace inversionMethods
