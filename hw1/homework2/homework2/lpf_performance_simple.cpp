#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 512
#define width 512
#define mask_size 5	//window size for the filter

using namespace std;
unsigned char mean(unsigned char window[mask_size*mask_size])
{
	//mean filter
	int sum = 0;
	unsigned char mean_value;
	int i; //loop variable
	for (i=0; i<mask_size*mask_size; i++)
		sum = sum + window[i];
	mean_value = (unsigned char) (sum/(mask_size*mask_size));
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
	
	//simple averaging filter
	const int extension = (mask_size-1)/2;
	unsigned char extended_input[height+2*extension][width+2*extension] ={0};
	
	int i,j,k,l;	//loop variables

	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			extended_input[i][j] = input[i-extension][j-extension];	//extending input for filtering at border
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
			
			output[i-extension][j-extension] = mean(window);	//mean filter
		}
	}
	
	const char output_file[] = "peppers_filtered2.raw";	//output file
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