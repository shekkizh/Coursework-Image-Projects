#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 250
#define width 250

using namespace std;

int main()
{
	unsigned char input [height][width], output[height][width]={0};
	FILE *file;
	const char input_file[] = "Gear_position.raw";
	if (!(file = fopen (input_file, "rb")))
	{
		cout<<"The input image does not exsist"<<endl;
		system ("pause");
		return 0;
	}
	fread (input, sizeof(unsigned char), height*width, file);
	fclose (file);

	int i,j,k,l;	//loop variables
	int flag;
	for(i=0;i<=height;i++)
	{
		for(j=0;j<width;j++)
		{
			flag=0;
			if (input[i][j]==255)
			{
			for(k=0;k<10;k++)
			{
				for(l=0;l<10;l++)
				{
				if((input[height-i-k][width-j-l]==255))
				{
				flag=1;
				break;
				}
				}
			}
			if (flag==0)
				output[height-i-k/2][width-j-l/2] = 255;
			}
		}
	}


	const char output_file[] = "Gear_missing.raw";
	file = fopen(output_file,"wb");
	fwrite(output,sizeof(unsigned char), height*width, file);
	fclose(file);

	system("pause");
	return 0;
}
			