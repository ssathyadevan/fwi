__kernel void vectorMultiplication(__global float* vec){
    size_t idx = get_global_id(0);
    vec[idx] = sin(vec[idx])*sin(vec[idx]);
}

__kernel void calculateKappa(__global float* source_count, __global float* reciever_count, float* _kappa){
    float i = get_global_id(0);
    float j = get_global_id(1);
    int k = get_global_id(2);
    int li, lj;

    li = i * reciever_count * source_count; 
    lj = j * source_count;
    
    // _kappa[li + lj + k] =  _greens[i]->getRecieverCont(j) 
    _kappa[li + lj + k] = Greens_getRecieverCont(i,j);
}

float Greens_getRecieverCont(int i, int j){
    float out;
    return out;
}