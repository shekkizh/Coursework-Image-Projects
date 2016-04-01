#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 250
#define width 250
#define input_image "GearTooth.raw"
#define output_image "Gear_skel1.raw"

using namespace std;


int skeleton (unsigned char input_seq, int bond)
{
	unsigned char skeleton_mask[40] = {0x12,0x0A,
									 0x48,0x50,0x29,0xE0,0x94,0x07,0xE8,0x69,0xF0,0xD4,
									 0x96,0x17,0x0F,0x2B,0xE9,0xF4,0x97,0x2F,0x6B,0xF8,
									 0xD6,0x1F,0xEB,0x6F,0xFC,0xF9,0xF6,0xD7,0x9F,0x3F,
									 0xEF,0xFD,0xF7,0xBF,0xFB,0xFE,0xDF,0x7F};
	int i;	//loop variable
	switch(bond)
	{
	case 4:
		for (i=0; i<8; i++)
		{
			if (input_seq^skeleton_mask[i])
				return 1;
		}
		break;

	case 6:
		for (i=8; i<16; i++)
		{
			if (!(input_seq^skeleton_mask[i]))
				return 1;
		}
		break;

	case 7:
		for (i=16; i<20; i++)
		{
			if (!(input_seq^skeleton_mask[i]))
				return 1;
		}
		break;

	case 8:
		for (i=20; i<24; i++)
		{
			if (!(input_seq^skeleton_mask[i]))
				return 1;
		}
		break;

	case 9:
		for (i=24; i<32; i++)
		{
			if (!(input_seq^skeleton_mask[i]))
				return 1;
		}
		break;


	case 10:
		for (i=32; i<36; i++)
		{
			if (!(input_seq^skeleton_mask[i]))
				return 1;
		}
		break;

	case 11:
		for (i=36; i<40; i++)
		{
			if (!(input_seq^skeleton_mask[i]))
				return 1;
		}
	default: break;
	}
return 0;
}

int main()
{
	unsigned char input [height][width], output [height][width]={0};
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

	memcpy(output, input,height*width);
	//skeletonizing
	int i,j,k,l;	//loop variables
	int iteration=1;
	while(iteration==1)
	{
		iteration=0;
	unsigned char intermediate_output[height+2][width+2] = {0};
	unsigned char extended_input[height+2][width+2] = {0};
	for (i = 1; i<height+1; i++)
	{
		for (j=1; j<width+1; j++)
		{
			extended_input[i][j] =  (output[i-1][j-1]/255);
		}
	}

	//calculation of bond and filter
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
						input_sequence = input_sequence<<1;
						input_sequence += extended_input[k][l];
						}
					}
				}
				if (skeleton (input_sequence,bond))
				{intermediate_output[i][j] = 1;
				}
			}
		}
	}

	//stage 2 filter
	unsigned char unconditional_masks [28] = {0x01,0x04,0x20,0x80, 0x02,0x08,0x10,0x40, 0x48,0x50,
										0x0A,0x12,
										0x4C,0xCC,0x4D,0xCD,0x51,0x71,0x55,0x75,0x32,0xB2,
										0x33,0xB3,0x8A,0xAA,0x8E,0xAE};
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
				while(m<28)
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
				else if (intermediate_output[i-1][j]&&intermediate_output[i][j-1] && intermediate_output[i][j+1])
					flag=1;
				
				else if (intermediate_output[i-1][j]&&intermediate_output[i][j-1] && intermediate_output[i+1][j])
					flag=1;
				
				else if (intermediate_output[i+1][j]&&intermediate_output[i][j-1] && intermediate_output[i][j+1])
					flag=1;
				
				else if (intermediate_output[i-1][j]&&intermediate_output[i+1][j] && intermediate_output[i][j+1])
					flag=1;
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

