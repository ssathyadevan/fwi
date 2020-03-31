#include "greensSerial.h"

using namespace Eigen;

Greens_rect_2D_cpu::Greens_rect_2D_cpu(
    const Grid2D &grid_, const std::function<std::complex<double>(double, double)> G_func_, const Sources &src_, const Receivers &recv_, double k_) :
    G_func(G_func_),
    grid(grid_), src(src_), recv(recv_), k(k_), G_vol(), G_recv()
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

PressureFieldComplexSerial Greens_rect_2D_cpu::ContractWithField(const PressureFieldComplexSerial &inputField) const
{
    // Assure we are working on the same grid
    assert(grid == inputField.GetGrid());
    PressureFieldComplexSerial outputField(grid);
    const std::array<int, 2> &nx = grid.GetGridDimensions();
    contract_Greens_rect_2D(G_vol, inputField, outputField, nx, 2 * nx[0] - 1);
    return outputField;
}

// Babak 2018 10 25: This method generates the dot product of two matrices Greens function and contrast sources dW
// Equation ID: "rel:buildField"
PressureFieldComplexSerial Greens_rect_2D_cpu::dot1(const PressureFieldComplexSerial &dW) const
{
    const std::array<int, 2> &nx1 = grid.GetGridDimensions();
    const int &nx = nx1[0];
    const int &nz = nx1[1];

    int l1, l2, l3, l4;

    const std::vector<std::complex<double>> dWData = dW.getData();

    assert(grid == dW.GetGrid());

    Matrix<std::complex<double>, Dynamic, 1, ColMajor> dW_vec, eigprod;
    Matrix<std::complex<double>, Dynamic, 1, ColMajor> dummy;

    dW_vec.resize(nx * nz, NoChange);
    eigprod.resize(nx * nz, NoChange);
    dummy.resize(nx, NoChange);

    for(int i = 0; i < nx * nz; i++)
    {
        dW_vec(i) = dWData[i];
    }

    for(int i = 0; i < nx * nz; i++)
    {
        eigprod(i) = double(0.0);
    }

    for(int i = 0; i < nz; i++)
    {
        l1 = i * nx;
        for(int j = 0; j < nz - i; j++)
        {
            l2 = j * nx;
            l3 = l1 + l2;

            dummy = (G_vol2.block(0, l1, nx, nx)) * dW_vec.block(l1 + l2, 0, nx, 1);
            eigprod.block(l2, 0, nx, 1) += dummy;

            if((2 * i + j < nz) && (i > 0))
            {
                l4 = 2 * l1 + l2;
                eigprod.block(l4, 0, nx, 1) += dummy;
            }
        }
    }

    for(int i = 1; i < nz; i++)
    {
        if(i <= nz - i)
        {
            l1 = i * nx;
            for(int j = 0; j < i; j++)
            {
                l2 = j * nx;
                l3 = l1 + l2;
                eigprod.block(l3, 0, nx, 1) += (G_vol2.block(0, l1, nx, nx)) * dW_vec.block(l2, 0, nx, 1);
            }
        }
        else
        {
            l1 = i * nx;
            int lnz_i = nz - i;
            for(int j = 0; j < lnz_i; j++)
            {
                l2 = j * nx;
                l3 = l1 + l2;
                eigprod.block(l3, 0, nx, 1) += (G_vol2.block(0, l1, nx, nx)) * dW_vec.block(l2, 0, nx, 1);
            }
        }
    }

    PressureFieldComplexSerial product(grid);
    std::vector<std::complex<double>> productData(product.GetNumberOfGridPoints(), 0.0);
    for(int i = 0; i < nx * nz; i++)
    {
        productData[i] = eigprod(i);
    }
    product.setData(productData);

    return product;
}

void Greens_rect_2D_cpu::create_Greens_volume()
{
    double vol = grid.GetCellVolume();
    const std::array<int, 2> &nx = grid.GetGridDimensions();
    const std::array<double, 2> &dx = grid.GetCellDimensions();

    for(int i = -nx[1] + 1; i <= nx[1] - 1; i++)
    {
        double z = i * dx[1];
        for(int j = -nx[0] + 1; j <= nx[0] - 1; j++)
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
    G_vol2.resize(nx, nx * nz);

    double p2_z = x_min[1] + dx[1] / double(2.0);
    for(int i = 0; i < nx; i++)
    {
        double p2_x = x_min[0] + (i + double(0.5)) * dx[0];
        PressureFieldComplexSerial G_x(grid);
        setGreensFunction(G_x, [this, vol, p2_x, p2_z](const double &x, const double &y) { return vol * G_func(k, dist(x - p2_x, y - p2_z)); });

        for(int j = 0; j < nx * nz; j++)
        {
            G_vol2(i, j) = G_x.getData()[j];
        }
    }
}

void Greens_rect_2D_cpu::create_Greens_recv()
{
    double vol = grid.GetCellVolume();
    PressureFieldComplexSerial G_bound_cpu(grid);
    for(int i = 0; i < recv.nRecv; i++)
    {
        double x_recv = recv.xRecv[i][0];
        double z_recv = recv.xRecv[i][1];
        PressureFieldComplexSerial *G_bound = new PressureFieldComplexSerial(grid);

        setGreensFunction(*G_bound, [this, vol, x_recv, z_recv](const double x, const double z) { return vol * G_func(k, dist(x - x_recv, z - z_recv)); });

        G_recv.push_back(G_bound);
    }
}

void Greens_rect_2D_cpu::delete_Greens_recv()
{
    for(int i = 0; i < recv.nRecv; i++)
    {
        delete G_recv[i];
    }
}

void Greens_rect_2D_cpu::setGreensFunction(PressureFieldComplexSerial &greensFunctionField, const std::function<std::complex<double>(double, double)> func)
{
    const std::array<int, 2> &nx = GetGrid().GetGridDimensions();
    const std::array<double, 2> &dx = GetGrid().GetCellDimensions();   // Babak 2018 10 29: get rid of template for grid_rect_2D
    const std::array<double, 2> &x_min = GetGrid().GetGridStart();     // Babak 2018 10 29: get rid of template for grid_rect_2D

    std::vector<std::complex<double>> greensFunctionData(greensFunctionField.GetNumberOfGridPoints(), 0.0);

    for(int j = 0; j < nx[1]; j++)
    {
        double z = x_min[1] + (double(0.5) + j) * dx[1];
        int nx_j = nx[0] * j;

        for(int i = 0; i < nx[0]; i++)
        {
            double x = x_min[0] + (double(0.5) + i) * dx[0];
            greensFunctionData[nx_j + i] = func(x, z);
        }
    }

    greensFunctionField.setData(greensFunctionData);
}
