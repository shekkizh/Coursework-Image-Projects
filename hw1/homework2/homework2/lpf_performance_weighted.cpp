#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 512
#define width 512
#define weight_values 1,2,3,2,1,2,4,5,4,2,3,5,6,5,3,2,4,5,4,2,1,2,3,2,1
//#define weight_values 1,2,1,2,3,2,1,2,1
//weights defined for window to mean filter the input
#define mask_size 5	//window size

using namespace std;
unsigned char weighted_mean(unsigned char window[mask_size*mask_size])
{
	//weighted mean
	float sum = 0, average_divisor = 0;
	unsigned char mean_value;
	float weights[mask_size*mask_size]={weight_values};
	int i; //loop variable
	for (i=0; i<mask_size*mask_size; i++)
	{
		sum = sum + weights[i]*window[i];
		average_divisor += weights[i];
	}
	mean_value = (unsigned char)(int) (sum/(average_divisor));
	return (mean_value);
}

int main()
{
	unsigned char input[height][width], output[height][width];
	FILE *file;
	const char input_file[] = "peppers_mixed.raw";	//input file
	if (!(file = fopen(input_file,"rb")))
	{
		cout<<"file: "<<input_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input, sizeof(unsigned char), width*height, file);
	fclose(file);
	
	
	//weighted averaging filter
	const int extension = (mask_size-1)/2;
	unsigned char extended_input[height+2*extension][width+2*extension] ={0};
	
	int i,j,k,l;	//loop variables

	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			extended_input[i][j] = input[i-extension][j-extension];
		}
	}

	unsigned char window[mask_size*mask_size];

	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			int window_pixel = 0;
			for (k=(i-extension);k<=(i+extension);k++)
			{
				for (l=(j-extension);l<=(j+extension);l++)
				{
					window[window_pixel] = extended_input[k][l];
					window_pixel++;
				}
			}
			
			output[i-extension][j-extension] = weighted_mean(window);	//weighted_mean filter
		}
	}
	
	const char output_file[] = "peppers_filtered3.raw";	//output file
	if(!(file = fopen(output_file,"wb")))
	{
		cout<<"file: "<<output_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fwrite(output, sizeof(unsigned char), width*height, file);
	fclose(file);

system ("pause");
return 0;
}