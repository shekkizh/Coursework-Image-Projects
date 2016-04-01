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

	//full range linear scaling
	int i,j;	//loop variables
	int	Scaled_value; 

		for (i = 0; i < height ; i++)
		{
			for (j = 0; j < width; j++)
			{
				Scaled_value = (5.1*input[i][j]);
				if (Scaled_value > 255)
					output[i][j] = (unsigned char) 255;		//clipping
				else
					output[i][j] = (unsigned char) Scaled_value;
			}	
		}

	const char output_file[] = "desk_enhanced1.raw";	//output file
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