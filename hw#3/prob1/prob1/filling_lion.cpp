#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define height 500
#define width 400

using namespace std;

int main()
{
	unsigned char input[height][width][3], puzzle[122][122][3], output[height][width][3];
	FILE *file;

	const char input_file[] = "lion.raw";
	if (!(file=fopen (input_file,"rb")))
	{
		cout<< "the input file does not exist"<<endl;
		system("pause");
	}
	fread(input, sizeof(unsigned char), height*width*3, file);
	fclose(file);
	
	memcpy(output,input,height*width*3);

	const char puzzle_file[] = "piece_transformed.raw";
	if (!(file=fopen (puzzle_file,"rb")))
	{
		cout<< "the puzzle file does not exist"<<endl;
		system("pause");
	}
	fread(puzzle, sizeof(unsigned char), 122*122*3, file);
	fclose(file);

	int i,j;	//loop variable
	for (i=143;i<263;i++)
	{
		for (j=228;j<348;j++)
		{
			output[i][j][0] = puzzle[i-142][j-227][0];
			output[i][j][1] = puzzle[i-142][j-227][1];
			output[i][j][2] = puzzle[i-142][j-227][2];
		}
	}

	const char output_file[] ="lion_full.raw";
	file = fopen(output_file,"wb");
	fwrite(output,sizeof(unsigned char), height*width*3,file);
	fclose(file);

	system("pause");
	return 0;
}