#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#define height 512
#define width 512
#define mask_size 3
// floyd steinberg
#define diffusion_weights1 {0,0,0},{0,0,7},{1,5,3}	//left to right
#define diffusion_weights2 {0,0,0},{7,0,0},{3,5,1}	//right to left	*/
/* Javis et.al.
#define diffusion_weights1 {0,0,0,0,0},{0,0,0,0,0},{0,0,0,7,5},{3,5,7,5,3},{1,3,5,3,1}	//left to right
#define diffusion_weights2 {0,0,0,0,0},{0,0,0,0,0},{5,7,0,0,0},{3,5,7,5,3},{1,3,5,3,1}	//right to left	*/
/* Stucki
#define diffusion_weights1 {0,0,0,0,0},{0,0,0,0,0},{0,0,0,8,4},{2,4,8,4,2},{1,2,4,2,1}	//left to right
#define diffusion_weights2 {0,0,0,0,0},{0,0,0,0,0},{4,8,0,0,0},{2,4,8,4,2},{1,2,4,2,1}	//right to left	*/
#define scaling_factor 16

using namespace std;

int main()
{
	unsigned char input[height][width];
	unsigned char output[height][width];
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

	//normalize to range [0,1]
	int i,j,k,l;	//loop variables
	const int extension = (mask_size-1)/2;
	float normalized_input[height+2*extension][width+2*extension]={0};

	for(i=extension; i < height+extension; i++)
	{
		for(j=extension; j<width+extension; j++)
		{
			normalized_input[i][j] = (float)(input[i-extension][j-extension])/255;
		}
	}

	//thresholding followed by diffusion
	int weights[2][mask_size][mask_size] = {{diffusion_weights1},{diffusion_weights2}};
	int divisor = scaling_factor; 
	int direction;
	for (i=extension; i < height+extension; i++)
	{
		for(j=extension; j < width+extension; j++)
		{
			int j_new;	//variable that decides in which direction we are moving
			switch(direction = i%2)
			{
			case 0: j_new = j;	break;
			case 1: j_new = (height + extension -j);
			}
			if(normalized_input[i][j_new] > 0.5)	//thresholding
			{
				float error = normalized_input[i][j_new] - 1;
				output[i-extension][j_new-extension] = 255;
			for (k=i;k<=(i+extension);k++)
			{
				for (l=(j_new-extension);l<=(j_new+extension);l++)
				{
					{
						normalized_input[k][l] += ((weights[direction][k-i+1][l-j_new+1]*error)/divisor);
					}
				}
			}
			}
			else
			{
				float error = normalized_input[i][j_new];
				output[i-extension][j_new-extension] = 0;
			for (k=i;k<=(i+extension);k++)
			{
				for (l=(j_new-extension);l<=(j_new+extension);l++)
				{
					{
						normalized_input[k][l] += ((weights[direction][k-i+1][l-j_new+1]*error)/divisor);
					}
				}
			}
			}
		}
	}

	//writing output
	const char output_file[] = "man_diffused.raw";
	if(!(file = fopen(output_file,"wb")))
	{
		cout<<"the output file does not exist"<<endl;
		system("pause");
		return 0;
	}
	fwrite(output, sizeof(unsigned char), height*width, file);
	fclose(file);

	system("pause");
	return 0;
}

	
	 
