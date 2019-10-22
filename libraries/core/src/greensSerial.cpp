#include "greensSerial.h"
#include "CL/cl2.hpp"
#include <chrono>
#include <exception>

using namespace Eigen;

Greens_rect_2D_cpu::Greens_rect_2D_cpu(const grid2D &grid_, const std::function< std::complex<double>(double,double) > G_func_, const sources &src_, const receivers &recv_, double k_)
    : G_func(G_func_), grid(grid_), src(src_), recv(recv_), k(k_), G_vol(), G_recv()
{
    const std::array<int, 2> &nx = grid.GetGridDimensions();
    G_vol = new std::complex<double>[(2 * nx[1] - 1) * (2 * nx[0] - 1)];
    create_Greens_volume();
    create_Greens_recv();
    create_Greens_volume_ankit();
}

Greens_rect_2D_cpu::~Greens_rect_2D_cpu()
{
    delete_Greens_recv();
    delete[] G_vol;
}

pressureFieldComplexSerial Greens_rect_2D_cpu::ContractWithField(const pressureFieldComplexSerial &x) const
{
    // Assure we are working on the same grid
    assert(&grid == &x.GetGrid());
    pressureFieldComplexSerial y(grid);
    std::complex<double> *y_data = y.GetDataPtr();
    const std::array<int, 2> &nx = grid.GetGridDimensions();
    contract_Greens_rect_2D(G_vol, x.GetDataPtr(), y_data, nx, 2 * nx[0] - 1);
    return y;
}

// Babak 2018 10 25: This method generates the dot product of two matrices Greens function and contrast sources dW
// Equation ID: "rel:buildField"
pressureFieldComplexSerial Greens_rect_2D_cpu::dot1(const pressureFieldComplexSerial &dW) const
{
    const std::array<int, 2> &nx1 = grid.GetGridDimensions();
    const int &nx = nx1[0];
    const int &nz = nx1[1];

    pressureFieldComplexSerial prod1(grid);
    int l1, l2, l3, l4;

    std::complex<double> *p_prod = prod1.GetDataPtr();
    const std::complex<double> *p_dW = dW.GetDataPtr();

    assert(&grid == &dW.GetGrid());

    bool success = false;//acceleratedDot(G_vol2, p_dW, p_prod); turn on when FPGA/GPU is available

    if (!success) //do CPU multiplication
    {
        Matrix< std::complex<double>, Dynamic, 1, ColMajor> dW_vec, eigprod;
        Matrix< std::complex<double>, Dynamic, 1, ColMajor> dummy;

        dW_vec.resize(nx*nz,NoChange);
        eigprod.resize(nx*nz,NoChange);
        dummy.resize(nx,NoChange);

        for(int i=0; i < nx*nz; i++) { dW_vec(i) = p_dW[i]; }
        eigprod.setZero();

        for(int i=0; i < nz; i++)
        {
            l1 = i*nx;
            for (int j=0; j < nz-i; j++)
            {
                l2 = j*nx;
                l3 = l1 + l2;

                product(dummy, G_vol2, dW_vec, nx, l1, l3);
                eigprod.block(l2,0,nx,1) += dummy;

                if ( (2*i + j < nz) && (i>0) )
                {
                    l4 = 2*l1 + l2;
                    eigprod.block(l4,0,nx,1) += dummy;
                }
            }
        }

        for(int i=1; i < nz; i++)
        {
            if (i <= nz-i)
            {
                l1 = i*nx;
                for (int j=0; j < i; j++)
                {
                    l2 = j*nx;
                    l3 = l1 + l2;

                    product(dummy, G_vol2, dW_vec, nx, l1, l2);
                    eigprod.block(l3,0,nx,1) += dummy;
                }
            }
            else
            {
                l1 = i*nx;
                int lnz_i = nz-i;
                for (int j=0; j < lnz_i; j++)
                {
                    l2 = j*nx;
                    l3 = l1 + l2;

                    product(dummy, G_vol2, dW_vec, nx, l1, l2);
                    eigprod.block(l3,0,nx,1) += dummy;
                }
            }
        }

        for(int i=0; i < nx*nz; i++)
        {
            p_prod[i] = eigprod(i);
        }
    }
    return prod1;
}

void Greens_rect_2D_cpu::checkSuccess(int success, std::string error) const
{
   if (success != CL_SUCCESS)
   {
     std::cout << error << ":  " << success << std::endl;
     throw std::exception();
   }
}

bool Greens_rect_2D_cpu::acceleratedDot( const Matrix<std::complex<double>, 1, Dynamic, RowMajor>& G_vol2, const std::complex<double> *dW, std::complex<double> *p_prod ) const
{
    try
    {
        int nx = GetGrid().GetGridDimensions()[0];
        int nz = GetGrid().GetGridDimensions()[1];
        int n = nx*nz;

        std::complex<double>* Gxx = new std::complex<double>[2*nx*nz - nz];
        for (int i = 0; i < nz; i++ )
        {
            int d1 = ( 2*nx - 1 ) * i + nx - 1;
            Gxx[ d1 ] = G_vol2[ i * nx ];
            for (int j = 1; j < nx; j++ )
            {
                Gxx[ d1 + j] = G_vol2[ i * nx + j];
                Gxx[ d1 - j] = G_vol2[ i * nx + j];
            }
        }

        std::vector<cl::Platform> platformList;
        cl_int success = cl::Platform::get(&platformList);
        checkSuccess(success, "no platform found");

        std::string platformName;
        success = platformList[0].getInfo( CL_PLATFORM_NAME, &platformName);
        checkSuccess(success, "no platform found");

        std::vector<cl::Device> deviceList;
        success = platformList[0].getDevices(CL_DEVICE_TYPE_ALL, &deviceList);
        checkSuccess(success, "no device found");

        cl::Context context( deviceList, nullptr, nullptr, nullptr, &success );
        checkSuccess(success, "no contex created");

        std::clock_t c_start = std::clock();
        auto t_start = std::chrono::high_resolution_clock::now();

        cl::Buffer bufGreens( context, CL_MEM_READ_ONLY, (2*n - nz) * sizeof(cl_double2), nullptr, &success );
        checkSuccess(success, "error allocating matrix A");

        cl::Buffer bufVecB( context, CL_MEM_READ_ONLY, n * sizeof(cl_double2), nullptr, &success );
        checkSuccess(success, "error allocating vector B");

        cl::Buffer bufVecC( context, CL_MEM_WRITE_ONLY, n * sizeof(cl_double2), nullptr, &success );
        checkSuccess(success, "error allocating vector C");

        std::ifstream inputFile( "../parallelized-fwi/inputFiles/matVecKernel2.cl" );
        std::string content( ( std::istreambuf_iterator<char>(inputFile) ), ( std::istreambuf_iterator<char>() ) );
        cl::Program::Sources source{ { content.c_str(), content.size() } };

        cl::Program program( context, source, &success );
        checkSuccess(success, "error could not create program");

        success = program.build( deviceList, nullptr, nullptr, nullptr );
        checkSuccess(success, "error could not build program");

        cl::Kernel kernel(program, "matVecKernel2", &success);
        checkSuccess(success, "error could not create kernel");

        success = kernel.setArg( 0, bufGreens );
        checkSuccess(success, "error could not set matA");
        success = kernel.setArg( 1, bufVecB );
        checkSuccess(success, "error could not set vectB");
        success = kernel.setArg( 2, bufVecC );
        checkSuccess(success, "error could not set vectC");
        success = kernel.setArg( 3, cl::Local(nx*sizeof(cl_double2)) );
        checkSuccess(success, "error could not set 4th argument (local memory)");
        success = kernel.setArg( 4, nx );
        checkSuccess(success, "error could not set nx");
        success = kernel.setArg( 5, nz );
        checkSuccess(success, "error could not set nz");

        int blockSize = 32;
        cl::NDRange range(n * blockSize);


        cl::CommandQueue queue(context, deviceList[0], CL_QUEUE_PROFILING_ENABLE, &success);

        std::vector<cl::Event> events;
        cl::Event event;
        success = queue.enqueueWriteBuffer( bufGreens, CL_FALSE, 0, (2*n - nz) * sizeof(cl_double2), Gxx, nullptr, &event );
        checkSuccess(success, "error could not enque write buffer matA");
        events.push_back(event);

        success = queue.enqueueWriteBuffer( bufVecB, CL_FALSE, 0, n * sizeof(cl_double2), dW, nullptr, &event );
        checkSuccess(success, "error could not enque write buffer vecB");
        events.push_back(event);

        success = queue.enqueueNDRangeKernel( kernel, cl::NullRange, range, cl::NDRange(blockSize), &events, &event );
        checkSuccess(success, "error could not execute kernel");
        events.clear();
        events.push_back(event);

        success = queue.enqueueReadBuffer( bufVecC, CL_TRUE, 0, n * sizeof(cl_double2), p_prod, &events, nullptr );
        //checkSuccess(success, "error could not copy vectC");

        auto t_end = std::chrono::high_resolution_clock::now();
        delete[] Gxx;
    }
    catch(std::exception )
    {
        return false;
    }
    return true;
}

void Greens_rect_2D_cpu::product(Matrix< std::complex<double>, Dynamic, 1, ColMajor>& dummyVector,
                                 const Matrix<std::complex<double>, 1, Dynamic, RowMajor>& G_vol2,
                                 const Matrix< std::complex<double>, Dynamic, 1, ColMajor>& dW_vec,
                                 int nx, int l1, int l2) const
{
    dummyVector.setZero();
    std::complex<double> dummy = .0;
    for (int i = 0; i < nx; i++)
    {
        for (int j = 0; j < nx - i; j++)
        {
            dummy = G_vol2[ l1 + i ] * dW_vec[ l2 + i + j];
            dummyVector[ j ] += dummy;
            if ( (2*i + j < nx) && i != 0 )
            {
                dummyVector[ 2*i + j ] += dummy;
            }
        }
    }

    for (int i = 1; i < nx; i++)
    {
        if (i <= nx-i)
        {
            for (int j = 0; j < i; j++)
            {
                dummyVector[ i + j ] += G_vol2[ l1 + i ] * dW_vec[ l2 + j];
            }
        }
        else
        {
            for (int j = 0; j < nx - i; j++)
            {
                dummyVector[ i + j ] += G_vol2[ l1 + i ] * dW_vec[ l2 + j];
            }
        }
    }
}

void Greens_rect_2D_cpu::create_Greens_volume()
{

    double vol = grid.GetCellVolume();
    const std::array<int, 2> &nx = grid.GetGridDimensions();
    const std::array<double, 2> &dx = grid.GetCellDimensions();

    for(int i=-nx[1]+1; i <= nx[1]-1; i++)
    {
        double z = i * dx[1];
        for(int j=-nx[0]+1; j <= nx[0]-1; j++)
        {
            double x = j * dx[0];
            double r = dist(z, x);
            std::complex<double> val = G_func(k, r);
            G_vol[(nx[1] + i - 1) * (2 * nx[0] - 1) + (nx[0] + j - 1)] = val * vol;
        }
    }
}

void Greens_rect_2D_cpu::create_Greens_volume_ankit()
{
    double vol = grid.GetCellVolume();
    const std::array<int, 2> &nx1 = grid.GetGridDimensions();
    const std::array<double, 2> &dx = grid.GetCellDimensions();
    const std::array<double, 2> &x_min = grid.GetGridStart();
    const int &nx = nx1[0];
    const int &nz = nx1[1];

    G_vol2.resize(1,nx*nz);

    double p2_z = x_min[1] + dx[1]/double(2.0);
    double p2_x = x_min[0] + ( double(0.5) )*dx[0];
    pressureFieldComplexSerial G_x(grid);

    G_x.SetField( [this,vol,p2_x,p2_z] (const double &x, const double &y)
    {return vol*G_func( k, dist(x-p2_x, y-p2_z) ); } );

    for (int j=0; j<nx*nz; j++)
        G_vol2(0,j) = *(G_x.GetDataPtr() + j);
}

void Greens_rect_2D_cpu::create_Greens_recv()
{
    double vol = grid.GetCellVolume();
    pressureFieldComplexSerial G_bound_cpu(grid);
    for(int i=0; i < recv.nRecv; i++)
    {
        double x_recv = recv.xRecv[i][0];
        double z_recv = recv.xRecv[i][1];
        pressureFieldComplexSerial *G_bound = new pressureFieldComplexSerial(grid);
        G_bound->SetField(
                    [this, vol, x_recv, z_recv](const double x, const double z)
        { return vol * G_func(k, dist(x - x_recv, z - z_recv)); });
        G_recv.push_back(G_bound);
    }
}

void Greens_rect_2D_cpu::delete_Greens_recv()
{
    for(int i=0; i < recv.nRecv; i++)
    {
        delete G_recv[i];
    }
}
