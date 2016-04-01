#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#define height 256	
#define width 256
#define x_magnitude -1,0,1,-2,0,2,-1,0,1
#define y_magnitude 1,2,1,0,0,0,-1,-2,-1
#define mask_size 3
#define threshold 45

using namespace std;
unsigned char sobel_operator(unsigned char window[mask_size*mask_size])
{
	int sum_x = 0,sum_y = 0;
	int x_value, y_value;
	int weights[2][mask_size*mask_size]={x_magnitude, y_magnitude};
	int i; //loop variable
	for (i=0; i<mask_size*mask_size; i++)
	{
		sum_x = sum_x + weights[0][i]*window[i];
		sum_y = sum_y + weights[1][i]*window[i];
	}
	x_value = (sum_x/4); y_value = (sum_y/4);
	int magnitude = (int)sqrt ((double)(x_value*x_value + y_value*y_value));
	return (magnitude);
}

int main()
{
	unsigned char input[height][width], output[height][width];
	FILE *file;
	const char input_file[] = "girl.raw";
	if (!(file = fopen(input_file,"rb")))
	{
		cout<<"file: "<<input_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input, sizeof(unsigned char), width*height, file);
	fclose(file);
	
	//mixednoise denoising
	//filter1 averaging filter
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
			unsigned char magnitude = sobel_operator(window);
			if (magnitude < threshold)
			output[i-extension][j-extension] = 0;
			else
			output[i-extension][j-extension] = 255;

		}
	}
	
	const char output_file[] = "girl_edge - thresh 45.raw";
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