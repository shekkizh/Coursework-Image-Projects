#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#define height 256
#define width 256
#define mask_size 17
#define weight 8

using namespace std;

int *k_means(double **data, int n, int m, int k, double t, double **centroids)
{
	/*** References
** - J. MacQueen, "Some methods for classification and analysis
**   of multivariate observations", Fifth Berkeley Symposium on
**   Math Statistics and Probability, 281-297, 1967.
** - I.S. Dhillon and D.S. Modha, "A data-clustering algorithm
**   on distributed memory multiprocessors",
**   Large-Scale Parallel Data Mining, 245-260, 1999.
** Notes
** - this function is provided as is with no warranty.
** - the author is not responsible for any damage caused
**   either directly or indirectly by using this function.
** - anybody is free to do whatever he/she wants with this
**   function as long as this header section is preserved.
** Created on 2005-04-12 by
** - Roger Zhang (rogerz@cs.dal.ca)
** Modifications
** -
** Last compiled under Linux with gcc-3
*/
   /* output cluster label for each data point */
   int *labels = (int*)calloc(n, sizeof(int));

   int h, i, j; /* loop counters, of course :) */
   int *counts = (int*)calloc(k, sizeof(int)); /* size of each cluster */
   double old_error, error = DBL_MAX; /* sum of squared euclidean distance */
   double **c = centroids ? centroids : (double**)calloc(k, sizeof(double*));
   double **c1 = (double**)calloc(k, sizeof(double*)); /* temp centroids */

   assert(data && k > 0 && k <= n && m > 0 && t >= 0); /* for debugging */

   /****
   ** initialization */

   for (h = i = 0; i < k; h += n / k, i++) {
      c1[i] = (double*)calloc(m, sizeof(double));
      if (!centroids) {
         c[i] = (double*)calloc(m, sizeof(double));
      }
      /* pick k points as initial centroids */
      for (j = m; j-- > 0; c[i][j] = data[h][j]);
   }

   /****
   ** main loop */

   do {
      /* save error from last step */
      old_error = error, error = 0;

      /* clear old counts and temp centroids */
      for (i = 0; i < k; counts[i++] = 0) {
         for (j = 0; j < m; c1[i][j++] = 0);
      }

      for (h = 0; h < n; h++) {
         /* identify the closest cluster */
         double min_distance = DBL_MAX;
         for (i = 0; i < k; i++) {
            double distance = 0;
            for (j = m; j-- > 0; distance += pow(data[h][j] - c[i][j], 2));
            if (distance < min_distance) {
               labels[h] = i;
               min_distance = distance;
            }
         }
         /* update size and temp centroid of the destination cluster */
         for (j = m; j-- > 0; c1[labels[h]][j] += data[h][j]);
         counts[labels[h]]++;
         /* update standard error */
         error += min_distance;
      }

      for (i = 0; i < k; i++) { /* update all centroids */
         for (j = 0; j < m; j++) {
            c[i][j] = counts[i] ? c1[i][j] / counts[i] : c1[i][j];
         }
      }

   } while (fabs(error - old_error) > t);

   /****
   ** housekeeping */

   for (i = 0; i < k; i++) {
      if (!centroids) {
         free(c[i]);
      }
      free(c1[i]);
   }

   if (!centroids) {
      free(c);
   }
   free(c1);

   free(counts);

   return labels;
}
int matrix_mult(int input1[5],int input2[5],int output[5][5])
{
	int i,j;	//loop variables
	int size = sizeof (int);
	for (i=0;i<5;i++)
	{
		for (j=0;j<5;j++)
		{
			output[i][j] = input1[i]*input2[j];
		}
	}
	return 0;
}

int convolve(unsigned char input[5][5],int filter[5][5])
{
	int i,j;
	int divisor = 0,sum = 0;
	int result;
	for (i=0;i<5;i++)
	{
		for (j=0;j<5;j++)
		{
			sum+=filter[i][j]*input[i][j];
			divisor+=filter[i][j];
		}
	}
	if (divisor==0)
		result =  sum;
	else
		result =  sum/divisor;
	return (result);
}

double energy_mask(int input[mask_size][mask_size])
{
	int i,j;
	double sum = 0;
	for (i=0;i<mask_size;i++)
	{
		for (j=0;j<mask_size;j++)
		{
			sum+=input[i][j]*input[i][j];
		}
	}
	return (sum/(mask_size*mask_size*mask_size*mask_size));
}

int main()
{
	const int extension = (mask_size-1)/2;
	unsigned char input[height][width], output[height][width];
	int filter_output[25][height][width];
	double energy[25][height][width];

	FILE *file;
	int filters[5][5] = {{1,4,6,4,1},{-1,-2,0,2,1},{-1,0,2,0,-1},{-1,2,0,-2,1},{1,-4,6,-4,1}};
	int filter_bank[25][5][5]; 

	const char input_file[] = "comb.raw";
	file = fopen (input_file, "rb");
	fread(input, sizeof(unsigned char), height*width, file);
	fclose(file);
	
	int i,j,k,l;	//loop variables

	for (k=0;k<5;k++)
	{
		for(l=0;l<5;l++)
		{
			matrix_mult(filters[k],filters[l],filter_bank[k*5+l]);
		}
	}


	unsigned char extended_input[height+4][width+4];
	for(i=0;i<height+4;i++)
	{
		for(j=2;j<width+2;j++)
		{
			if((i-2)<0)
			extended_input[i][j] = input[i+2][j-2];
			else if ((i-2)>(height-1))
			extended_input[i][j] = input[i-4][j-2];
			else
			extended_input[i][j] = input[i-2][j-2];
		}
	}

	for(i=0;i<height+4;i++)
	{
		for(j=0;j<width+4;j++)
		{
			if((j-2)<0)
			extended_input[i][j] = extended_input[i][j+2];
			else if ((j-2)>(width-1))
			extended_input[i][j] = extended_input[i][j-4];

		}
	}

	//filter bank feature extraction
	int index;
	unsigned char window[5][5];
	for (index=0;index<25;index++)
	{
		for(i=2;i<height+2;i++)
		{
			for(j=2;j<width+2;j++)
			{
				for(k=i-2;k<=i+2;k++)
				{
					for(l=j-2;l<=j+2;l++)
						window[k-i+2][l-j+2] = extended_input[k][l];
				}
				filter_output[index][i-2][j-2] = convolve(window, filter_bank[index]);
			}
		}
	}

	int energy_window[mask_size][mask_size];
	for (index=0;index<25;index++)
	{
		for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)
			{
				for(k=i-extension;k<=i+extension;k++)
				{
					for(l=j-extension;l<=j+extension;l++)
					{
						if(k>=0 && k<height && l>=0 && l<width)
						energy_window[k-i+extension][l-j+extension] = filter_output[index][k][l];
						else
						energy_window[k-i+extension][l-j+extension] = 0;
					}
				}
				energy[index][i][j] = energy_mask(energy_window);
			}
		}
	}

	for (index=1;index<25;index++)
	{
		for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)
			{
				energy[index][i][j] = energy[index][i][j]/energy[0][i][j];
			}
		}
	}

	double **data_points = new double *[height*width];
	int *cluster_output = new int[height*width];
	for (i=0; i<height*width;i++)
		data_points[i] = new double [26];

	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			for (index=0;index<24;index++)
				data_points[(i*width + j)][index] = energy[index+1][i][j];
			data_points [i*width + j][24] = weight*i;
			data_points [i*width + j][25] = weight*j;
		}
	}
	double **centroid = 0;
	cluster_output = k_means(data_points, height*width, 26, 5, 0.0001,centroid);
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			output[i][j] =cluster_output[(i*width+j)]*63;
		}
	}
	
	const char output_file[] = "output.raw";
	file = fopen(output_file,"wb");
	fwrite(output,sizeof(unsigned char),height*width,file);
	fclose(file);

	system("pause");
	return 0;
}