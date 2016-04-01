#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 384
#define width 384
#define block_size 54
#define border 9
#define ip_height 192
#define ip_width 192
#define input_image "weave.raw"
#define output_image "weave_efros1.raw"

using namespace std;

int i_start, j_start;

int select_block1(unsigned char block1[block_size][block_size][3], unsigned char input[ip_height][ip_width][3])
{
	int i,j,k,l;	//loop variables
	int error[3];
	double squared_error=0,min_error;
	for (i= 0; i < (ip_height-block_size); i++)
	{
		for (j=0;j < (ip_width-block_size);j++)
		{
			squared_error=0;
			for (k=i;k<i+block_size;k++)
			{
				for(l=j;l<j+border;l++)
				{
					error[0]=block1[k-i][block_size-border+l-j][0] - input[k][l][0];
					error[1]=block1[k-i][block_size-border+l-j][1] - input[k][l][1];
					error[2]=block1[k-i][block_size-border+l-j][2] - input[k][l][2];
					squared_error += error[0]*error[0]+error[1]*error[1]+error[2]*error[2];
				}
			}
			if (i==0 && j==0)
			{
				min_error = squared_error;
				i_start = 0;
				j_start = 0;
			}
			else
			{
			if (squared_error < min_error)
			{
				min_error = squared_error;
				i_start = i;
				j_start = j;
			}
			}
		}
	}
	return 0;
}


int select_block2(unsigned char block1[block_size][block_size][3], unsigned char input[ip_height][ip_width][3])
{
	int i,j,k,l;	//loop variables
	int error[3];
	double squared_error=0,min_error;
	for (i= 0; i< (ip_height-block_size); i++)
	{
		for (j=0;j<(ip_width-block_size);j++)
		{
			squared_error = 0;
			for (k=i;k<i+border;k++)
			{
				for(l=j;l<j+block_size;l++)
				{
					error[0]=block1[block_size-border+k-i][l-j][0] - input[k][l][0];
					error[1]=block1[block_size-border+k-i][l-j][1] - input[k][l][1];
					error[2]=block1[block_size-border+k-i][l-j][2] - input[k][l][2];
					squared_error += error[0]*error[0]+error[1]*error[1]+error[2]*error[2];
				}
			}
			if (i==0 && j==0)
			{
				min_error = squared_error;
				i_start = 0;
				j_start = 0;
			}
			else
			{
			if (squared_error < min_error)
			{
				min_error = squared_error;
				i_start = i;
				j_start = j;
			}
			}
		}
	}
	return 0;
}


int select_block3(unsigned char block1[block_size][block_size][3], unsigned char block2[block_size][block_size][3], unsigned char input[ip_height][ip_width][3])
{
	int i,j,k,l;	//loop variables
	int error[3];
	double squared_error=0,min_error;
	for (i= 0; i< (ip_height-block_size); i++)
	{
		for (j=0;j<(ip_width-block_size);j++)
		{
			squared_error = 0;
			for (k=i;k<i+block_size;k++)
			{
				for(l=j;l<j+border;l++)
				{
					error[0]=block1[k-i][block_size-border+l-j][0] - input[k][l][0];
					error[1]=block1[k-i][block_size-border+l-j][1] - input[k][l][1];
					error[2]=block1[k-i][block_size-border+l-j][2] - input[k][l][2];
					squared_error += error[0]*error[0]+error[1]*error[1]+error[2]*error[2];
				}
			}

			for (k=i;k<i+border;k++)
			{
				for(l=j;l<j+block_size;l++)
				{
					error[0]=block2[block_size-border+k-i][l-j][0] - input[k][l][0];
					error[1]=block2[block_size-border+k-i][l-j][1] - input[k][l][1];
					error[2]=block2[block_size-border+k-i][l-j][2] - input[k][l][2];
					squared_error += error[0]*error[0]+error[1]*error[1]+error[2]*error[2];
				}
			}
			
			if (i==0 && j==0)
			{
				min_error = squared_error;
				i_start = 0;
				j_start = 0;
			}
			else
			{
			if (squared_error < min_error)
			{
				min_error = squared_error;
				i_start = i;
				j_start = j;
			}
			}
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

	int i,j,k,l,index;
	long error[3];
	long squared_error=0,min_error=0;
	int boundary_index[block_size];
	int boundary_index2[block_size];
	unsigned char block1[block_size][block_size][3],block2[block_size][block_size][3];
	i_start = (ip_height/3), j_start = (ip_width/3);
	
	for(i=0; i<height;i+=block_size-border)
	{
		for(j=0;j<width;j+=block_size-border)
		{
			if (i==0 && j==0)
			{
				for (k= i_start; k< i_start+block_size; k++)
				{
					for (l= j_start; l<j_start+block_size; l++)
					{
						output[k-i_start][l-j_start][0] = input[k][l][0];
						output[k-i_start][l-j_start][1] = input[k][l][1];
						output[k-i_start][l-j_start][2] = input[k][l][2];
					}
				}
			}
			else if(i==0 && j!=0)
			{
				for(k=i;k<i+block_size;k++)
				{
					for(l=(j-(block_size-border));l<j+border;l++)
					{
						block1[k-i][l-(j-block_size+border)][0] = output[k][l][0];
						block1[k-i][l-(j-block_size+border)][1] = output[k][l][1];
						block1[k-i][l-(j-block_size+border)][2] = output[k][l][2];
					}
				}
				select_block1(block1,input);
				for(k=i_start;k<i_start+block_size;k++)
				{
					for(l=j_start;l<j_start+block_size;l++)
					{						
						if ((i+k-i_start)<height && (j+l-j_start)<width)
						{
						output[i+k-i_start][j+l-j_start][0] = input[k][l][0];
						output[i+k-i_start][j+l-j_start][1] = input[k][l][1];
						output[i+k-i_start][j+l-j_start][2] = input[k][l][2];
						}
					}
				}
			}
			else if (i!=0 && j==0)
			{
				for(k=(i-block_size+border);k<i+border;k++)
				{
					for(l=j;l<j+block_size;l++)
					{
						block1[k-(i-block_size+border)][l-j][0] = output[k][l][0];
						block1[k-(i-block_size+border)][l-j][1] = output[k][l][1];
						block1[k-(i-block_size+border)][l-j][2] = output[k][l][2];
					}
				}
				select_block2(block1,input);
				for(k=i_start;k<i_start+block_size;k++)
				{
					for(l=j_start;l<j_start+block_size;l++)
					{
						if ((i+k-i_start)<height && (j+l-j_start)<width)
						{
						output[i+k-i_start][j+l-j_start][0] = input[k][l][0];
						output[i+k-i_start][j+l-j_start][1] = input[k][l][1];
						output[i+k-i_start][j+l-j_start][2] = input[k][l][2];
						}
					}
				}
			}
			else
			{
				for(k=i;k<i+block_size;k++)
				{
					for(l=(j-block_size+border);l<j+border;l++)
					{
						block1[k-i][l-(j-block_size+border)][0] = output[k][l][0];
						block1[k-i][l-(j-block_size+border)][1] = output[k][l][1];
						block1[k-i][l-(j-block_size+border)][2] = output[k][l][2];
					}
				}
				for(k=(i-block_size+border);k<i+border;k++)
				{
					for(l=j;l<j+block_size;l++)
					{
						block2[k-(i-block_size+border)][l-j][0] = output[k][l][0];
						block2[k-(i-block_size+border)][l-j][1] = output[k][l][1];
						block2[k-(i-block_size+border)][l-j][2] = output[k][l][2];
					}
				}
				select_block3(block1,block2,input);
				for(k=i_start;k<i_start+block_size;k++)
				{
					for(l=j_start;l<j_start+block_size;l++)
					{
						if ((i+k-i_start)<height && (j+l-j_start)<width)
						{
						output[i+k-i_start][j+l-j_start][0] = input[k][l][0];
						output[i+k-i_start][j+l-j_start][1] = input[k][l][1];
						output[i+k-i_start][j+l-j_start][2] = input[k][l][2];
						}
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