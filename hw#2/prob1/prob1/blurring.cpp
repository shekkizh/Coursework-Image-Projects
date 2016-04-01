#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#define height 256
#define width 256
#define mask_size 7	
#define co_ordinates {70,170,30,130}	//background region to be left unblurred {x1,x2,y1,y2}
//#define co_ordinates {130,250,130,250}	//foreground region to be left unblurred
using namespace std;

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
	unsigned char input[height][width][3], output[height][width][3];
	FILE *file;
	const char input_file[] = "sailboat_background.raw";	//input file
	if (!(file = fopen(input_file,"rb")))
	{
		cout<<"file: "<<input_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fread(input, sizeof(unsigned char), width*height*3, file);
	fclose(file);
	
	//median filter
	unsigned char extended_input[height+(2*extension)][width+(2*extension)][3] ={128};
	int i,j,k,l,m;	//loop variables

	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			for(m=0; m<3; m++)
			{
			extended_input[i][j][m] = input[i-extension][j-extension][m];	//extending the input array for filtering
			}
		}
	}

	unsigned char window[mask_size*mask_size];
	int focus_region[4] = co_ordinates;		//region of focus

	for (i=extension; i<height+extension; i++)
	{
		for (j=extension; j<width+extension; j++)
		{
			for (m=0; m<3; m++)
			{
				if (!((i>focus_region[0] & i<focus_region[1])&(j>focus_region[2] & j<focus_region[3])))	//selective filter
				{
				int window_pixel = 0;
					for (k=(i-extension);k<=(i+extension);k++)
					{
						for (l=(j-extension);l<=(j+extension);l++)
						{
							window[window_pixel] = extended_input[k][l][m];
							window_pixel++;
						}
					}
			
				output[i-extension][j-extension][m] = median(window);	//median filter
				}
				else
				{
				output[i-extension][j-extension][m] = extended_input[i][j][m];
				}
			}
		}
	}

	const char output_file[] = "sailboat_blur.raw";	//output file
	if(!(file = fopen(output_file,"wb")))
	{
		cout<<"file: "<<output_file<<" does not exsist or cannot be opened"<<endl;
		exit(1);
	}
	fwrite(output, sizeof(unsigned char), width*height*3, file);
	fclose(file);

system ("pause");
return 0;
}