#ifndef INPUT_PARAMETERS
#define INPUT_PARAMETERS

const int calc_alpha=1; //put 1 if alpha in calcField.h is to be calculated.
const int n_max = 5;
const int n_iter1 = 30; //for chi update
const int n_iter2 = 25; //for p_tot update

const double tol1 = 1e-6; //CG tol for chi update
const double tol2 = 5e-5; //tol for p_tot update

const int do_reg = 1; //put 1 to turn on regularization
const int interactive = 1; //put 1 to turn on interactive python session


// weight of the regularization
const double delta_amplification_start = 100.0;
const double delta_amplification_slope = 10.0;

#endif // INPUT_PARAMETERS

