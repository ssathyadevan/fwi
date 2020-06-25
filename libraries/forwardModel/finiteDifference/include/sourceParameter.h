#pragma once

<<<<<<< HEAD
namespace fwi
{
    namespace forwardModels
    {
        struct SourceParameter
        {
            int r;
            double beta;

            SourceParameter()
                : r(0)
                , beta(0.0)
            {
            }
            SourceParameter(int r_, double beta_)
                : r(r_)
                , beta(beta_)
            {
            }
        };
    }   // namespace forwardModels
}   // namespace fwi
=======
struct SourceParameter
{
    int r;
    double beta;

    SourceParameter()
        : r(0)
        , beta(0.0)
    {
    }
    SourceParameter(int r_, double beta_)
        : r(r_)
        , beta(beta_)
    {
    }
};
>>>>>>> Enforce one struct per file
