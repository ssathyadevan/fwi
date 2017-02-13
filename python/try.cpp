#include <iostream>
#include <chrono>
#include "ProfileCpu.h" 
#include <complex>
#include <gsl/gsl_math.h>
#include "mpi.h"

int main(int argc, char* argv[])
{
	MPI_Init(&argc,&argv);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	int i,j, nz, nx, l1, l2, l3, l4;
	nz = 32;
	nx = 64;
	ProfileCpu prof;
	
	_Complex double *dW;
	_Complex double **matA;
	matA = new _Complex double*[nx];
	dW = new _Complex double[nz*nx];
	
	_Complex double *dummy = new _Complex double[nx];
	
	for (i=0;i<nx;i++)
		matA[i] = new _Complex double[nz*nx];
		
	for (i=0;i<nx;i++)
	{
		for (j=0;j<nz*nx;j++)
			matA[i][j] = 1;		
	}
	
	for (j=0;j<nz*nx;j++)
	{
		dW[j] = 1;		
	}
		
	
	
	_Complex double *p_prod = new _Complex double[nx*nz];
	for(int i=0; i < nx; i++) 
		p_prod[i] = 0;
		
	
	
	for(int i=0; i < nz; i++)
	{
	  l1 = i*nx;
	  for (int j=0; j < nz-i; j++)
	  {
	      l2 = j*nx;
	      l3 = l1 + l2;
	      
	      prof.StartRegion("try");

	      for(int k=0; k<nx; k++)
	      {
		  const _Complex double *p_Gxx = matA[k];
		  dummy[k] = 0;

		  for(int l=0; l<nx; l++)
		      dummy[k] += p_Gxx[ l1 + l ] * dW[ l3 + l ];

		  p_prod[ l2 + k ] += dummy[k];
	      }
	      prof.EndRegion();

	      if ( (2*i + j < nz) && (i>0) )
	      {
		  l4 = 2*l1 + l2;
		  for(int k=0; k<nx; k++)
		      p_prod[ l4 + k ] += dummy[k];
	      }
	  }
	}


	for(int i=1; i < nz; i++)
	{
	  if (i <= nz-i)
	  {
	      l1 = i*nx;
	      for (int j=0; j < i; j++)
	      {
		  l2 = j*nx;
		  l3 = l1 + l2;

		  for(int k=0; k<nx; k++)
		  {
		      const _Complex double *p_Gxx = matA[k];
		      for(int l=0; l<nx; l++)
		          p_prod[ l3 + k ] += p_Gxx[ l1 + l ] * dW[ l2 + l ];
		  }
	      }
	  }
	  else
	  {
	      l1 = i*nx;
	      for (int j=0; j < nz-i; j++)
	      {
		  l2 = j*nx;
		  l3 = l1 + l2;

		  for(int k=0; k<nx; k++)
		  {
		      const _Complex double *p_Gxx = matA[k];

		      for(int l=0; l<nx; l++)
		          p_prod[ l3 + k ] += p_Gxx[ l1 + l ] * dW[ l2 + l ];
		  }
	      }
	  }
	}

	prof.EndRegion();
	MPI_Finalize();
			
}

