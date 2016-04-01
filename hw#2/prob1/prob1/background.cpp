#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#define height 256
#define width 256
#define background_image "grain3.raw"
#define a -0.5
#define b 30

using namespace std;

int main()
{
	unsigned char input [height][width][3], output [height][width][3];
	unsigned char background [height][width][3];
	FILE *file;
	const char input_file[] = "sailboat_special.raw";
	if (!(file = fopen (input_file, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input, sizeof(unsigned char), height*width*3, file);
	fclose (file);

	const char background_file[] = background_image;
	if (!(file = fopen (background_file, "rb")))
	{
		cout<<"The background image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (background, sizeof(unsigned char), height*width*3, file);
	fclose (file);

	//adding background
	int i,j;	//loop varaibles
	int additive_background[3];
	for (i=0; i<height; i++)
	{
		for (j=0; j<width; j++)
		{
			additive_background[0]  = abs(input[i][j][0] + a*background[i][j][0]+b) ;
			additive_background[1]  = abs(input[i][j][1] + a*background[i][j][1]+b) ;
			additive_background[2]  = abs(input[i][j][2] + a*background[i][j][2]+b) ;
			if (additive_background[0]>255)
				output[i][j][0] = input[i][j][0]+a*background[i][j][0];
			else
				output[i][j][0] = additive_background[0];
			if (additive_background[1]>255)
				output[i][j][1] = input[i][j][1]+a*background[i][j][1];
			else
				output[i][j][1] = additive_background[1];
			if (additive_background[2]>255)
				output[i][j][2] = input[i][j][2]+a*background[i][j][2];
			else
				output[i][j][2] = additive_background[2];
			 
			
		}
	}

	const char output1_file[] = "sailboat_background.raw";
	if (!(file = fopen (output1_file, "wb")))
	{
		cout<<"The backgrounded output image file does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fwrite (output, sizeof(unsigned char), height*width*3, file);
	fclose (file);

	system("pause");
	return 0;
	}