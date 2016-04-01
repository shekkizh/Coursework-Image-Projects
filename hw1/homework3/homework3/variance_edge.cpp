#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#define height 256
#define width 256
#define mask_size 5		//window size	
#define threshold 120	//threshold to convert to binary image

using namespace std;

unsigned char variance_value (unsigned char window[mask_size*mask_size])
{
	float mean;
	int variance;
	int sum =0;
	int i,j;	//loop variable

	//calculation mean
	for (j=0;j<mask_size*mask_size;j++)
		sum += window[j];
	mean = sum/(mask_size*mask_size);
	
	//calculation variance
	float difference;
	sum=0;
	for (j=0;j<mask_size*mask_size;j++)
	{
		difference = (window[j] - mean);
		sum += difference*difference;
	}
	variance = sum/(mask_size*mask_size-1);
	return (variance);
}

int main()
{
	unsigned char input[height][width], output[height][width];
	FILE *file;
	const char input_file[] = "girl.raw";	//input file
	if (!(file = fopen(input_file,"rb")))
	{
		cout<<"file: "<<input_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input, sizeof(unsigned char), width*height, file);
	fclose(file);
	
	
	const int extension = (mask_size-1)/2;
	unsigned char extended_input[height+2*extension][width+2*extension] ={128};
	
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
			unsigned char variance = variance_value(window);
			if (variance < threshold)	//converting to binary image
			output[i-extension][j-extension] = 0;
			else
			output[i-extension][j-extension] = 255;

		}
	}
	
	const char output_file[] = "girl_edge.raw";	//output file
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
