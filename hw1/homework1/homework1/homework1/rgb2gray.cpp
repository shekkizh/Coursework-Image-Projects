#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define height 300	// height of the image
#define width 400	// width of the image
#define BytesPerPixel 3

using namespace std;

int main()
{
	unsigned char input[height][width][BytesPerPixel], output[height][width];
	FILE *file;
	const char input_file[] = "desk.raw";	//input file
	if (!(file = fopen(input_file,"rb")))
	{
		cout<<"file: "<<input_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);
	
	//rgb2gray
	int i,j;	//loop variables

		for (i = 0; i < height ; i++)
		{
			for (j = 0; j < width; j++)
			{
				output[i][j] = (unsigned char)(0.21*input[i][j][0] + 0.72*input[i][j][1] + 0.07*input[i][j][2]);
			//using luminosity method to convert rgb to gray valued image
				}	
			}

	const char output_file[] = "desk_gray.raw";	//output file
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