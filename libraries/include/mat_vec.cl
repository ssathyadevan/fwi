__kernel void mat_vec(__global double2* A, __global double2* vec, __global double2 *res, int nx, int ny, __local double2 *l_sum)
{

	int gid = get_group_id(0);
	int lid = get_local_id(0);
	
	int i_nz = gid/nx;

	double2 sum;
	sum.s0 = 0.0;
	sum.s1 = 0.0;
	
	int block_size = get_local_size(0);

	int l_ind = -1;
	int l1, l2, l3;
	int ntot = nx*ny;
	
	l1 = (gid-i_nz*nx)*ntot;
			
	for (int i=lid; i<ntot; i+=block_size)
	{
		if(i < i_nz*nx) //treating the lower triangular part//
		{
	//		printf("%d %d %d %d %d\n", gid, lid, i_nz*nx, block_size, get_global_size(0));
			for(int k=0; k<=i_nz; k++)
			{
				if(i/nx <= k)
				{
					l_ind = k;
					break;
				}
			}

			//l2 = (l_ind + 1) * nx;
			
			//if(gid==32)
			//	printf("%d %d %d %d\n", i, l_ind, l2, i_nz);		
					
			sum.s0 += A[ i_nz*nx - l_ind*nx + l1 + i - l_ind*nx ].s0 * vec[i].s0 - A[ i_nz*nx - l_ind*nx + l1 + i - l_ind*nx ].s1 * vec[i].s1; 
			sum.s1 += A[ i_nz*nx - l_ind*nx + l1 + i - l_ind*nx ].s0 * vec[i].s1 + A[ i_nz*nx - l_ind*nx + l1 + i - l_ind*nx ].s1 * vec[i].s0;
				
			//if(gid==64)
			//	printf("%d %d %d %d %d %f %d %f\n", i, i_nz*nx, l_ind*nx, l1, i_nz*nx - l_ind*nx + l1 + i, A[i_nz*nx - l_ind*nx + l1 + i].s0, i, vec[i].s0);

			
		}
		else
		{
			l2 = i_nz * nx;

			sum.s0 += A[ l1 + i - l2 ].s0 * vec[i].s0 - A[ l1 + i - l2 ].s1 * vec[i].s1;
			sum.s1 += A[ l1 + i - l2 ].s0 * vec[i].s1 + A[ l1 + i - l2 ].s1 * vec[i].s0;
		}
	}
	
	l_sum[lid] = sum;
	
	for(int i=block_size/2; i>=1; i/=2)
	{
		barrier(CLK_LOCAL_MEM_FENCE);
		if(lid < i)
			l_sum[lid] += l_sum[lid+i]; 
	}
	
	if (lid==0)
		res[gid] = l_sum[0];
}

