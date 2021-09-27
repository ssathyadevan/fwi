 __kernel void Helloo(int temp) {
     printf("Hellooo %d", temp);
 }
 
 __kernel void ProcessArray(__global int* data,__global int* outData){
     outData[get_global_id(0)] = data[get_global_id(0)]*2;
 }

__kernel void Process2DArray(__global int* data) {
    size_t id = get_global_id(1)*get_global_size(0) + get_global_id(0);
    data[id] = data[id]*2;
}

__kernel void NumericalReduction(__global int* data, __local int* localData, __global int* outData){
    
    size_t globalID = get_global_id(0);
    size_t localSize = get_local_size(0);
    size_t localID = get_local_id(0);

    localData[localID] = data[globalID];
    barrier(CLK_GLOBAL_MEM_FENCE);

    for (uint i = localSize>>1; i > 0 ; i >>= 1){
        if (localID < i )
            localData[localID] += localData[localID + i];
    barrier(CLK_LOCAL_MEM_FENCE);
    }

    if(localID == 0)
        outData[get_group_id(0)] = localData[0];
        
}

__kernel void vectorMultiplication(__global float* vec){
    size_t idx = get_global_id(0);
    vec[idx] = sin(vec[idx])*sin(vec[idx]);
}