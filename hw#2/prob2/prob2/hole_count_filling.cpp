#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 400
#define width 550

using namespace std;

int main()
{
	unsigned char input [height][width],input2 [height][width],output[height][width];
	FILE *file;
	const char input_file[] = "PCB_shrink1.raw";
	if (!(file = fopen (input_file, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input, sizeof(unsigned char), height*width, file);
	fclose (file);

	const char input_file1[] = "PCB.raw";
	if (!(file = fopen (input_file1, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input2, sizeof(unsigned char), height*width, file);
	fclose (file);

	int i,j,k,l,m,n;	//loop variables
	int count=0;
	memcpy(output,input2,height*width);
	for(i=1; i<height-1; i++)
	{
		for(j=1;j<width-1;j++)
		{
			if (input[i][j] == 255)
			{
			int sum = 0;
			for(k=i-1;k<=i+1;k++)
			{
				for(l=j-1;l<=j+1;l++)
				{
					sum+=input[k][l];
				}
			}
			if (sum==255)
			{
			count++;
			for (m=i-5;m<=i+5;m++)
			{
				for(n=j-5;n<=j+5;n++)
				{
					output[m][n] = 0;
				}
			}
			}
			}
		}
	}
	cout<<count<<endl;

	const char output_file2[] = "PCB_hole.raw";
	if (!(file = fopen (output_file2, "wb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fwrite(output,sizeof(unsigned char), height*width, file);
	fclose(file);

	system("pause");
	return 0;
}
