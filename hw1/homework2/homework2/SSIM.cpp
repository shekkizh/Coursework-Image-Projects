#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#define height 512
#define width 512
#define mask_size 5	//window size over which similarity is found
#define object_focus_values 75,420,45,420		
//the center of focus is given as x co-ordinate1, x co-ordinate2,y co-ordinate1, y co-ordinate2
#define filtered_file "peppers_filtered2.raw"
//image file for which SSIM is to be calculated

using namespace std;
struct parameter
{
	float mean[2];
	float variance[2];
	float co_variance;
};	// a global structure that stores mean, variance, co-variance

float similarity_value (unsigned char window[2][mask_size*mask_size])
{
	float SSIM_value;
	parameter parameter_values ;
	int sum =0;
	int i,j;	//loop variable

	//calculation mean
	for (i=0;i<2;i++)
	{
		sum = 0;
	for (j=0;j<mask_size*mask_size;j++)
		sum += window[i][j];
	parameter_values.mean[i] = sum/(mask_size*mask_size);
	}

	//calculation variance
	float difference;
	for(i=0;i<2;i++)
	{
		sum=0;
	for (j=0;j<mask_size*mask_size;j++)
	{
		difference = (window[i][j] - parameter_values.mean[i]);
		sum += difference*difference;
	}
	parameter_values.variance[i] = sum/(mask_size*mask_size-1);
	}

	//calculation covariance
	float product;
	sum=0;
	for (j=0;j<mask_size*mask_size;j++)
	{
		product = ((window[0][j]- parameter_values.mean[0])*(window[1][j]-parameter_values.mean[1]));
		sum += product;
	}
	parameter_values.co_variance = sum/(mask_size*mask_size-1);
	
	//calculation SSIM
	float numerator, denominator;
	numerator = (2*parameter_values.mean[0]*parameter_values.mean[1] + 6.5025)*(2*parameter_values.co_variance + 58.5225);
	denominator = ((parameter_values.mean[0])*parameter_values.mean[0] + parameter_values.mean[1]*parameter_values.mean[1] + 6.5025)*(parameter_values.variance[0] + parameter_values.variance[1] + 58.5225);

	SSIM_value = numerator/denominator;
	return (SSIM_value);
}


int main()
{
	unsigned char input1[height][width], input2[height][width];
	unsigned char SSIM[height][width];
	float similarity, sum=0;
	FILE *file;
	// reading 1st file 
	const char input_file1[] = "peppers.raw";	//original image file
	if (!(file = fopen(input_file1,"rb")))
	{
		cout<<"file: "<<input_file1<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input1, sizeof(unsigned char), width*height, file);
	fclose(file);
	
	//reading 2nd file	
	const char input_file2[] = filtered_file;
	if(!(file = fopen(input_file2,"rb")))
	{
		cout<<"file: "<<input_file2<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input2, sizeof(unsigned char), width*height, file);
	fclose(file);

	//SSIM
	const int extension = (mask_size-1)/2;
	unsigned char extended_input[2][height+2*extension][width+2*extension] ={0};
	int i,j,k,l;	//loop variables
	int object_focus[4] ={object_focus_values};

	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			extended_input[0][i][j] = input1[i-extension][j-extension];
			extended_input[1][i][j] = input2[i-extension][j-extension];
		}
	}

	unsigned char window[2][mask_size*mask_size];

	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			int window_pixel = 0;
			for (k=(i-extension);k<=(i+extension);k++)
			{
				for (l=(j-extension);l<=(j+extension);l++)
				{
					window[0][window_pixel] = extended_input[0][k][l];
					window[1][window_pixel] = extended_input[1][k][l];
					window_pixel++;
				}
			}
			
			similarity =similarity_value(window);	//calculating similarity between the two images 
			
			if ((i>object_focus[0] & i <object_focus[1]) & (j>object_focus[2] & j<object_focus[3]))
				sum +=2*similarity;	//icreased weight for the center of focus 
			else
				sum+=similarity;

			SSIM [i-extension][j-extension] = (unsigned char)(int) 255*similarity;
		}
	}
	int size_object_focus = (object_focus[1] - object_focus[0])*(object_focus[3] - object_focus[2]); 
	cout<<(sum/((height*width)+size_object_focus))<<endl;

//writing SSIM to file	
	const char output_file[] = "SSIM.raw";	//SSIM map file
	if(!(file = fopen(output_file,"wb")))
	{
		cout<<"file: "<<output_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fwrite(SSIM, sizeof(unsigned char), width*height, file);
	fclose(file);
	

system ("pause");
return 0;
}