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

	//hist_eq_cdf
	int i,j;	//loop variables
	int gray_value; 
	int hist_array[256] = {0}, equalized_value [256] = {0};
	unsigned int cdf_value[256];

		for (i = 0; i < height ; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray_value = input[i][j];
				hist_array[gray_value]++;
			//obtaining histogram
			}	
		}


		//compute cdf
		cdf_value[0] = hist_array[0];
		for (i = 1; i < 256 ; i++)
		{
			cdf_value[i] = cdf_value[(i-1)] + hist_array[i] ;
		}


		// mapping input gray value to equalized value 
		float index = 0;	
		for (i = 0; i < 256; i++)
		{

			while (!(((height*width*(index)/256) - (float) cdf_value[i]) >= 0))
			{
				index++;
			}
			equalized_value[i] = (index-1);
		}

		
		//histogram equalizing the input image
		for (i = 0; i < height ; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray_value = input[i][j];
				output[i][j] = (unsigned char) equalized_value[gray_value];
			//replacing input value by eqalized value
			}	
		}

	// writing to output file
	const char output_file[] = "desk_enhanced3.raw";	//output file
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