#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#define height 256
#define width 256
#define border_image "border.raw"

using namespace std;

int main()
{
	unsigned char input [height][width][3], output [height][width][3];
	unsigned char border [height][width][3];
	FILE *file;
	const char input_file[] = "sailboat_background.raw";
	if (!(file = fopen (input_file, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input, sizeof(unsigned char), height*width*3, file);
	fclose (file);

	const char border_file[] = border_image;
	if (!(file = fopen (border_file, "rb")))
	{
		cout<<"The border image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (border, sizeof(unsigned char), height*width*3, file);
	fclose (file);

	int i,j;	//loop varaibles
	memcpy(output,input,height*width*3);
	//adding border
	for (i=0; i<height; i++)
	{
		for (j=0; j<10; j++)
		{
			output[i][j][0] = border[i][j][0];
			output[i][width-j][0] = border[i][width-j][0];
			output[i][j][1] = border[i][j][1];
			output[i][width-j][1] = border[i][width-j][1];
			output[i][j][2] = border[i][j][2];
			output[i][width-j][2] = border[i][width-j][2];
		}
	}
	for (i=0; i<8; i++)
	{
		for (j=0; j<width; j++)
		{
			output[i][j][0] = border[i][j][0];
			output[height-i][j][0] = border[height-i][j][0];
			output[i][j][1] = border[i][j][1];
			output[height-i][j][1] = border[height-i][j][1];
			output[i][j][2] = border[i][j][2];
			output[height-i][j][2] = border[height-i][j][2];
		}
	}

	const char output2_file[] = "sailboat_bordered.raw";
	if (!(file = fopen (output2_file, "wb")))
	{
		cout<<"The bordered output image file does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fwrite (output, sizeof(unsigned char), height*width*3, file);
	fclose (file);

	system("pause");
	return 0;
	}