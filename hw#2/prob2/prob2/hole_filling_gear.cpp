#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 250
#define width 250

using namespace std;

int main()
{
	unsigned char input1 [height][width],input2 [height][width],output1[height][width], output2[height][width]={0};
	FILE *file;
	const char input_file1[] = "Gear_shrink.raw";
	if (!(file = fopen (input_file1, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input1, sizeof(unsigned char), height*width, file);
	fclose (file);

	const char input_file2[] = "GearTooth.raw";
	if (!(file = fopen (input_file2, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input2, sizeof(unsigned char), height*width, file);
	fclose (file);

	int i,j,m,n;	//loop variables
	int sumx=0,sumy=0;
	memcpy(output1,input2,height*width);
	for(i=75; i<height-75; i++)
	{
		for(j=75;j<width-75;j++)
		{
			if (input1[i][j] == 0)
			{
				sumx+= j;
				sumy+= i;
				output2[i][j]=255;
			for (m=i-25;m<=i+25;m++)
			{
				for(n=j-25;n<=j+25;n++)
				{
					output1[m][n] = 255;
				}
			}
			}
		}
	}
	cout<<"x co-ordinate: "<< sumx/4<<endl <<"y co-ordinate: "<<sumy/4<<endl;

	const char output1_file[] = "Gear_hole.raw";
	if (!(file = fopen (output1_file, "wb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fwrite(output1,sizeof(unsigned char), height*width, file);
	fclose(file);

	const char output2_file[] = "Gear_hole_position.raw";
	if (!(file = fopen (output2_file, "wb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fwrite(output2,sizeof(unsigned char), height*width, file);
	fclose(file);

	system("pause");
	return 0;
}
