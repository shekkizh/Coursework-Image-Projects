#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define height 500
#define width 400
#define input_image "obama.raw"

using namespace std;

float dot_product(float x[2], float y[2])
{
	float result = 0;
	result = x[0]*y[0] + x[1]*y[1];
	return (result);
}

int barycentric(float a[2], float b[2], float c[2], float p[2])
{
	float v0[2], v1[2], v2[2];	//vectors between the points
	v0[0] = c[0] - a[0];
	v0[1] = c[1] - a[1];
	v1[0] = b[0] - a[0];
	v1[1] = b[1] - a[1];
	v2[0] = p[0] - a[0];
	v2[1] = p[1] - a[1];

	float dot00, dot01, dot02, dot11, dot12;	//dot products
	dot00 = dot_product(v0, v0);
	dot01 = dot_product(v0, v1);
	dot02 = dot_product(v0, v2);
	dot11 = dot_product(v1, v1);
	dot12 = dot_product(v1, v2);

	float denominator = 1;
	denominator = 1/(dot00 * dot11 - dot01*dot01);
	float u,v;	//barycentric co-ordinates
	u = (dot11 * dot02 - dot01 * dot12)*denominator;
	v = (dot00 * dot12 - dot01 * dot02)*denominator;

	return (u>=0 && v>=0 && (u+v)<1);
}

int main()
{
	unsigned char input[height][width][3],output[height][width][3];
	FILE *file;

	const char input_file[] = input_image;
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
	float u,v,co_ordinates[2];	//real axis co-ordinates
	int triangle_case;

	float corner1[2] = {0.5,499.5};
	float corner2[2] = {399.5,499.5};
	float corner3[2] = {0.5,0.5};
	float corner4[2] = {399.5, 0.5};
	float feature[2] = {210.5, 286.5};

	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			co_ordinates[0] = j+0.5;
			co_ordinates[1] = height-0.5-i;
			if (barycentric(corner1, corner2, feature, co_ordinates))
				triangle_case = 1;
			else if (barycentric(corner1, corner3, feature, co_ordinates))
				triangle_case = 2;
			else if (barycentric(corner4, corner3, feature, co_ordinates))
				triangle_case = 3;
			else 
				triangle_case = 4;


			switch (triangle_case)		
			{
			case 1:
				u = 25.7958 + (1*co_ordinates[0]) - (0.0516*co_ordinates[1]);
				v = 0 + 28.1408 + (0*co_ordinates[0]) + (0.9437*co_ordinates[1]);
				break;
			case 2:
				u = 0 - 0.0262 + (1.0524*co_ordinates[0]) - (0*co_ordinates[1]);
				v = 0 - 0.0286 + (0.0571*co_ordinates[0]) + (1*co_ordinates[1]);
				break;
			case 3:
				u = 0 - 0.0192 + (1*co_ordinates[0]) + (0.0385*co_ordinates[1]);
				v = 0 - 0.021 + (0*co_ordinates[0]) + (1.042*co_ordinates[1]);
				break;
			case 4:
				u = 0 + 23.2513 + (0.9418*co_ordinates[0]) - (0*co_ordinates[1]);
				v = 0 + 25.3631 - (0.0635*co_ordinates[0]) + (1*co_ordinates[1]);
				break;
			}
			i1= height - 0.5 - v;
			j1= u - 0.5;
			row_index = (int) i1;
			column_index = (int) j1;
			if (row_index >=0 && row_index <height-1 && column_index >=0 && column_index <width-1)
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
				output[i][j][0] = 0;
				output[i][j][1] = 0;
				output[i][j][2] = 0;
			}
		}
	}

	const char output_file[] = "obama_wrap.raw";
	file = fopen (output_file,"wb");
	fwrite(output, sizeof(unsigned char),3*height*width,file);
	fclose (file);

	system("pause");
}
