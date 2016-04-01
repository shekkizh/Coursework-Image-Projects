#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#define height 256	
#define width 256
#define x_magnitude -1,0,1,-2,0,2,-1,0,1	//sobel mask for X gradient
#define y_magnitude 1,2,1,0,0,0,-1,-2,-1	//sobel mask for Y gradient
#define mask_size 3

using namespace std;
struct gradient
{
	unsigned char magnitude;
	unsigned char x_value, y_value;
}; // structure holding the magnitude,x gradient and y gradient values

gradient sobel_operator(unsigned char window[mask_size*mask_size])
{
	int sum_x = 0,sum_y = 0;
	gradient gradient_value;
	int x_value,y_value;
	int weights[2][mask_size*mask_size]={x_magnitude, y_magnitude};
	int i; //loop variable
	for (i=0; i<mask_size*mask_size; i++)
	{
		sum_x = sum_x + weights[0][i]*window[i];
		sum_y = sum_y + weights[1][i]*window[i];
	}
	x_value = (sum_x/4); y_value = (sum_y/4);
	gradient_value.magnitude = (int)sqrt ((double)(x_value*x_value + (y_value*y_value)));
	gradient_value.x_value = x_value;
	gradient_value.y_value = y_value;
	return (gradient_value);
}

int main()
{
	unsigned char input[height][width], output[height][width],X_gradient[height][width],Y_gradient[height][width];
	FILE *file;
	const char input_file[] = "elaine_edge.raw";	//input file
	if (!(file = fopen(input_file,"rb")))
	{
		cout<<"file: "<<input_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input, sizeof(unsigned char), width*height, file);
	fclose(file);
	
	//edge thinning
	const int extension = (mask_size-1)/2;
	unsigned char extended_input[height+2*extension][width+2*extension] ={128};
	unsigned char intermediate_output[height+2*extension][width+2*extension] ={0} ;
	
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
			gradient gradient_data = sobel_operator(window);
			X_gradient[i-extension][j-extension] = gradient_data.x_value;
			Y_gradient[i-extension][j-extension] = gradient_data.y_value;
			
			intermediate_output[i][j] = gradient_data.magnitude;

		}
	}

	//thinning
	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{	//finding the non dominant direction
			if (X_gradient[i-extension][j-extension] > Y_gradient[i-extension][j-extension])
			{
				//checking whether the pixel is to be considered as edge
				if ((intermediate_output[i][j] > intermediate_output[i][j-1]) && (intermediate_output[i][j] > intermediate_output[i][j+1]))
					output[i-extension][j-extension] = input[i-extension][j-extension];
				else
					output[i-extension][j-extension] = 0;
			}
			else
			{
				if ((intermediate_output[i][j] > intermediate_output[i-1][j]) && (intermediate_output[i][j] > intermediate_output[i+1][j]))
					output[i-extension][j-extension] = input[i-extension][j-extension];
				else
					output[i-extension][j-extension] = 0;
			}
		}
	}	
	const char output_file[] = "elaine_thinned_edge.raw";	//output file
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