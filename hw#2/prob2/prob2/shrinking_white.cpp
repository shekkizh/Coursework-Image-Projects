#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 400
#define width 550
#define input_image "PCB.raw"
#define output_image "PCB_shrink1.raw"

using namespace std;


int shrink (unsigned char input_seq, int bond)
{
	unsigned char shrink_mask[54] = {0x08,0x40,0x10,0x02,0x28,0x60,0xC0,0x90,0x14,0x06,
									 0x03,0x09,0x29,0xE0,0x94,0x07,0xC8,0x49,0x70,0x2A,
									 0x68,0xD0,0x16,0x0B,0xC9,0x74,0xE8,0x69,0xF0,0xD4,
									 0x96,0x17,0x0F,0x2B,0xE9,0xF4,0x97,0x2F,0x6B,0xF8,
									 0xD6,0x1F,0xEB,0x6F,0xFC,0xF9,0xF6,0xD7,0x9F,0x3F,
									 0xEF,0xFD,0xF7,0xBF};
	int i;	//loop variable
	switch(bond)
	{
	case 1:		return 1;

	case 2:
		for (i=0; i<4; i++)
		{
			if (input_seq==shrink_mask[i])
				return 1;
		}
		break;

	case 3:
		for (i=4; i<12; i++)
		{
			if (input_seq==shrink_mask[i])
				return 1;
		}
		break;

	case 4:
		for (i=12; i<16; i++)
		{
			if (input_seq==shrink_mask[i])
				return 1;
		}
		break;

	case 5:
		for (i=16; i<24; i++)
		{
			if (input_seq==shrink_mask[i])
				return 1;
		}
		break;

	case 6:
		for (i=24; i<34; i++)
		{
			if (input_seq==shrink_mask[i])
				return 1;
		}
		break;

	case 7:
		for (i=34; i<38; i++)
		{
			if (input_seq==shrink_mask[i])
				return 1;
		}
		break;

	case 8:
		for (i=38; i<42; i++)
		{
			if (input_seq==shrink_mask[i])
				return 1;
		}
		break;

	case 9:
		for (i=42; i<50; i++)
		{
			if (input_seq==shrink_mask[i])
				return 1;
		}
		break;


	case 10:
		for (i=50; i<54; i++)
		{
			if (input_seq==shrink_mask[i])
				return 1;
		}
		break;
	default: break;
	}
return 0;
}

int main()
{
	unsigned char input [height][width], output [height][width];
	FILE *file;
	const char input_file[] = input_image;
	if (!(file = fopen (input_file, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input, sizeof(unsigned char), height*width, file);
	fclose (file);

	//shrinking
	memcpy(output, input, height*width);
	int i,j,k,l;	//loop variables
	int iteration = 1;
	while(iteration==1)
	{
	unsigned char extended_input[height+2][width+2] = {0};
	unsigned char intermediate_output[height+2][width+2] = {0};
	
		iteration = 0;
	for (i = 1; i<height+1; i++)
	{
		for (j=1; j<width+1; j++)
		{
			extended_input[i][j] = (output[i-1][j-1])/255;
		}
	}

	//calculation of bond and stage1 filter
	unsigned char input_sequence;
	int bond;
	unsigned char bond_window[3][3] = {{1,2,1},{2,0,2},{1,2,1}};
	for (i=1; i<height+1;i++)
	{
		for (j=1; j<width+1; j++)
		{
			if (extended_input[i][j] == 1)
			{
				input_sequence = 0;
				bond = 0;
				for (k= i-1; k <= i+1 ; k++)
				{
					for (l=j-1; l<=j+1; l++)
					{
						if (!(k==i && l==j))
						{
						bond += extended_input[k][l]*bond_window[k-i+1][l-j+1];
						input_sequence = input_sequence*2;
						input_sequence += extended_input[k][l];
						}
					}
				}
				if (shrink (input_sequence,bond))
				{intermediate_output[i][j] = 1;//iteration=1;
				}
			}
		}
	}
	
	
	unsigned char unconditional_masks[72] ={0x20,0x80, 0x02,0x08, 0x28,0x60,0xC0,0x90,0x14,0x06,
										  0x03,0x09, 0x70,0xC8,0x49,0x2A, 0x64,0x2C,0x6C,0xC1,
										  0x91,0xD1,0x34,0x26,0x36,0x82,0x89,0x8B, 0x58,0xD8,
										  0x5C,0xDC,0x78,0x59,0x79,0x3A,0x1B,0x3B,
										  0x1A,0x9A,0x1E,0x9E,0xB2,0x72,0xF2,0x52,0x56,
										  0x53,0x57,0x4A,0x4E,0x4B,0x4F,0xCA,0x6A,0xEA,
										  0x4C,0xCC,0x4D,0xCD,0x51,0x71,0x55,0x75,0x32,0xB2,
										  0x33,0xB3,0x8A,0xAA,0x8E,0xAE};

	//stage2 filter
	for (i=1; i<height+1;i++)
	{
		for (j=1; j<width+1; j++)
		{
			if (intermediate_output[i][j] == 1) 
			{
				input_sequence = 0;
				for (k= i-1; k <= i+1 ; k++)
				{
					for (l=j-1; l<=j+1; l++)
					{
						if (!(k==i && l==j))
						{
						input_sequence = input_sequence*2;
						input_sequence += intermediate_output[k][l];
						}
					}
				}
				int m=0;
				int flag=0;
				while(m<72)
				{
				if(input_sequence==unconditional_masks[m])
				{flag=1;break;}
				m++;
				}
				if(flag==0)
				{
				if (intermediate_output[i-1][j-1] && intermediate_output[i][j-1] && intermediate_output[i-1][j])
						flag=1;
				else if(intermediate_output[i-1][j-1] && intermediate_output[i-1][j+1])
				{
					if(intermediate_output[i+1][j-1]||intermediate_output[i+1][j]||intermediate_output[i+1][j+1])
						flag=1;
				}
				else if(intermediate_output[i-1][j-1] && intermediate_output[i+1][j-1])
				{
					if(intermediate_output[i-1][j+1]||intermediate_output[i][j+1]||intermediate_output[i+1][j+1])
						flag=1;
				}
				else if(intermediate_output[i+1][j+1] && intermediate_output[i-1][j+1])
				{
					if(intermediate_output[i-1][j-1]||intermediate_output[i][j-1]||intermediate_output[i+1][j-1])
						flag=1;
				}
				else if(intermediate_output[i+1][j-1] && intermediate_output[i+1][j+1])
				{
					if(intermediate_output[i-1][j-1]||intermediate_output[i-1][j]||intermediate_output[i-1][j+1])
						flag=1;
				}
				}
				if(flag==0)
				{	output[i-1][j-1]=0;
					iteration =1;
				}
				else
					output[i-1][j-1]=255;
			}
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
	fwrite (output, sizeof(unsigned char), height*width, file);
	fclose (file);

	system("pause");
	return 0;
}

