// #define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120

#include "FiniteDifferenceForwardModel.h"
#include "FiniteDifferenceForwardModelInput.h"
#include "ForwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"
#ifdef __APPLE__
#include <OpenCL/opencl.hpp>
#else
#include <CL/opencl.hpp>
#endif

namespace fwi
{
    class OpenCLParallelized
    { 
    protected:
        static cl::Program program;
        static cl::Context context;
        static cl::Device device; 
        cl_int err;
    public:
        OpenCLParallelized(){
        }
        static void setProgram(cl::Program prog);
        static void setContext(cl::Context con);
        static void setDevice(cl::Device dev);
    };
    namespace forwardModels
    {
        class FiniteDifferenceForwardModelParallelOpenCL : public FiniteDifferenceForwardModel, OpenCLParallelized
        {
        public:
            FiniteDifferenceForwardModelParallelOpenCL(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver,
                const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput);
            // Overriding ForwardModel
            std::vector<std::complex<double>> calculatePressureField(const core::dataGrid2D &chiEst);
            void calculateKappa();
            void calculatePTot(const core::dataGrid2D &chiEst);
            void getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes);
        };
    }   // namespace forwardModels
}   // namespace fwi

