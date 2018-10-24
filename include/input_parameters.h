#ifndef INPUT_PARAMETERS
#define INPUT_PARAMETERS
//const int nxt = 32;
//const int nzt = 32;
//const int nSrct = 17;
//const int nFreq_Total = 20;
const int calc_alpha = 0;
const int n_max = 5;
const int n_iter1 = 50; //ATTENTION CHANGED FROM 50 TO 10 TO GO FASTER
const int n_iter2 = 100;
const double tol1 = 1e-8;
const double tol2 = 5e-05;
const int do_reg = 1;
//const int interactive = 0;
//const double F_min1 = 10.0;
//const double F_max1 = 40.0;
const int freq_dist_group = 1;
const int nFreq_input[] = {1};
//const int gpu = 0;
const double delta_amplification_start = 100.0;
const double delta_amplification_slope = 10.0;
#endif // INPUT_PARAMETERS
