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

int quicksort(data_seq input_array[width*height],int start_point, int end_point)
{
	int pivot_index;
    int arraysize = (end_point-start_point)+1;
    if(arraysize > 1)
	{
		pivot_index = (start_point+end_point)/2;
		unsigned char pivot = input_array[pivot_index].gray_value;
        data_seq swap ;
        int origl = start_point;
        int origr = end_point;
        while (start_point < end_point)
		{
			while (input_array[start_point].gray_value < pivot)
			{
                start_point++;
            }
			while (input_array[end_point].gray_value > pivot)
			{
                end_point--;
            }
            if (start_point == pivot_index)
			{
				pivot_index = end_point;
            }
            else if (end_point == pivot_index)
			{
				pivot_index = start_point;
            }
            swap = input_array[start_point];
            input_array[start_point] = input_array[end_point];
            input_array[end_point] = swap;
			if((input_array[start_point].gray_value == pivot) || (input_array[end_point].gray_value == pivot))
			{
                break;
            }
        }
	}
    quicksort(input_array,0,pivot_index-1);
    quicksort(input_array, pivot_index+1, width*height - 1);
	return 0;
}

int main()
{
	unsigned char input[height][width], output[height][width];
	FILE *file;
	const char input_file[] = "desk_gray.raw";
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
	
	quicksort (input_data, 0, width*height-1);	//sorting
	int partition = (width*height/256);	

	for (i=1; i < 256; i++)
	{
		if (input_data[(i*partition)].gray_value != input_data[(i-1)*partition].gray_value)
		{
			for(j=0; j < partition; j++)
				input_data[(i*partition) + j].gray_value = (unsigned char) i;
		}
		else
		{
			for(j=0; j < partition; j++)				
				input_data[(i*partition) + j].gray_value = input_data[(i-1)*partition].gray_value;
		}
	}
	


	int row_index, column_index;

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
	const char output_file[] = "desk_enhanced4.raw";
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