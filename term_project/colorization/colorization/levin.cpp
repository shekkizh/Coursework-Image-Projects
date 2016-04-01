#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#define height 265
#define width 320
#define mask_size 3

using namespace std;
int convert_yuv(unsigned char rgb_input[height][width][3], double yuv_output[height][width][3])
{
	int i,j;	//loop variables
	double temp[3];

	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			temp[0] = rgb_input[i][j][0]/255;
			temp[1] = rgb_input[i][j][1]/255;
			temp[2] = rgb_input[i][j][2]/255;
			yuv_output[i][j][0] = 0.299*temp[0] + 0.587*temp[1] + 0.114*temp[2];
			yuv_output[i][j][1] = 0-0.14713*temp[0] - 0.28886*temp[1] + 0.436*temp[2];
			yuv_output[i][j][2] = 0.615*temp[0] - 0.51499*temp[1] - 0.10001*temp[2];
		}
	}
	return 0;
}

int convert_rgb(double yuv_input[height][width][3], unsigned char rgb_output[height][width][3])
{
	int i,j;	//loop variables
	double temp[3];

	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			temp[0] = yuv_input[i][j][0];
			temp[1] = yuv_input[i][j][1];
			temp[2] = yuv_input[i][j][2];
			rgb_output[i][j][0] = 255*(1*temp[0] + 0*temp[1] + 1.13983*temp[2]);
			rgb_output[i][j][1] = 255*(1*temp[0] - 0.39465*temp[1] - 0.58060*temp[2]);
			rgb_output[i][j][2] = 255*(1*temp[0] + 2.03211*temp[1] - 0*temp[2]);
		}
	}
	return 0;
}

double variance_calc (double window[mask_size][mask_size])
{
	double mean, result;
	double sum = 0;
	int i,j;	//loop varialbes

	for (i=0;i<mask_size;i++)
	{
		for (j=0;j<mask_size;j++)
			sum += window[i][j];
	}

	mean = sum/(mask_size*mask_size);
	sum = 0;
	for (i=0;i<mask_size;i++)
	{
		for (j=0;j<mask_size;j++)
			sum += (window[i][j]-mean)*(window[i][j]-mean);
	}
	result = sum/((mask_size*mask_size)*(mask_size*mask_size));
	return (result);
}

int weight_calculation(double window[mask_size][mask_size], double weight[mask_size][mask_size])
{
	int i,j;	//loop variables
	double variance;
	variance = variance_calc(window);
	
	for (i=0;i<mask_size;i++)
	{
		for (j=0;j<mask_size;j++)
			weight[i][j] = exp ((-0.5)*(window[1][1] - window[i][j])*(window[1][1] - window[i][j])/variance);
	}
	return 0;
}

double colorizing (double window[mask_size][mask_size], double weight[mask_size][mask_size])
{
	int i,j;	//loop variables
	double sum = 0, divisor = 0;
	double result;

	for (i=0;i<mask_size;i++)
	{
		for (j=0;j<mask_size;j++)
		{
			sum += weight[i][j]*window[i][j];
			divisor += weight[i][j];
		}
	}
	result = sum/divisor;
	return (result);
}

int main()
{
	unsigned char input1[height][width][3], input2[height][width][3], output[height][width][3];
	double input1_yuv[height][width][3], input2_yuv[height][width][3], output_yuv[height][width][3];
	double color_line[height][width];
	const int extension = (mask_size - 1)/2;
	FILE *file;

	const char input1_file[]="unmarked.raw";
	if (!(file=fopen(input1_file,"rb")))
	{
		cout<<"the input file 1 does not exist"<<endl;
		system ("pause");
		return 0;
	}
	fread(input1, sizeof (unsigned char),height*width*3,file);
	fclose(file);

	const char input2_file[]="marked.raw";
	if (!(file=fopen(input2_file,"rb")))
	{
		cout<<"the input file 2 does not exist"<<endl;
		system ("pause");
		return 0;
	}
	fread(input2, sizeof (unsigned char),height*width*3,file);
	fclose(file);

	int i,j,k,l, index;	//loop variables;
	int error;

	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			error = 0;
			error += abs(input1[i][j][0] - input2[i][j][0]);
			error += abs(input1[i][j][1] - input2[i][j][1]);
			error += abs(input1[i][j][2] - input2[i][j][2]);
			if (error != 0)
				color_line[i][j] = 1;
			else
				color_line[i][j] = 0;
		}
	}

	convert_yuv(input1, input1_yuv);
	convert_yuv(input2, input2_yuv);

	

	double input[height+2*extension][width+2*extension][3];		//extending input while combining Y and UV of inputs
	for (i=extension;i< (height+extension);i++)
	{
		for (j=extension;j< (width+extension);j++)
		{
			input[i][j][0] = input1[i-extension][j-extension][0];
			input[i][j][1] = input2[i-extension][j-extension][1];
			input[i][j][2] = input2[i-extension][j-extension][2];
		}
	}

	for (i=0;i<extension;i++)
	{
		for (j=extension;j< width + extension;j++)
		{
				input[i][j][0] = input1[2*extension-i-1][j-extension][0];
				input[i][j][1] = input2[2*extension-i-1][j-extension][1];
				input[i][j][2] = input2[2*extension-i-1][j-extension][2];
			
				input[height+i][j][0] = input1[height+i+1-2*extension][j-extension][0];
				input[height+i][j][1] = input2[height+i+1-2*extension][j-extension][1];
				input[height+i][j][2] = input2[height+i+1-2*extension][j-extension][2];
		}
	}
	for (i=0;i<height+2*extension;i++)
	{
		for (j=0;j<extension;j++)
		{
				input[i][j][0] = input[i][2*extension-j-1][0];
				input[i][j][1] = input[i][2*extension-j-1][1];
				input[i][j][2] = input[i][2*extension-j-1][2];
			
				input[i][width+j][0] = input[i][width+j+1-2*extension][0];
				input[i][width+j][1] = input[i][width+j+1-2*extension][1];
				input[i][width+j][2] = input[i][width+j+1-2*extension][2];
		}
	}
	double weights[mask_size][mask_size];
	double window[3][mask_size][mask_size];
	for (i=extension;i<height+extension;i++)
	{
		for (j=extension;j<width+extension;j++)
		{
			output_yuv[i-extension][j-extension][0] = input[i][j][0];
			for (k = i-extension; k<=i+extension; k++)
			{
				for (l = j-extension; l<=j+extension; l++)
				{
					window[0][k-i+extension][l-j+extension] = input[k][l][0];
					window[1][k-i+extension][l-j+extension] = input[k][l][1];
					window[2][k-i+extension][l-j+extension] = input[k][l][2];
				}
			}
			weight_calculation(window[0],weights);

			output_yuv[i-extension][j-extension][1] = colorizing(window[1],weights);
			output_yuv[i-extension][j-extension][2] = colorizing(window[2],weights);
		}
	}

	convert_rgb(output_yuv, output);
	const char output_file[] = "colored.raw";
	file = fopen(output_file, "wb");
	fwrite(output,sizeof(unsigned char), height*width*3,file);
	fclose(file);

	system("pause");
	return 0;
}