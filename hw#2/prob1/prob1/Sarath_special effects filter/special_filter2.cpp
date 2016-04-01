//////////////////////////////////////////////////////////////////////////
// Name: Sarath Shekkizhar												//
// USC ID: 5063534629													//
// USC Email: shekkizh@usc.edu											//
// Documentation Date: 11/16/2012										//
//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 450
#define width 600
#define response_image1 "response6.raw"
#define response_image2 "response4.raw"
#define response_image3 "response5.raw"
#define response_image4 "response.raw"
#define response_image5 "response2.raw"
#define response_image6 "response3.raw"
#define input_image	"Clipboard.raw"
#define output_image "special_filter1.raw"

using namespace std;

int main()
{
	unsigned char input [height][width][3], output [height][width][3] = {0};
	unsigned char probe [256][256][3], response [6][256][256][3];
	FILE *file;
	const char input_file[] = input_image;
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

	const char response_file5[] = response_image5;
	if (!(file = fopen (response_file5, "rb")))
	{
		cout<<"The response image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (response[4], sizeof(unsigned char), height*width*3, file);
	fclose (file);

	const char response_file6[] = response_image6;
	if (!(file = fopen (response_file6, "rb")))
	{
		cout<<"The response image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (response[5], sizeof(unsigned char), height*width*3, file);
	fclose (file);

	memcpy(output,input,height*width*3);
	int red_map[6][256], green_map[6][256], blue_map[6][256];
	int i,j,k;	//loop variables
	//coarse mapping
	for (k=0; k<6; k++)
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

	//fine mapping
	for (k=0; k<6; k++)
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
	int height1= height - (height%6);
	int width1 = width - (width%6);
	for (k=0; k<6;k++)
	{
	for (i=0; i<height; i++)
	{
		for (j=0; j<width1/6; j++)
		{
			index[0] = input[i][(k*width1/6)+j][0]; index[1] = input[i][(k*width1/6)+j][1]; index[2] = input[i][(k*width1/6)+j][2];
			
			output[i][(k*width1/6)+j][0] = red_map[k][index[0]];
			output[i][(k*width1/6)+j][1] = green_map[k][index[1]];
			output[i][(k*width1/6)+j][2] = blue_map[k][index[2]];
		}
	}
	}

	for (k=0; k<6;k++)
	{
	for (i=0; i<height1/6; i++)
	{
		for (j=0; j<width; j++)
		{
			index[0] = output[(k*height/6)+i][j][0]; index[1] = output[(k*height1/6)+i][j][1]; index[2] = output[(k*height1/6)+i][j][2];
			
			output[(k*height1/6)+i][j][0] = red_map[k][index[0]];
			output[(k*height1/6)+i][j][1] = green_map[k][index[1]];
			output[(k*height1/6)+i][j][2] = blue_map[k][index[2]];
		}
	}
	}

	const char output_file[] = output_image;
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



