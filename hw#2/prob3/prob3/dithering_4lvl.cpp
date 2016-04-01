#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#define height 512
#define width 512
#define mask_size 2
#define dithering_weights {0,2},{3,1}	//I2
//#define dithering_weights {0,8,2,10},{12,4,14,6},{3,11,1,9},{15,7,13,5}	//I4
//#define dithering_weights {0,32,8,40,2,34,10,42},{48,16,56,24,50,18,58,26},{12,44,4,36,14,46,6,38},{60,28,52,20,62,30,54,22},{3,35,11,43,1,33,9,41},{51,19,59,27,49,17,57,25},{15,47,7,39,13,45,5,37},{63,31,55,23,61,29,53,21}
							//I8
//#define dithering_weights {14,10,11,15},{9,3,0,4},{8,2,1,5},{13,7,6,12}	//A4
//#define dithering_weights {62,57,48,36,37,49,58,63},{56,47,35,21,22,38,50,59},{46,34,20,10,11,23,39,51},{33,19,9,3,0,4,12,24},{32,18,8,2,1,5,13,25},{45,31,17,7,6,14,26,40},{55,44,30,16,15,27,41,52},{61,54,43,29,28,42,53,60}
							//A8
using namespace std;

int main()
{
	unsigned char input[height][width];
	unsigned char output[height][width]={0};
	FILE *file;
	
	const char input_file[] = "man.raw"; 
	if(!(file = fopen(input_file,"rb")))
	{
		cout<<"input file missing"<<endl;
		system("pause");
		return 0;
	}
	fread(input, sizeof(unsigned char), height*width, file);
	fclose(file);

	int i,j,k,l,m;	//loop variables
	//calculation of threshold
	int weights[mask_size][mask_size] = {dithering_weights};
	float threshold_matrix[4][mask_size][mask_size];

	for (m=0; m<4; m++)
	{
	for(i=0; i < mask_size; i++)
	{
		for(j=0;j<mask_size; j++)
		{
			threshold_matrix[m][i][j] = ((weights[i][j]) + m*0.25)*255/(mask_size*mask_size);
		}
	}
	}

	//halftoning by dithering 
	for (i=0; i<height; i+=mask_size)
	{
		for (j=0; j<width; j+=mask_size)
		{
			for (k=0; k<mask_size; k++)
			{
				for(l=0; l<mask_size; l++)
				{
					for (m=0; m<3; m++)
					{
					if ((input[i+k][j+l]) < threshold_matrix[m][k][l])
					{output[i+k][j+l] = m*82; break;}
					else
						output[i+k][j+l] = 255;
					}
				}
			}
		}
	}

	//writing the output file
	const char output_file[] = "man_dithered4lvl.raw";
	if(!(file = fopen(output_file,"wb")))
	{
		cout<<"The output file does not exist"<<endl;
		system("pause");
		return 0;
	}
	fwrite(output, sizeof(unsigned char), height*width, file);
	fclose(file);

	system("pause");
	return 0;
}	