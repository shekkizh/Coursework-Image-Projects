#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define height 300
#define width 300

using namespace std;

int main()
{
	unsigned char input[height][width][3], output[height][width][3];
	FILE *file;

	const char input_file[] = "piece.raw";
	if (!(file=fopen (input_file,"rb")))
	{
		cout<< "the input file does not exist"<<endl;
		system("pause");
	}
	fread(input, sizeof(unsigned char), height*width*3, file);
	fclose(file);

	int i,j;		//loop variables
	float i1,j1;	//reverse map variables
	int row_index,column_index;
	float u,v,x,y;	//real axis co-ordinates

	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			x = j+0.5;
			y = height-0.5-i;
			u = 0.9854*x - 0.17*y;
			v = 0.17*x + 0.9854*y;
			i1= height + 0.5 - v;
			j1= u + 0.5;
			row_index = (int) i1;
			column_index = (int) j1;
			if (row_index >=0 && column_index >=0)
			{
				output[i][j][0] = input[row_index][column_index][0]*(1-(i1-row_index))*(1-(j1-column_index))+
								  input[row_index+1][column_index][0]*((i1-row_index))*(1-(j1-column_index))+
								  input[row_index][column_index+1][0]*(1-(i1-row_index))*((j1-column_index))+
								  input[row_index][column_index][0]*((i1-row_index))*((j1-column_index));

				output[i][j][1] = input[row_index][column_index][1]*(1-(i1-row_index))*(1-(j1-column_index))+
								  input[row_index+1][column_index][1]*((i1-row_index))*(1-(j1-column_index))+
								  input[row_index][column_index+1][1]*(1-(i1-row_index))*((j1-column_index))+
								  input[row_index][column_index][1]*((i1-row_index))*((j1-column_index));

				output[i][j][2] = input[row_index][column_index][2]*(1-(i1-row_index))*(1-(j1-column_index))+
								  input[row_index+1][column_index][2]*((i1-row_index))*(1-(j1-column_index))+
								  input[row_index][column_index+1][2]*(1-(i1-row_index))*((j1-column_index))+
								  input[row_index][column_index][2]*((i1-row_index))*((j1-column_index));
			}
			else
			{
				output[i][j][0] = 255;
				output[i][j][1] = 255;
				output[i][j][2] = 255;
			}
		}
	}

	const char output_file[] = "piece_rotated.raw";
	file = fopen (output_file,"wb");
	fwrite(output, sizeof(unsigned char),3*height*width,file);
	fclose (file);

	system("pause");
}
