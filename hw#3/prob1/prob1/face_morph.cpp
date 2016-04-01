#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define height 1044
#define width 642
#define input_image1 "face_morph3.raw"
#define input_image2 "terminator_wrap.raw"
#define parameter 0.1
#define i_start 300
#define i_end 470
#define j_start 110
#define j_end 425
using namespace std;

int main()
{
	unsigned char input1[height][width][3], input2[height][width][3], output[height][width][3];
	FILE *file;

	const char input_file1[] = input_image1;
	if (!(file=fopen (input_file1,"rb")))
	{
		cout<< "the input file does not exist"<<endl;
		system("pause");
	}
	fread(input1, sizeof(unsigned char), height*width*3, file);
	fclose(file);

	
	const char input_file2[] = input_image2;
	if (!(file=fopen (input_file2,"rb")))
	{
		cout<< "the input file does not exist"<<endl;
		system("pause");
	}
	fread(input2, sizeof(unsigned char), height*width*3, file);
	fclose(file);

	int i,j;
	float a = parameter; 
	memcpy(output,input1,height*width*3);
	for(i=i_start;i<i_end;i++)
	{
		for(j=j_start;j<j_end;j++)
		{
			output[i][j][0] = (1-a)*input1[i][j][0] + a*input2[i][j][0];
			output[i][j][1] = (1-a)*input1[i][j][1] + a*input2[i][j][1];
			output[i][j][2] = (1-a)*input1[i][j][2] + a*input2[i][j][1];
		}
	}

	const char output_file[] = "face_morph3.raw";
	file = fopen(output_file,"wb");
	fwrite(output,sizeof(unsigned char),3*height*width,file);
	fclose(file);

	system("pause");
	return 0;
}