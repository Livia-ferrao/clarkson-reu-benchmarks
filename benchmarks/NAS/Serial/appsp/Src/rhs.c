/*
|| NAS CFD code: APPSP
||
|| Fortran version:
||
|| Author: Sisira Weeratunga
||         NASA Ames Research Center
||         (10/25/90)
||
|| This is the C version of the Fortran code developed at NASA Ames Research
|| Center.  We converted the code to C as a part of our class project for 
|| CS 838-3/ChE 562  offered in Spring 1993 by Mark D. Hill, Sangtae Kim and
|| Mary Vernon at the University of Wisconsin at Madison.  CS 838-3/ChE 562 
|| was an experimental course that brought computer scientists and computation
|| scientists together to promote interdisciplinary research.
||
||					June 18, 1993.
||
||					Shubhendu S. Mukherjee (Shubu)
||					shubu@cs.wisc.edu
||					Computer Sciences Department
||
||					Iasonas Moustakis
||					iasonas@luther.che.wisc.edu
||					Chemical Engineering Department
||					
||					University of Wisconsin at Madison
||
*/

/* rhs.c */

#define MAIN extern
#include "common.h"

#define flux(dim1, dim2)	TWO_D_ARRAY(FLUX, dim1, dim2)

/* compute the right hand side based on exact solution */

rhs()
{
  double tmp;
  int i,j,k,m;
  double q, u21, u31, u41, u51, u21i, u31i, u41i, u51i;
  double u21im1, u31im1, u41im1, u51im1;
  double u21j, u31j, u41j, u51j;
  double u21jm1, u31jm1, u41jm1, u51jm1;
  double u21k, u31k, u41k, u51k;
  double u21km1, u31km1, u41km1, u51km1;
  double FLUX[TWO_D_SIZE];
  
  for (k=1;k<=nz;k++)
    {
      for (j=1;j<=ny;j++)
	{
	  for (i=1;i<=nx;i++)
	    {
	      for (m=1;m<=5;m++)
		{
                  rsd(m,i,j,k) = - frct(m,i,j,k);
		}
            }
	}
    }
  
  /* xi-direction flux differences */
  
  for (k=2;k<=nz-1;k++)
    {
      for (j=2;j<=ny-1;j++)
	{
	  for (i=1;i<=nx;i++)
	    {
	      flux(1,i) = u(2,i,j,k);
	      u21 = u(2,i,j,k) / u(1,i,j,k);
	      q = 0.50 * (  u(2,i,j,k) * u(2,i,j,k)
			  + u(3,i,j,k) * u(3,i,j,k)
			  + u(4,i,j,k) * u(4,i,j,k) )
		/ u(1,i,j,k);
	      flux(2,i) = u(2,i,j,k) * u21 + c2 * ( u(5,i,j,k) - q );
	      flux(3,i) = u(3,i,j,k) * u21;
	      flux(4,i) = u(4,i,j,k) * u21;
	      flux(5,i) = ( c1 * u(5,i,j,k) - c2 * q ) * u21;
            }
	  for (i=2;i<=nx-1;i++)
	    {
	      for (m=1;m<=5;m++)
		{
                  rsd(m,i,j,k) =  rsd(m,i,j,k)
		    - tx2 * ( flux(m,i+1) - flux(m,i-1) );
		}
            }
	  
	  for (i=2;i<=nx;i++)
	    {
	      tmp = 1.0 / u(1,i,j,k);
	      
	      u21i = tmp * u(2,i,j,k);
	      u31i = tmp * u(3,i,j,k);
	      u41i = tmp * u(4,i,j,k);
	      u51i = tmp * u(5,i,j,k);
	      
	      tmp = 1.0 / u(1,i-1,j,k);
	      
	      u21im1 = tmp * u(2,i-1,j,k);
	      u31im1 = tmp * u(3,i-1,j,k);
	      u41im1 = tmp * u(4,i-1,j,k);
	      u51im1 = tmp * u(5,i-1,j,k);
	      
	      flux(2,i) = (4.0/3.0) * tx3 * ( u21i - u21im1 );
	      flux(3,i) = tx3 * ( u31i - u31im1 );
	      flux(4,i) = tx3 * ( u41i - u41im1 );
	      flux(5,i) = 0.50 * ( 1.0 - c1*c5 )
		* tx3 * ( ( u21i*u21i + u31i*u31i + u41i*u41i )
			 - ( u21im1*u21im1 + u31im1*u31im1 + 
			    u41im1*u41im1 ) )
		  + (1.0/6.0)
		    * tx3 * ( u21i*u21i - u21im1*u21im1 )
		      + c1 * c5 * tx3 * ( u51i - u51im1 );
            }
	  
	  for (i=2;i<=nx-1;i++)
	    {
	      
	      rsd(1,i,j,k) = rsd(1,i,j,k)
		+ dx1 * tx1 * (            u(1,i-1,j,k)
			       - 2.0 * u(1,i,j,k)
			       +           u(1,i+1,j,k) );
	      
	      rsd(2,i,j,k) = rsd(2,i,j,k)
		+ tx3 * c3 * c4 * ( flux(2,i+1) - flux(2,i) )
		  + dx2 * tx1 * (            u(2,i-1,j,k)
				 - 2.0 * u(2,i,j,k)
				 +           u(2,i+1,j,k) );
	      
	      rsd(3,i,j,k) = rsd(3,i,j,k)
		+ tx3 * c3 * c4 * ( flux(3,i+1) - flux(3,i) )
		  + dx3 * tx1 * (            u(3,i-1,j,k)
				 - 2.0 * u(3,i,j,k)
				 +           u(3,i+1,j,k) );
	      rsd(4,i,j,k) = rsd(4,i,j,k)
		+ tx3 * c3 * c4 * ( flux(4,i+1) - flux(4,i) )
		  + dx4 * tx1 * (            u(4,i-1,j,k)
				 - 2.0 * u(4,i,j,k)
				 +           u(4,i+1,j,k) );
	      rsd(5,i,j,k) = rsd(5,i,j,k)
		+ tx3 * c3 * c4 * ( flux(5,i+1) - flux(5,i) )
		  + dx5 * tx1 * (            u(5,i-1,j,k)
				 - 2.0 * u(5,i,j,k)
				 +           u(5,i+1,j,k) );
            }
	  
	  /* Fourth-order dissipation */
	  
	  for (m=1;m<=5;m++)
	    {
	      rsd(m,2,j,k) = rsd(m,2,j,k)
                - dssp * (  5.0 * u(m,2,j,k)
			  - 4.0 * u(m,3,j,k)
			  +           u(m,4,j,k) );
	      rsd(m,3,j,k) = rsd(m,3,j,k)
                - dssp * ( -4.0 * u(m,2,j,k)
			  + 6.0 * u(m,3,j,k)
			  - 4.0 * u(m,4,j,k)
			  +           u(m,5,j,k) );
            }
	  for (i=4;i<=nx-3;i++)
	    {
	      for (m=1;m<=5;m++)
		{
                  rsd(m,i,j,k) = rsd(m,i,j,k)
		    - dssp * (            u(m,i-2,j,k)
			      - 4.0 * u(m,i-1,j,k)
			      + 6.0 * u(m,i,j,k)
			      - 4.0 * u(m,i+1,j,k)
			      +           u(m,i+2,j,k) );
		}
            }
	  for (m=1;m<=5;m++)
	    {
	      rsd(m,nx-2,j,k) = rsd(m,nx-2,j,k)
                - dssp * (             u(m,nx-4,j,k)
			  - 4.0 * u(m,nx-3,j,k)
			  + 6.0 * u(m,nx-2,j,k)
			  - 4.0 * u(m,nx-1,j,k)  );
	      rsd(m,nx-1,j,k) = rsd(m,nx-1,j,k)
                - dssp * (             u(m,nx-3,j,k)
			  - 4.0 * u(m,nx-2,j,k)
			  + 5.0 * u(m,nx-1,j,k) );
            }
	}
    }
  
  /* eta-direction flux differences */
  
  for (k=2;k<=nz-1;k++)
    {
      for (i=2;i<=nx-1;i++)
	{
	  for (j=1;j<=ny;j++)
	    {
	      flux(1,j) = u(3,i,j,k);
	      u31 = u(3,i,j,k) / u(1,i,j,k);
	      q = 0.50 * (  u(2,i,j,k) * u(2,i,j,k)
			  + u(3,i,j,k) * u(3,i,j,k)
			  + u(4,i,j,k) * u(4,i,j,k) )
		/ u(1,i,j,k);
	      flux(2,j) = u(2,i,j,k) * u31 ;
	      flux(3,j) = u(3,i,j,k) * u31 + c2 * ( u(5,i,j,k) - q );
	      flux(4,j) = u(4,i,j,k) * u31;
	      flux(5,j) = ( c1 * u(5,i,j,k) - c2 * q ) * u31;
            }
	  for (j=2;j<=ny-1;j++)
	    {
	      for (m=1;m<=5;m++)
		{
                  rsd(m,i,j,k) =  rsd(m,i,j,k)
		    - ty2 * ( flux(m,j+1) - flux(m,j-1) );
		}
            }
	  for (j=2;j<=ny;j++)
	    {
	      tmp = 1.0 / u(1,i,j,k);
	      u21j = tmp * u(2,i,j,k);
	      u31j = tmp * u(3,i,j,k);
	      u41j = tmp * u(4,i,j,k);
	      u51j = tmp * u(5,i,j,k);
	      tmp = 1.0 / u(1,i,j-1,k);
	      u21jm1 = tmp * u(2,i,j-1,k);
	      u31jm1 = tmp * u(3,i,j-1,k);
	      u41jm1 = tmp * u(4,i,j-1,k);
	      u51jm1 = tmp * u(5,i,j-1,k);
	      flux(2,j) = ty3 * ( u21j - u21jm1 );
	      flux(3,j) = (4.0/3.0) * ty3 * ( u31j - u31jm1 );
	      flux(4,j) = ty3 * ( u41j - u41jm1 );
	      flux(5,j) = 0.50 * ( 1.0 - c1*c5 )
		* ty3 * ( ( u21j*u21j + u31j*u31j + u41j*u41j )
			 - ( u21jm1*u21jm1 + u31jm1*u31jm1 + 
			    u41jm1*u41jm1 ) )
		  + (1.0/6.0)
		    * ty3 * ( u31j*u31j - u31jm1*u31jm1 )
		      + c1 * c5 * ty3 * ( u51j - u51jm1 );
            }
	  for (j=2;j<=ny-1;j++)
	    {
	      rsd(1,i,j,k) = rsd(1,i,j,k)
		+ dy1 * ty1 * (            u(1,i,j-1,k)
			       - 2.0 * u(1,i,j,k)
			       +           u(1,i,j+1,k) );
	      rsd(2,i,j,k) = rsd(2,i,j,k)
		+ ty3 * c3 * c4 * ( flux(2,j+1) - flux(2,j) )
		  + dy2 * ty1 * (            u(2,i,j-1,k)
				 - 2.0 * u(2,i,j,k)
				 +           u(2,i,j+1,k) );
	      rsd(3,i,j,k) = rsd(3,i,j,k)
		+ ty3 * c3 * c4 * ( flux(3,j+1) - flux(3,j) )
		  + dy3 * ty1 * (            u(3,i,j-1,k)
				 - 2.0 * u(3,i,j,k)
				 +           u(3,i,j+1,k) );
	      rsd(4,i,j,k) = rsd(4,i,j,k)
		+ ty3 * c3 * c4 * ( flux(4,j+1) - flux(4,j) )
		  + dy4 * ty1 * (            u(4,i,j-1,k)
				 - 2.0 * u(4,i,j,k)
				 +           u(4,i,j+1,k) );
	      rsd(5,i,j,k) = rsd(5,i,j,k)
		+ ty3 * c3 * c4 * ( flux(5,j+1) - flux(5,j) )
		  + dy5 * ty1 * (            u(5,i,j-1,k)
				 - 2.0 * u(5,i,j,k)
				 +           u(5,i,j+1,k) );
            }
	  /* fourth-order dissipation */
	  
	  for (m=1;m<=5;m++)
	    {
	      rsd(m,i,2,k) = rsd(m,i,2,k)
                - dssp * (  5.0 * u(m,i,2,k)
			  - 4.0 * u(m,i,3,k)
			  +           u(m,i,4,k) );
	      rsd(m,i,3,k) = rsd(m,i,3,k)
                - dssp * ( - 4.0 * u(m,i,2,k)
			  + 6.0 * u(m,i,3,k)
			  - 4.0 * u(m,i,4,k)
			  +           u(m,i,5,k) );
            }
	  for (j=4;j<=ny - 3; j++)
	    {
	      for (m=1;m<=5;m++)
		{
                  rsd(m,i,j,k) = rsd(m,i,j,k)
		    - dssp * (            u(m,i,j-2,k)
			      - 4.0 * u(m,i,j-1,k)
			      + 6.0 * u(m,i,j,k)
			      - 4.0 * u(m,i,j+1,k)
			      +           u(m,i,j+2,k) );
		}
            }
	  for (m=1;m<=5;m++)
	    {
	      rsd(m,i,ny-2,k) = rsd(m,i,ny-2,k)
                - dssp * (             u(m,i,ny-4,k)
			  - 4.0 * u(m,i,ny-3,k)
			  + 6.0 * u(m,i,ny-2,k)
			  - 4.0 * u(m,i,ny-1,k)  );
	      rsd(m,i,ny-1,k) = rsd(m,i,ny-1,k)
                - dssp * (             u(m,i,ny-3,k)
			  - 4.0 * u(m,i,ny-2,k)
			  + 5.0 * u(m,i,ny-1,k) );
            }
	}
    }
  /* zeta-direction flux differences */
  for (j=2;j<=ny-1;j++)
    {
      for (i=2;i<=nx-1;i++)
	{
	  for (k=1;k<=nz;k++)
	    {
	      flux(1,k) = u(4,i,j,k);
	      u41 = u(4,i,j,k) / u(1,i,j,k);
	      q = 0.50 * (  u(2,i,j,k) * u(2,i,j,k)
			  + u(3,i,j,k) * u(3,i,j,k)
			  + u(4,i,j,k) * u(4,i,j,k) )
		/ u(1,i,j,k);
	      flux(2,k) = u(2,i,j,k) * u41 ;
	      flux(3,k) = u(3,i,j,k) * u41 ;
	      flux(4,k) = u(4,i,j,k) * u41 + c2 * ( u(5,i,j,k) - q );
	      flux(5,k) = ( c1 * u(5,i,j,k) - c2 * q ) * u41;
            }
	  for (k=2;k<=nz-1;k++)
	    {
	      for (m=1;m<=5;m++)
		{
                  rsd(m,i,j,k) =  rsd(m,i,j,k)
		    - tz2 * ( flux(m,k+1) - flux(m,k-1) );
		}
            }
	  for (k=2;k<=nz;k++)
	    {
	      tmp = 1.0 / u(1,i,j,k);
	      u21k = tmp * u(2,i,j,k);
	      u31k = tmp * u(3,i,j,k);
	      u41k = tmp * u(4,i,j,k);
	      u51k = tmp * u(5,i,j,k);
	      tmp = 1.0 / u(1,i,j,k-1);
	      u21km1 = tmp * u(2,i,j,k-1);
	      u31km1 = tmp * u(3,i,j,k-1);
	      u41km1 = tmp * u(4,i,j,k-1);
	      u51km1 = tmp * u(5,i,j,k-1);
	      flux(2,k) = tz3 * ( u21k - u21km1 );
	      flux(3,k) = tz3 * ( u31k - u31km1 );
	      flux(4,k) = (4.0/3.0) * tz3 * ( u41k - u41km1 );
	      flux(5,k) = 0.50 * ( 1.0 - c1*c5 )
		* tz3 * ( ( u21k*u21k + u31k*u31k + u41k*u41k )
			 - ( u21km1*u21km1 + u31km1*u31km1 +
			    u41km1*u41km1 ) )
		  + (1.0/6.0)
		    * tz3 * ( u41k*u41k - u41km1*u41km1 )
		      + c1 * c5 * tz3 * ( u51k - u51km1 );
            }
	  for (k=2;k<=nz-1;k++)
	    {
	      rsd(1,i,j,k) = rsd(1,i,j,k)
		+ dz1 * tz1 * (            u(1,i,j,k-1)
			       - 2.0 * u(1,i,j,k)
			       +           u(1,i,j,k+1) );
	      rsd(2,i,j,k) = rsd(2,i,j,k)
		+ tz3 * c3 * c4 * ( flux(2,k+1) - flux(2,k) )
		  + dz2 * tz1 * (            u(2,i,j,k-1)
				 - 2.0 * u(2,i,j,k)
				 +           u(2,i,j,k+1) );
	      rsd(3,i,j,k) = rsd(3,i,j,k)
		+ tz3 * c3 * c4 * ( flux(3,k+1) - flux(3,k) )
		  + dz3 * tz1 * (            u(3,i,j,k-1)
				 - 2.0 * u(3,i,j,k)
				 +           u(3,i,j,k+1) );
	      rsd(4,i,j,k) = rsd(4,i,j,k)
		+ tz3 * c3 * c4 * ( flux(4,k+1) - flux(4,k) )
		  + dz4 * tz1 * (            u(4,i,j,k-1)
				 - 2.0 * u(4,i,j,k)
				 +           u(4,i,j,k+1) );
	      rsd(5,i,j,k) = rsd(5,i,j,k)
		+ tz3 * c3 * c4 * ( flux(5,k+1) - flux(5,k) )
		  + dz5 * tz1 * (            u(5,i,j,k-1)
				 - 2.0 * u(5,i,j,k)
				 +           u(5,i,j,k+1) );
            }
	  
	  /* fourth-order dissipation */
	  for (m=1;m<=5;m++)
	    {
	      rsd(m,i,j,2) = rsd(m,i,j,2)
                - dssp * (  5.0 * u(m,i,j,2)
			  - 4.0 * u(m,i,j,3)
			  +           u(m,i,j,4) );
	      rsd(m,i,j,3) = rsd(m,i,j,3)
                - dssp * ( - 4.0 * u(m,i,j,2)
			  + 6.0 * u(m,i,j,3)
			  - 4.0 * u(m,i,j,4)
			  +           u(m,i,j,5) );
            }
	  for (k=4; k<= nz - 3; k++)
	    {
	      for (m=1;m<=5;m++)
		{
                  rsd(m,i,j,k) = rsd(m,i,j,k)
		    - dssp * (            u(m,i,j,k-2)
			      - 4.0 * u(m,i,j,k-1)
			      + 6.0 * u(m,i,j,k)
			      - 4.0 * u(m,i,j,k+1)
			      +           u(m,i,j,k+2) );
		}
            }
	  for (m=1;m<=5;m++)
	    {
	      rsd(m,i,j,nz-2) = rsd(m,i,j,nz-2)
                - dssp * (             u(m,i,j,nz-4)
			  - 4.0 * u(m,i,j,nz-3)
			  + 6.0 * u(m,i,j,nz-2)
			  - 4.0 * u(m,i,j,nz-1)  );
	      rsd(m,i,j,nz-1) = rsd(m,i,j,nz-1)
                - dssp * (             u(m,i,j,nz-3)
			  - 4.0 * u(m,i,j,nz-2)
			  + 5.0 * u(m,i,j,nz-1) );
            }
	}
    }
  
}

