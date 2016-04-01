#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 512
#define width 512
#define mask_size 3		//window size used to denoise
#define weight_values 1,1,1,1,1,1,1,1,1  //weights to be used while mean filtering

using namespace std;
unsigned char mean(unsigned char window[mask_size*mask_size])
{
	//mean filter
	float sum = 0, average_divisor = 0;
	unsigned char mean_value;
	float weights[mask_size*mask_size]={weight_values};
	int i; //loop variable
	for (i=0; i<mask_size*mask_size; i++)
	{
		sum = sum + weights[i]*window[i];
		average_divisor += weights[i];
	}
	mean_value = (unsigned char)(int) (sum/(average_divisor));
	return (mean_value);
}

int quicksort(unsigned char *start_point, unsigned char *end_point)
{
	//sorting done for median filtering
    int arraysize = (end_point-start_point)+1;
    if(arraysize > 1)
	{
        unsigned char *pivot = (start_point+(arraysize/2));
        int swap = 0;
        unsigned char *origl = start_point;
        unsigned char *origr = end_point;
        while (start_point != end_point)
		{
            while (*start_point < *pivot)
			{
                start_point++;
            }
            while (*end_point > *pivot)
			{
                *end_point--;
            }
            if (start_point == pivot)
			{
                pivot = end_point;
            }
            else if (end_point == pivot)
			{
                pivot = start_point;
            }
            swap = *start_point;
            *start_point = *end_point;
            *end_point = swap;
            if((*start_point == *pivot) || (*end_point == *pivot))
			    break;
         }
        quicksort(origl, pivot-1);
        quicksort(pivot+1, origr);
    }
return 0;
}


unsigned char median(unsigned char window[mask_size*mask_size])
{
	//median filter
	unsigned char median_value;
	quicksort (&window[0], &window[mask_size*mask_size-1]);	
	median_value = window[((mask_size*mask_size)-1)/2];
	return (median_value);
}

int main()
{
	const int extension = (mask_size - 1)/2;
	unsigned char input[height][width], output[height][width];
	FILE *file;
	const char input_file[] = "peppers_mixed.raw";	//input file
	if (!(file = fopen(input_file,"rb")))
	{
		cout<<"file: "<<input_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input, sizeof(unsigned char), width*height, file);
	fclose(file);
	
	//mixednoise denoising
	//filter1 
	unsigned char extended_input[height+(2*extension)][width+(2*extension)] ={128};
	unsigned char intermediate_output[height+2*extension][width+2*extension] = {128};
	int i,j,k,l;	//loop variables

	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			extended_input[i][j] = input[i-extension][j-extension];	//extending the input array for filtering
		}
	}

	unsigned char window[mask_size*mask_size];

	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			if (extended_input[i][j] == 0 || extended_input[i][j] == 255)	//selective filter
			{
			int window_pixel = 0;
			for (k=(i-extension);k<=(i+extension);k++)
			{
				for (l=(j-extension);l<=(j+extension);l++)
				{
					window[window_pixel] = extended_input[k][l];
					window_pixel++;
				}
			}
			
			extended_input[i][j] = median(window);	//recursive median filter
			}
			else
			{
				extended_input[i][j] = extended_input[i][j];	
			}
		}
	}
	
//filter2
	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			int window_pixel = 0;
			for (k=(i-extension);k<=(i+extension);k++)
			{
				for (l=(j-extension);l<=(j+extension);l++)
				{
					window[window_pixel] = extended_input[k][l];
					window_pixel++;
				}
			}
			
			extended_input[i][j] = mean(window);	//recursive mean filter
		}
	}

	//filter3
	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			int window_pixel = 0;
			for (k=(i-extension);k<=(i+extension);k++)
			{
				for (l=(j-extension);l<=(j+extension);l++)
				{
					window[window_pixel] = extended_input[k][l];
					window_pixel++;
				}
			}
			
			output[i-extension][j-extension] = mean(window);	//mean filter
		}
	}

	

	const char output_file[] = "peppers_filtered1.raw";	//output file
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