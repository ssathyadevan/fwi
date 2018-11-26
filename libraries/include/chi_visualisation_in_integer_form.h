#ifndef CHI_VISUALISATION_IN_INTEGER_FORM_H
#define CHI_VISUALISATION_IN_INTEGER_FORM_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

// this function visualises the chi field in an integer matrix form (where chi is normalised between values 2 and 7), where 2 represents background medium and 7 represents the subsurface.
// 1 and 8 are acceptable out of bounds, while 0 and 9 imply a relatively significant out of bound estimation of chi

void chi_visualisation_in_integer_form(std::string filename, int nxt);

#endif // CHI_VISUALISATION_IN_INTEGER_FORM_H
