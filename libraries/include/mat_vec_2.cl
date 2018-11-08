__kernel void mat_vec(__global double2* A, __global double2* vec, __global double2 *res, int nx, int ny, int s_Gxx, int s_dW, int s_dot, __local double2 *l_sum, int i, int j)
{

	int gid = get_group_id(0);
	int lid = get_local_id(0);

	double2 sum;
	sum.s0 = 0.0;
	sum.s1 = 0.0;
	
	int block_size = get_local_size(0);

	int l1 = gid * nx * ny + s_Gxx;
	for (int i=lid; i<nx; i+=block_size)
	{
		sum.s0 += A[l1+i].s0 * vec[s_dW+i].s0 - A[l1+i].s1 * vec[s_dW+i].s1;
		sum.s1 += A[l1+i].s0 * vec[s_dW+i].s1 + A[l1+i].s1 * vec[s_dW+i].s0;
	}
	
	l_sum[lid] = sum;
	
	for(int i=block_size/2; i>=1; i/=2)
	{
		barrier(CLK_LOCAL_MEM_FENCE);
		if(lid < i)
			l_sum[lid] += l_sum[lid+i]; 
	}
	
	if (lid==0)
	{
		res[s_dot + gid] += l_sum[0];
		if( (2*i+j < ny) && (i>0) )	
			res[s_Gxx + s_dW + gid]	+= l_sum[0];
	}	
}


__kernel void vec_copy(__global double2* res, int c_from, int c_to, int nx)
{

	int block_size = get_local_size(0);
	int lid = get_local_id(0);
	
	for (int i=lid; i<nx; i+=block_size)
		res[c_to + i] = res[c_from + i];
}


__kernel void vec_clear(__global double2* res)
{
	int block_size = get_local_size(0);
	int tot_size = get_global_size(0);
	int lid = get_local_id(0);
	
	for (int i=lid; i<tot_size; i+=block_size)
	{
		res[i].s0 = 0.0;
		res[i].s1 = 0.0;
	}
}
