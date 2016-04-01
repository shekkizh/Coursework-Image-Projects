#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define height 300 
#define width 400

using namespace std;

int main()
{
	unsigned char input[height][width], output[height][width];
	FILE *file;
	const char input_file[] = "desk_gray.raw";	//input file
	if (!(file = fopen(input_file,"rb")))
	{
		cout<<"file: "<<input_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input, sizeof(unsigned char), width*height, file);
	fclose(file);

	//enhancement by histogram specification
	int i,j;	//loop variables
	int gray_value; 
	int hist_array[256] = {0}, specified_hist[256] = {0};
	int	equalized_value [256] = {0};
	float cdf_value[256], specified_cdf[256];

	//specified histogram
	for (i = 0; i <160  ; i++)
		specified_hist[i] = height*width/200;
	for (i = 200; i < 240 ; i++)
		specified_hist[i] = height*width/200;
	
	//histogram of given image
	for (i = 0; i < height ; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray_value = input[i][j];
				hist_array[gray_value]++;
			//obtaining histogram of given image
			}	
		}


		//compute cdf
		cdf_value[0] = hist_array[0];
		specified_cdf[0] = specified_hist[0];
		for (i = 1; i < 256 ; i++)
		{
			cdf_value[i] = cdf_value[(i-1)] + hist_array[i] ;
			specified_cdf[i] = specified_cdf[(i-1)] + specified_hist[i];
		}


		// mapping input gray value to equalized value 
		int index = 0;	
		for (i = 0; i < 256; i++)
		{

			while (!((specified_cdf[index] - cdf_value[i])>=0))
			{
				index++;
			}
			equalized_value[i] = (index-1);
		}

		
		//histogram equalizing the input image with the given histogram
		for (i = 0; i < height ; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray_value = input[i][j];
				output[i][j] = (unsigned char) equalized_value[gray_value];
			//replacing input value by equalized value
			}	
		}

	// writing to output file
	const char output_file[] = "desk_enhanced5.raw";	//output file
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