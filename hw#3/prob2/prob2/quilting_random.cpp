#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 248
#define width 370
#define block_size 64
#define ip_height 124
#define ip_width 185
#define input_image "apples.raw"
#define output_image "apples_syn1.raw"

using namespace std;

int select_block(unsigned char block[block_size][block_size][3], unsigned char input[ip_height][ip_width][3],int block_no)
{
	int i,j;	//loop variables
	int i_start,j_start;
	block_no--;
	i_start = (block_no%(ip_height-block_size));	//simple calculation to choose a block
	j_start = (block_no%(ip_width-block_size));
	for (i= i_start; i< i_start+block_size; i++)
	{
		for (j=j_start;j<j_start+block_size;j++)
		{
			block[i-i_start][j-j_start][0] = input[i][j][0];
			block[i-i_start][j-j_start][1] = input[i][j][1];
			block[i-i_start][j-j_start][2] = input[i][j][2];
		}
	}
	return 0;
}

int main()
{
	unsigned char input[ip_height][ip_width][3], output[height][width][3];
	FILE *file;
	const char input_file[] = input_image;
	if (!(file = fopen (input_file,"rb")))
	{
		cout<<"the input file is missing"<<endl;
		system ("pause");
		return 0;
	}
	fread(input, sizeof(unsigned char), 3*ip_height*ip_width,file);
	fclose(file);

	int i,j,k,l;
	unsigned char block[block_size][block_size][3];
	int block_no = 1000;	//some random number to decide the block
	for(i=0; i<height;i+=block_size)
	{
		for(j=0;j<width;j+=block_size)
		{
			select_block(block, input, block_no);
			block_no+=170;		//random number
			for(k=i;k<i+block_size;k++)
			{
				for(l=j;l<j+block_size;l++)
				{
					if (k<height && l<width)
					{
						output[k][l][0] = block[k-i][l-j][0];
						output[k][l][1] = block[k-i][l-j][1];
						output[k][l][2] = block[k-i][l-j][2];
					}
				}
			}
		}
	}

	const char output_file[] = output_image;
	file = fopen (output_file, "wb");
	fwrite(output, sizeof(unsigned char), 3*height*width, file);
	fclose(file);

	system("pause");
	return 0;
}