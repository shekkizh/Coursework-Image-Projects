#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 256
#define width 256
#define response_image "response.raw"

using namespace std;

int main()
{
	unsigned char input [height][width][3], output [height][width][3];
	unsigned char probe [height][width][3], response [height][width][3];
	FILE *file;
	const char input_file[] = "sailboat.raw";
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

	const char response_file[] = response_image;
	if (!(file = fopen (response_file, "rb")))
	{
		cout<<"The response image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (response, sizeof(unsigned char), height*width*3, file);
	fclose (file);

	//
	unsigned char red_map[256]={0}, green_map[256]={0}, blue_map[256]={0};
	int i,j;	//loop variables
	//coarse mapping
	for (i=70; i<192; i+=16)
	{
		for (j=70; j<192; j+=16)
		{
			red_map[probe[i][j][0]] = response[i][j][0];
			green_map[probe[i][j][1]] = response[i][j][1];
			blue_map[probe[i][j][2]] = response[i][j][2];
		}
	}

	const char red_coarse[]="red_coarse_map.txt"; 
	file=fopen(red_coarse,"wb");
	for(i=0;i<256;i++)
	fprintf(file,"%d \n",red_map[i]);

	const char green_coarse[]="green_coarse_map.txt"; 
	file=fopen(green_coarse,"wb");
	for(i=0;i<256;i++)
	fprintf(file,"%d \n",green_map[i]);
	
	const char blue_coarse[]="blue_coarse_map.txt"; 
	file=fopen(blue_coarse,"wb");
	for(i=0;i<256;i++)
	fprintf(file,"%d \n",blue_map[i]);

	//fine mapping
	
	for (i=1; i<254; i+=4)
	{
		if (i==253)
		{
			//extrapolation
			red_map[i+1] = red_map[i]; red_map[i+2] = red_map[i];
			green_map[i+1] = green_map[i]; green_map[i+2] = green_map[i];
			blue_map[i+1] = blue_map[i]; blue_map[i+2] = blue_map[i];
			break;
		}

		//linear interpolation
		red_map[i+1] = red_map[i] + ((red_map[i+4] - red_map[i])/4);
		red_map[i+2] = red_map[i] + (2*(red_map[i+4] - red_map[i])/4);
		red_map[i+3] = red_map[i] + (3*(red_map[i+4] - red_map[i])/4);

		green_map[i+1] = green_map[i] + ((green_map[i+4] - green_map[i])/4);
		green_map[i+2] = green_map[i] + (2*(green_map[i+4] - green_map[i])/4);
		green_map[i+3] = green_map[i] + (3*(green_map[i+4] - green_map[i])/4);
	
		blue_map[i+1] = blue_map[i] + ((blue_map[i+4] - blue_map[i])/4);
		blue_map[i+2] = blue_map[i] + (2*(blue_map[i+4] - blue_map[i])/4);
		blue_map[i+3] = blue_map[i] + (3*(blue_map[i+4] - blue_map[i])/4);
	}
	//extrapolation
	red_map[0] = red_map[1]; green_map[0] = green_map[1]; blue_map[0] = blue_map[1];

	
	const char red_fine[]="red_fine_map.txt"; 
	file=fopen(red_fine,"wb");
	for(i=0;i<256;i++)
	fprintf(file,"%d \n",red_map[i]);

	const char green_fine[]="green_fine_map.txt"; 
	file=fopen(green_fine,"wb");
	for(i=0;i<256;i++)
	fprintf(file,"%d \n",green_map[i]);
	
	const char blue_fine[]="blue_fine_map.txt"; 
	file=fopen(blue_fine,"wb");
	for(i=0;i<256;i++)
	fprintf(file,"%d \n",blue_map[i]);

	//color transform
	int index[3];
	for (i=0; i<height; i++)
	{
		for (j=0; j<width; j++)
		{
			index[0] = input[i][j][0]; index[1] = input[i][j][1]; index[2] = input[i][j][2];
			output[i][j][0] = red_map[index[0]];
			output[i][j][1] = green_map[index[1]];
			output[i][j][2] = blue_map[index[2]];
		}
	}

	const char output_file[] = "sailboat_transform.raw";
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



