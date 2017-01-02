#ifndef INPUT_PARAMETERS
#define INPUT_PARAMETERS

const int calc_alpha=0; //put 1 if alpha in calcField.h is to be calculated.
const int n_max = 1;
const int n_iter1 = 30; //for chi update
const int n_iter2 = 25; //for p_tot update

const double tol1 = 1e-6; //CG tol for chi update
const double tol2 = 5e-5; //tol for p_tot update

const int do_reg = 0; //put 1 to turn on regularization
const int interactive = 1; //put 1 to turn on interactive python session

const int nFreq_Total = 20;

const double F_min1 = 10.0;
const double F_max1 = 40.0;

const int freq_dist_group = 1; //if group distribution of frequency is required

const int nFreq_input[] = {8,3,3,3,3};
#endif // INPUT_PARAMETERS

