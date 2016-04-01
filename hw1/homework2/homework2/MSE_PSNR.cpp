#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#define height 512
#define width 512
#define filtered_file "selective med-mean-mean.raw"		
//image file for which MSE & PSNR are to be calculated 
using namespace std;

int main()
{
	unsigned char input1[height][width], input2[height][width], output[height][width];
	FILE *file;
	// reading 1st file 
	const char input1_file[] = "peppers.raw";	//original image file 
	if (!(file = fopen(input1_file,"rb")))
	{
		cout<<"file: "<<input1_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input1, sizeof(unsigned char), width*height, file);
	fclose(file);
	
	//reading 2nd file	
	const char input2_file[] = filtered_file;
	if(!(file = fopen(input2_file,"rb")))
	{
		cout<<"file: "<<input2_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input2, sizeof(unsigned char), width*height, file);
	fclose(file);

	//MeanSquareError
	int i,j;	//loop variables
	long int squared_error = 0;
	int error;
	float MSE;

	for (i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
		error =(int)input1[i][j] - (int)input2[i][j];
		output[i][j] = (unsigned char) abs(error);
		squared_error += (error*error);
		}
	}
	MSE = squared_error/(width*height);
	cout<<MSE<<endl;
	
	//PSNR
	float PSNR;
	PSNR = 48.1308 - (10*log10(MSE));
	cout<<PSNR<<endl;

//writing file	
	const char output_file[] = "Absolute_Error.raw";	//Absolut error map file
	if(!(file = fopen(output_file,"wb")))
	{
		cout<<"file: "<<output_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fwrite(output, sizeof(unsigned char), width*height, file);
	fclose(file);

system ("pause");
return 0;
}