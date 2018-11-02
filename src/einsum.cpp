#include "einsum.h"
#include "volComplexField_rect_2D_cpu.h"
#include "volField_rect_2D_cpu.h"
#include "greens_rect_2D_cpu.h"
#include "frequencies_alternate.h"
#include "frequencies_group.h"

//template class einsum< double, volComplexField_rect_2D_cpu, volField_rect_2D_cpu, Greens_rect_2D_cpu,Frequencies_alternate>;
template class einsum< double, volComplexField_rect_2D_cpu, volField_rect_2D_cpu, Greens_rect_2D_cpu,Frequencies_group>;

