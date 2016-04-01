#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 250
#define width 250

using namespace std;

int main()
{
	unsigned char input [height][width], output[height][width]={0};
	FILE *file;
	const char input_file[] = "Gear_hole.raw";
	if (!(file = fopen (input_file, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input, sizeof(unsigned char), height*width, file);
	fclose (file);
	int i,j;	//loop variables
	int k,l;	// distance variables
	int i_center =123,j_center=125;
	int radius = 0,iteration=1;
	while(iteration==1)
	{
		iteration =30;
		radius++;
	for(i=0;i<height; i++)
	{
		for(j=0;j<width; j++)
		{
			if (input[i][j]==255)
			{	
			k = i-i_center; l= j-j_center;
			if ((k*k + l*l) == (radius*radius))
			{
				iteration=1;
			}
			}
		}
	}
	}
	cout<<"outer radius: "<<radius<<endl;

	for(i=0;i<height; i++)
	{
		for(j=0;j<width; j++)
		{
			k = i-i_center; l= j-j_center;
			if(((k*k + l*l) <(radius+2)*(radius+2)) && (k*k + l*l) > (radius-2)*(radius-2)) 
			{
				if (input[i][j] == 255)
				output[i][j] = 255;
			}
		}
	}

	const char output_file[] = "Gear_position.raw";
	file = fopen(output_file,"wb");
	fwrite(output,sizeof(unsigned char), height*width, file);
	fclose(file);

	system ("pause");
	return 0;
}