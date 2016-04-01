#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define height 1044
#define width 642
#define input_image "sarath.raw"

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

	float corner1[2] = {0.5,1043.5};
	float corner2[2] = {641.5,1043.5};
	float corner3[2] = {0.5,0.5};
	float corner4[2] = {641.5, 0.5};
	float feature1[2] = {204.5, 659.5};
	float feature2[2] = {397.5, 664.5};
	float feature3[2] = {269.5, 585.5};
	float feature4[2] = {204.5, 483.5};
	float feature5[2] = {335.5, 483.5};
	float feature6[2] = {267.5, 344.5};
	float feature7[2] = {142.5, 0.5};
	float feature[2] = {304.5, 264.5};

	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			co_ordinates[0] = j+0.5;
			co_ordinates[1] = height-0.5-i;
			if (barycentric(feature1, feature2, feature3, co_ordinates))
				triangle_case = 1;
			else if (barycentric(feature1, feature3, feature4, co_ordinates))
				triangle_case = 2;
			else if (barycentric(feature4, feature3, feature5, co_ordinates))
				triangle_case = 3;
			else if (barycentric(feature2, feature3, feature5, co_ordinates))
				triangle_case = 4;
			else if (barycentric(feature1, feature2, corner2, co_ordinates))
				triangle_case = 5;
			else if (barycentric(corner1, corner2, feature1, co_ordinates))
				triangle_case = 6;
			else if (barycentric(feature4, feature1, corner1, co_ordinates))
				triangle_case = 7;
			else if (barycentric(corner1, corner3, feature4, co_ordinates))
				triangle_case = 8;
			else if (barycentric(feature4, feature7, corner3, co_ordinates))
				triangle_case = 9;
			else if (barycentric(feature4, feature5, feature6, co_ordinates))
				triangle_case = 10;
			else if (barycentric(feature4, feature7, feature6, co_ordinates))
				triangle_case = 11;
			else if (barycentric(feature6, feature7, feature, co_ordinates))
				triangle_case = 12;
			else if (barycentric(feature7, feature6, corner4, co_ordinates))
				triangle_case = 13;
			else if (barycentric(feature5, feature6, corner4, co_ordinates))
				triangle_case = 14;
			else if (barycentric(feature2, feature5, corner4, co_ordinates))
				triangle_case = 15;
			else 
				triangle_case = 16;


			switch (triangle_case)		
			{
			case 1:
				u = 0-4.6534 + (0.9723*co_ordinates[0]) + (0.0702*co_ordinates[1]);
				v = 0 + 4.8487 - (0.0753*co_ordinates[0]) + (0.9068*co_ordinates[1]);
				break;
			case 2:
				u = 0 + 17.4633 + (0.9441*co_ordinates[0]) + (0.0455*co_ordinates[1]);
				v = 0 + 12.9672 - (0.0857*co_ordinates[0]) + (0.8977*co_ordinates[1]);
				break;
			case 3:
				u = 0 - 10.7505 + (0.6718*co_ordinates[0]) + (0.219*co_ordinates[1]);
				v = 0 + 9.9791 - (0.1145*co_ordinates[0]) + (0.9161*co_ordinates[1]);
				break;
			case 4:
				u = 0 - 107.4542 + (0.8209*co_ordinates[0]) + (0.3155*co_ordinates[1]);
				v = 0 - 5.5283 - (0.0906*co_ordinates[0]) + (0.9316*co_ordinates[1]);
				break;
			case 5:
				u = 0+ 84.6191 + (0.9758*co_ordinates[0]) - (0.0662*co_ordinates[1]);
				v = 0 - 242.1155 - (0.0851*co_ordinates[0]) + (1.2843*co_ordinates[1]);
				break;
			case 6:
				u = 0 + 97.8281 + (1*co_ordinates[0]) - (0.0937*co_ordinates[1]);
				v = 0 - 195.6562 - (0*co_ordinates[0]) + (1.1875*co_ordinates[1]);
				break;
			case 7:
				u = 0 - 47.5628 + (1.262*co_ordinates[0]) + (0.0455*co_ordinates[1]);
				v = 0 + 106.9943 - (0.5455*co_ordinates[0]) + (0.8977*co_ordinates[1]);
				break;
			case 8:
				u = 0 - 0.0686 + (1.1373*co_ordinates[0]) + (0*co_ordinates[1]);
				v = 0 + 0.1324 - (0.2647*co_ordinates[0]) + (1*co_ordinates[1]);
				break;
			case 9:
				u = 0 - 0.1429 + (1.3944*co_ordinates[0]) - (0.1086*co_ordinates[1]);
				v = 0 + 0.0599 - (0*co_ordinates[0]) + (0.8882*co_ordinates[1]);
				break;
			case 10:
				u = 0 + 177.4928 + (0.6718*co_ordinates[0]) - (0.1704*co_ordinates[1]);
				v = 0 - 2.0132 - (0.1145*co_ordinates[0]) + (0.9409*co_ordinates[1]);
				break;
			case 11:
				u = 0 + 64.9406 + (0.9374*co_ordinates[0]) - (0.0499*co_ordinates[1]);
				v = 0 + 25.6744 - (0.1799*co_ordinates[0]) + (0.9113*co_ordinates[1]);
				break;
			case 12:
				u = 0 + 268.8876 - (0.4956*co_ordinates[0]) + (0.4708*co_ordinates[1]);
				v = 0 + 108.4814 - (0.7617*co_ordinates[0]) + (1.1227*co_ordinates[1]);
				break;
			case 13:
				u = 0 + 72.0079 + (0.8878*co_ordinates[0]) - (0.0319*co_ordinates[1]);
				v = 0 + 0.077 - (0*co_ordinates[0]) + (0.8459*co_ordinates[1]);
				break;
			case 14:
				u = 0 + 171.4408 + (0.7329*co_ordinates[0]) - (0.2003*co_ordinates[1]);
				v = 0 - 15.7914 + (0.0247*co_ordinates[0]) + (0.8728*co_ordinates[1]);
				break;
			case 15:
				u = 0 - 187.5157 + (1.2922*co_ordinates[0]) + (0.1541*co_ordinates[1]);
				v = 0 - 28.4723 + (0.0445*co_ordinates[0]) + (0.8853*co_ordinates[1]);
				break;
			case 16:
				u = 0 + 81.502 + (0.873*co_ordinates[0]) - (0*co_ordinates[1]);
				v = 0 - 228.7316 + (0.3566*co_ordinates[0]) + (1*co_ordinates[1]);
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

	const char output_file[] = "sarath_wrap1.raw";
	file = fopen (output_file,"wb");
	fwrite(output, sizeof(unsigned char),3*height*width,file);
	fclose (file);

	system("pause");
	return 0;
}
