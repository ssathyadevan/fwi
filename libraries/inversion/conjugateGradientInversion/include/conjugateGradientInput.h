#pragma once
struct ConjugateGradientInput
{
      Iter1 iter1;
      DeltaAmplification deltaAmplification;
      bool doReg;
      int n_max;
};
