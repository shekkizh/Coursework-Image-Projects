#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define height 300
#define width 400
using namespace std;

struct data_seq
	{
		unsigned char gray_value;
		int row_index, column_index;
	};			//defining a global structure for storing the indices and value  

int bubblesort(data_seq input_array[width*height])
{
	int i,j;	//loop variables	
	data_seq swap;	//temporary variable for swapping data
	
	//bubble sort
	 for (i=0; i < height*width; i++)
	 {
		 for(j=0;j < height*width - 1; j++)
		 {
			 if(input_array[j+1].gray_value < input_array[j].gray_value)
			 {
				 swap = input_array[j];
				 input_array[j] = input_array[j+1];
				 input_array[j+1] = swap;
			 }
		 }
	 }		
return 0;
}

int main()
{
	unsigned char input[height][width], output[height][width];
	FILE *file;
	const char input_file[] = "desk_gray.raw";	//input file
	if (!(file = fopen(input_file,"rb")))
	{
		cout<<"file: "<<input_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input, sizeof(unsigned char), width*height, file);
	fclose(file);

	//hist_eq_digital
	
	int i,j;	//loop variables
	data_seq input_data[width*height];

	for (i = 0; i < height ; i++)
		{
			for (j = 0; j < width; j++)
			{
				input_data[i*width+j].gray_value = input[i][j];
				input_data[i*width+j].row_index = i;
				input_data[i*width+j].column_index = j;
			}
		}		//converting to linear array
	
	bubblesort (input_data);	//sorting
	int partition = (width*height/256);	

	//dividing into 256 parts and giving value
	for (i=0; i < 256; i++)
	{
		for(j=0; j < partition; j++)
				input_data[((i)*partition) + j].gray_value = (unsigned char) (i);
	}
	

	//mapping input values to equalized value 
	int row_index, column_index;	//position where equalized valueis to be stored in the image
	for (i = 0; i < height ; i++)
		{
			for (j = 0; j < width; j++)
			{
				row_index = input_data[i*width+j].row_index;
				column_index = input_data[i*width+j].column_index;
				output[row_index][column_index] = input_data[i*width+j].gray_value;
			}
		}

	// writing to output file
	const char output_file[] = "desk_enhanced4.raw";	//output file
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