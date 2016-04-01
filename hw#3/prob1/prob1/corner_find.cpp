#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define height 500
#define width 400
#define mask_size 3
#define input_image "lion.raw"
using namespace std;

int main()
{
	unsigned char input[height][width][3];
	FILE *file;

	const char input_file[] = input_image;
	if (!(file=fopen (input_file,"rb")))
	{
		cout<< "the input file does not exist"<<endl;
		system("pause");
	}
	fread(input, sizeof(unsigned char), height*width*3, file);
	fclose(file);

	int i,j,k,l;	//loop variables
	const int extension = (mask_size-1)/2;

	int count;
	for (i=extension;i<(height-extension);i++)
	{
		for(j=extension;j<(width-extension);j++)
		{
			if ((input[i][j][0]==255))
			{
			count = 0;
			for(k=i-extension;k<=i+extension;k++)
			{
				for(l=j-extension;l<=j+extension;l++)
				{
					if (input[k][l][0] != 255)
					{
						count++;
					}
				}
			}
			if (count>(mask_size*mask_size/2))
				cout<<i<<"   "<<j<<endl;
			}
		}
	}

	system("pause");
	return 0;
}

	