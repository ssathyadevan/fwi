#ifndef KAPPAOPERATOR_H
#define KAPPAOPERATOR_H

#include "greensSerial.h"


pressureFieldComplexSerial kappaOperator(const pressureFieldSerial &currentPressureFieldSerial)
{

    std::complex<double> *kOperator;

    for (int i = 0; i < m_freq.nFreq * m_src.nSrc * m_recv.nRecv; i++)
    {
        kOperator[i] = Summation( *m_Kappa[i], currentPressureFieldSerial);
    }

    return kOperator;
}

pressureFieldComplexSerial kappaOperator(const pressureFieldComplexSerial &)
{

}

class kappaOperator
{
    public:

    kappaOperator(ForwardModelInterface *forwardModel);
    ~kappaOperator();

    void calculateKappa( const pressureFieldSerial &ChiEst);

    pressureFieldSerial calculateKappaOperator(pressureFieldSerial);

    std::complex<double>* getResidual();
    double getResidualNormSq(double eta);

    const pressureFieldSerial& getPdata(std::complex<double> *p_data, pressureFieldSerial chiEst);

private:

    Greens_rect_2D_cpu **m_greens;
    pressureFieldComplexSerial **m_Kappa;
    pressureFieldComplexSerial **m_pData;
    std::complex<double> *m_residual;
    std::complex<double> *m_kZeta ;

    const grid2D& m_grid;
    const sources& m_src;
    const receivers& m_recv;
    const frequenciesGroup& m_freq;

    void createGreensSerial();
    void deleteGreensSerial();


    void calculateResidual();
    void calculateResidualNormSq(double eta);

    void createPdata(std::complex<double> *p_data, pressureFieldSerial chiEst);

};

#endif


