#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#define height 256	
#define width 256
#define laplace_mask -0.00387,-1,-0.00387,0.0154,3.9922,0.0154,-0.00387,-1,-0.00387
// window values obtained by multiplying the laplace mask with gaussian filter window
#define mask_size 3
#define threshold1 130	//lower theshold
#define threshold2 160	//upper threshold

using namespace std;
int laplace_operator(unsigned char window[mask_size*mask_size])
{
	//laplacian window edge processing
	int sum = 0;
	int weights[mask_size*mask_size]={laplace_mask};
	int i; //loop variable
	for (i=0; i<mask_size*mask_size; i++)
	{
		sum = sum + weights[i]*window[i];
	}
	int derivative_value = sum/1.016;
	return (derivative_value);
}

int main()
{
	unsigned char input[height][width], output[height][width]={0};
	FILE *file;
	const char input_file[] = "girl.raw";	//input file
	if (!(file = fopen(input_file,"rb")))
	{
		cout<<"file: "<<input_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input, sizeof(unsigned char), width*height, file);
	fclose(file);
	
	//laplacian edge detector
	const int extension = (mask_size-1)/2;
	unsigned char extended_input[height+2*extension][width+2*extension] ={128};
	int intermediate_output[height+2*extension][width+2*extension]={0};
	
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
			//trivalued map
			int magnitude = laplace_operator(window);
			if (magnitude < threshold1)
			intermediate_output[i][j] = -1;
			else if(magnitude>threshold2)
			intermediate_output[i][j] = 1;
			else
			intermediate_output[i][j] = 0;
		}
	}

	for(i=extension; i<height+extension;i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			if (intermediate_output[i][j] == 0)
			{
			for (k=(i-extension);k<=(i+extension);k++)
			{
				for (l=(j-extension);l<=(j+extension);l++)
				{	//zero crossing detection
					if ((intermediate_output[k][l] == -1) && (intermediate_output[(2*i-k)][(2*j-l)] == 1))
					{
						output[i-extension][j-extension] = 255;
						break;
					}
				}
			}
			
			}
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