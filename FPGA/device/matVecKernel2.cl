/*
This kernel works based on the assumptions that the working threads are divided into blocks (1-d). Threads in each block are responsible for a particular row of the Greens matrix,
i.e., a single group is responsible for a particular row of the Greens matrix.
The block size is min(nx, block_size supported by the device). Each thread first executes the multiplications assigned to it and adds to lsum[lid]. Later a reduction operator collects all the data into lsum[0].
Also important to note is that Gxx represents ONLY the first row of Greens matrix (in a some what peculiar fashion) and contains (2nx - 1)nz elements (while the 1st row of the Greens matrix contains nx.nz elements, i.e., Gxx has some repeated elements). gid/nx represents the block layer of Greens matrix.

Greens matrix looks like following (for nx = nz = 4)

                                                          B0                B1              B2                B3
                                                    ------------------------------------------------------------------------
                                                    |A0  A1  A2  A3|  A4  A5  A6  A7|  A8  A9  A10 A11|  A12  A13  A14  A15|
                                                 B0 |A1  A0  A1  A2|  A5  A4  A5  A6|  A9  A8  A9  A10|  A13  A12  A13  A14|    gid/nx = 0
                                                    |A2  A1  A0  A1|  A6  A5  A4  A5|  A10 A9  A8  A9 |  A14  A13  A12  A11|
                                                    |A3  A2  A1  A0|  A7  A6  A5  A4|  A11 A10 A9  A8 |  A15  A14  A13  A12|
                                                    ------------------------------------------------------------------------
                    |B0  B1  B2  B3|                |              |  A0  A1  A2  A3|  A4  A5  A6  A7 |  A8   A9   A10  A11|
                    |B1  B0  B1  B2|                |      B1      |  A1  A0  A1  A2|  A5  A4  A5  A6 |  A9   A8   A9   A10|    gid/nx = 1
   Greens matrix =  |B2  B1  B0  B1|                |              |  A2  A1  A0  A1|  A6  A5  A4  A5 |  A10  A9   A8   A9 |
                    |B3  B2  B1  B0|                |              |  A3  A2  A1  A0|  A7  A6  A5  A4 |  A11  A10  A9   A8 |
                                                    ------------------------------------------------------------------------
                                                    |              |                |  A0  A1  A2  A3 |  A4   A5   A6   A7 |
                                                    |              |                |  A1  A0  A1  A2 |  A5   A4   A5   A6 |    gid/nx = 2
                                                    |      B2      |       B1       |  A2  A1  A0  A1 |  A6   A5   A4   A5 |
                                                    |              |                |  A3  A2  A1  A0 |  A7   A6   A5   A4 |
                                                    ------------------------------------------------------------------------
                                                    |              |                |                 |  A0   A1   A2   A3 |
                                                    |              |                |                 |  A1   A0   A1   A2 |    gid/nx = 3
                                                    |      B3      |       B2       |     B1          |  A2   A1   A0   A1 |
                                                    |              |                |                 |  A3   A2   A1   A0 |
                                                    ------------------------------------------------------------------------

                                                         i = 0           i = 1               i = 2            i = 3

Greens matrix is symmetric, hence the blank elements can be derived easily.

corresponding Gxx looks like following

Gxx = | A3 A2 A1 A0 A1 A2 A3 | A7 A6 A5 A4 A5 A6 A7 | A11 A10 A9 A8 A9 A10 A11 | A15 A14 A13 A12 A13 A14 A15 |
        C0                     C1                     C2                         C3
                 D11                    D21                      D31                         D41
              D12                    D22                      D32                        D42
           D13                    D23                     D33                        D43
        D14                    D24                    D34                        D44
*/

__attribute__((max_work_group_size(512)))
__kernel void matVecKernel2(__global const double2* restrict Gxx, __global const double2* restrict dW, __global double2* restrict prod, __local double2* restrict lsum, int nx, int nz)
{
    int gid = get_group_id(0); //indicates the row index of the Greens matrix
    int lid = get_local_id(0);

    lsum[lid].s0 = .0;
    lsum[lid].s1 = .0;

    int numEntriesPerThreadPerI = ( nx / get_local_size(0) ); // for cases where nx > get_local_size(0).

    for (int i = 0; i < nz; i++) //i is going block row wise and points to the block matrix being worked at.
    {
        int positionOfBlockInMatrix = i < gid / nx   ?   ( gid / nx - i ) * ( 2 * nx - 1 )   :   ( i - gid / nx ) * ( 2 * nx - 1 ); //positionOfBlockInMatrix corressponds to C0/C1/C2/C3 positions in Gxx
        int matCol = positionOfBlockInMatrix + ( nx - 1 - gid % nx ); //matCol corressponds to different D_gid_j position in Gxx (based on gid and positionOfBlockInMatrix)

        int dWIndex = i * nx + lid;
        int GxxIndex = matCol + lid;

        for (int j = 0; j < numEntriesPerThreadPerI; j++) // for cases where nx > get_local_size(0).
        {
            lsum[ lid ].s0 += Gxx[ GxxIndex ].s0 * dW[ dWIndex ].s0 - Gxx[ GxxIndex ].s1 * dW[ dWIndex ].s1;
            lsum[ lid ].s1 += Gxx[ GxxIndex ].s0 * dW[ dWIndex ].s1 + Gxx[ GxxIndex ].s1 * dW[ dWIndex ].s0;
            dWIndex += get_local_size(0);
            GxxIndex += get_local_size(0);
        }
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    //REDUCTION OPERATION//
    int reductionGroupSize = get_local_size(0);
    for (int i = reductionGroupSize; i > 1; i-=reductionGroupSize/2 )
    {
        reductionGroupSize = i;
        if (lid < reductionGroupSize/2)
        {
            lsum[ lid ] += lsum[ lid + reductionGroupSize/2 ];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if (lid == 0)
        prod[gid] = lsum[0];
}
