#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 400
#define width 550

using namespace std;

int main()
{
	unsigned char input1 [height][width], input2 [height][width],output[height][width]={0};
	FILE *file;
	const char input_file1[] = "PCB_hole_shrink.raw";
	if (!(file = fopen (input_file1, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input1, sizeof(unsigned char), height*width, file);
	fclose (file);

	const char input_file2[] = "PCB_shrink1.raw";
	if (!(file = fopen (input_file2, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input2, sizeof(unsigned char), height*width, file);
	fclose (file);

	int i,j,k,l;	//loop variables
	int count=0;
	for(i=1; i<height-1; i++)
	{
		for(j=1;j<width-1;j++)
		{
			if (input1[i][j] == 0)
			{
				int sum=0;
				for(k=i-2;k<=i+2;k++)
				{
					for(l=j-2;l<=j+2;l++)
						sum+= input2[k][l];
				}
				if (sum != 255)
				{
					count++;
					output[i][j] = 255;
				}
			}
		}
	}
	cout<<count<<endl;

	const char output_file[] = "PCB_hole_shrink_path_count.raw";
	if (!(file = fopen (output_file, "wb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fwrite (output, sizeof(unsigned char), height*width, file);
	fclose (file);
	system("pause");
	return 0;
}

