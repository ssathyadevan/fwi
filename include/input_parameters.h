#ifndef INPUT_PARAMETERS
#define INPUT_PARAMETERS

const int nxt = 128;
const int nzt = 64;
const int nSrct = 17;
const int nFreq_Total = 30;

const int calc_alpha=0; //put 1 if alpha in calcField.h is to be calculated.
const int n_max = 4;
const int n_iter1 = 30; //for chi update
const int n_iter2 = 100; //for p_tot update

const double tol1 = 1e-5; //CG tol for chi update
const double tol2 = 5e-5; //tol for p_tot update

const int do_reg = 1; //put 1 to turn on regularization
const int interactive = 1; //put 1 to turn on interactive python session

const double F_min1 = 10.0;
const double F_max1 = 40.0;

const int freq_dist_group = 1; //if group distribution of frequency is required

const int nFreq_input[] = {12,7,6,5};//{4,2,2,2};// {9,5,3,3};

const int gpu = 1; //to turn on gpu computations

// weight of the regularization
const double delta_amplification_start = 100.0;
const double delta_amplification_slope = 10.0;

#endif // INPUT_PARAMETERS

