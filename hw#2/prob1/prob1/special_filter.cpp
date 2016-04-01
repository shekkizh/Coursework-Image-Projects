//////////////////////////////////////////////////////////////////////////
// Name: Sarath Shekkizhar												//
// USC ID: 5063534629													//
// USC Email: shekkizh@usc.edu											//
// Documentation Date: 11/16/2012										//
//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 384
#define width 512
#define response_image1 "response3.raw"
#define response_image2 "response6.raw"
#define response_image3 "response5.raw"
#define response_image4 "response2.raw"

using namespace std;

int main()
{
	unsigned char input [height][width][3], output [height][width][3] = {0};
	unsigned char probe [256][256][3], response [4][256][256][3];
	FILE *file;
	const char input_file[] = "college.raw";
	if (!(file = fopen (input_file, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input, sizeof(unsigned char), height*width*3, file);
	fclose (file);

	const char probe_file[] = "probe.raw";
	if (!(file = fopen (probe_file, "rb")))
	{
		cout<<"The probe image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (probe, sizeof(unsigned char), height*width*3, file);
	fclose (file);

	const char response_file1[] = response_image1;
	if (!(file = fopen (response_file1, "rb")))
	{
		cout<<"The response image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (response[0], sizeof(unsigned char), height*width*3, file);
	fclose (file);

	const char response_file2[] = response_image2;
	if (!(file = fopen (response_file2, "rb")))
	{
		cout<<"The response image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (response[1], sizeof(unsigned char), height*width*3, file);
	fclose (file);

	const char response_file3[] = response_image3;
	if (!(file = fopen (response_file3, "rb")))
	{
		cout<<"The response image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (response[2], sizeof(unsigned char), height*width*3, file);
	fclose (file);

	const char response_file4[] = response_image4;
	if (!(file = fopen (response_file4, "rb")))
	{
		cout<<"The response image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (response[3], sizeof(unsigned char), height*width*3, file);
	fclose (file);

	//
	int red_map[4][256], green_map[4][256], blue_map[4][256];
	int i,j,k;	//loop variables
	//coarse mapping
	for (k=0; k<4; k++)
	{
	for (i=70; i<192; i+=16)
	{
		for (j=70; j<192; j+=16)
		{
			red_map[k][probe[i][j][0]] = response[k][i][j][0];
			green_map[k][probe[i][j][1]] = response[k][i][j][1];
			blue_map[k][probe[i][j][2]] = response[k][i][j][2];
		}
	}
	}

	/*const char coarse_file[] = "red_coarse_map.txt";
	if (!(file = fopen (coarse_file, "wb")))
	{
		cout<<"The output file does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fwrite (red_map, sizeof(int), 256, file);
	fclose (file);*/

	//fine mapping
	for (k=0; k<4; k++)
	{
	for (i=1; i<254; i+=4)
	{
		if (i==253)
		{
			//extrapolation
			red_map[k][i+1] = red_map[k][i]; red_map[k][i+2] = red_map[k][i];
			green_map[k][i+1] = green_map[k][i]; green_map[k][i+2] = green_map[k][i];
			blue_map[k][i+1] = blue_map[k][i]; blue_map[k][i+2] = blue_map[k][i];
			break;
		}

		//linear interpolation
		red_map[k][i+1] = red_map[k][i] + ((red_map[k][i+4] - red_map[k][i])/4);
		red_map[k][i+2] = red_map[k][i] + (2*(red_map[k][i+4] - red_map[k][i])/4);
		red_map[k][i+3] = red_map[k][i] + (3*(red_map[k][i+4] - red_map[k][i])/4);

		green_map[k][i+1] = green_map[k][i] + ((green_map[k][i+4] - green_map[k][i])/4);
		green_map[k][i+2] = green_map[k][i] + (2*(green_map[k][i+4] - green_map[k][i])/4);
		green_map[k][i+3] = green_map[k][i] + (3*(green_map[k][i+4] - green_map[k][i])/4);
	
		blue_map[k][i+1] = blue_map[k][i] + ((blue_map[k][i+4] - blue_map[k][i])/4);
		blue_map[k][i+2] = blue_map[k][i] + (2*(blue_map[k][i+4] - blue_map[k][i])/4);
		blue_map[k][i+3] = blue_map[k][i] + (3*(blue_map[k][i+4] - blue_map[k][i])/4);
	}
	//extrapolation
	red_map[k][0] = red_map[k][1]; green_map[k][0] = green_map[k][1]; blue_map[k][0] = blue_map[k][1];
	}
	//color transform
	int index[3];
	for (k=0; k<4;k++)
	{
	for (i=0; i<height; i++)
	{
		for (j=0; j<width/4; j++)
		{
			index[0] = input[i][(k*width/4)+j][0]; index[1] = input[i][(k*width/4)+j][1]; index[2] = input[i][(k*width/4)+j][2];
			
			output[i][(k*width/4)+j][0] = red_map[k][index[0]];
			output[i][(k*width/4)+j][1] = green_map[k][index[1]];
			output[i][(k*width/4)+j][2] = blue_map[k][index[2]];
		}
	}
	}

	const char output_file[] = "special_filter1.raw";
	if (!(file = fopen (output_file, "wb")))
	{
		cout<<"The output image file does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fwrite (output, sizeof(unsigned char), height*width*3, file);
	fclose (file);

	system("pause");
	return 0;
	}



