#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#define height 300
#define width 200
#define test_image "license4.raw"
#define output_image "OCR.raw"
#define threshold 128

using namespace std;

int quad_count0(unsigned char input_seq)
{
	int count=0;
	if (input_seq == 0x09 || input_seq == 0x06)
		count++;
	return (count);
}
int quad_count1(unsigned char input_seq)
{
	int count=0;
	if (input_seq == 0x08 || input_seq == 0x04 || input_seq == 0x02 || input_seq == 0x01)
		count++;
	return (count);
}
int quad_count2(unsigned char input_seq)
{
	int count=0;
	if (input_seq == 0x0C || input_seq == 0x05 || input_seq == 0x03 || input_seq == 0x0A)
		count++;
	return (count);
}
int quad_count3(unsigned char input_seq)
{
	int count=0;
	if (input_seq == 0x0D || input_seq == 0x07 || input_seq == 0x0B || input_seq == 0x0E)
		count++;
	return (count);
}
int quad_count4(unsigned char input_seq)
{
	int count=0;
	if (input_seq == 0x0F)
		count++;
	return (count);
}
int main()
{
	unsigned char training_rgb[10][height][width][3], training[10][height][width], test_rgb[height][width][3],test[height][width];
	unsigned char output[height][width];
	int quad[11][5];	//quad bit count of every pixel
	int area[11],perimeter[11],euler[11];
	int i_start=height,j_start=width,i_end=0,j_end=0;
	float circularity[11];
	double x_moment[11], y_moment[11];
	double cross_inertia[11];
	FILE *file;
	const char *training_image[10] =   {"training1.raw",
										"training2.raw",
										"training3.raw",
										"training4.raw",
										"training5.raw",
										"training6.raw",
										"training7.raw",
										"training8.raw",
										"training9.raw",
										"training10.raw"};
	const char test_file[] = test_image;
	if(!(file=fopen(test_file,"rb")))
	{
		cout<<"the test image does not exist"<<endl;
		system ("pause");
		return 0;
	}
	fread(test_rgb,sizeof (unsigned char),3*height*width, file);
	fclose (file);

	int i,j,k,l;	//loop variables
	for (i=0;i<10;i++)
	{
	const char *input_file = training_image[i];
	if(!(file=fopen(input_file,"rb")))
	{
		cout<<"the training image does not exist"<<endl;
		system ("pause");
		return 0;
	}
	fread(training_rgb[i],sizeof (unsigned char),3*height*width, file);
	fclose (file);
	}

	int gray_value;
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			gray_value = (test_rgb[i][j][0] + test_rgb[i][j][1] + test_rgb[i][j][2])/3 ;
			if (gray_value > threshold)
				test[i][j] = 0;
			else
				test[i][j] = 1;
		}
	}

	for (i=0;i<4;i++)
	{
		for (j=0;j<width;j++)
		{
			test[i][j] = 0;
			test[height-i-1][j] = 0;
		}
	}
	for (i=0;i<height;i++)
	{
		for (j=0;j<4;j++)
		{
			test[i][j] = 0;
			test[i][width-j-1] = 0;
		}
	}

	int index;
	for (index=0;index<10;index++)
	{
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			gray_value = (training_rgb[index][i][j][0] + training_rgb[index][i][j][1] + training_rgb[index][i][j][2])/3 ;
			if (gray_value > threshold)
				training[index][i][j] =0;
			else
				training[index][i][j] =255;
		}
	}
	}
	unsigned char input_sequence;
	int i_temp=0;
	int j_temp=0;

	// feature set of training images
	for (index=0;index<10;index++)
	{
		cross_inertia[index] = 0;
		x_moment[index] = 0;
		y_moment[index] = 0;
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			if (j_temp < j_start)
				j_start = j_temp;
			if (j_temp > j_end)
				j_end = j_temp;
			if (training[index][i][j] == 255)
			{
				i_temp = i;
				j_temp = j;
			if (i_temp < i_start)
				i_start = i_temp;
			if (i_temp > i_end)
				i_end = i_temp;
				
			if (j_temp < j_start)
				j_start = j_temp;
			if (j_temp > j_end)
				j_end = j_temp;
			}
			input_sequence = 0;
			for (k=i;k<i+2;k++)
			{
				for (l=j;l<j+2;l++)
				{
					input_sequence *=2;
					input_sequence +=(training[index][k][l]/255);
				}
			}
			if (i==0 && j==0)
			{	
			quad[index][0] = quad_count0(input_sequence);	//diagonal count
			quad[index][1] = quad_count1(input_sequence);	//bit 1 count
			quad[index][2] = quad_count2(input_sequence);	//bit 2 count
			quad[index][3] = quad_count3(input_sequence);	//bit 3 count
			quad[index][4] = quad_count4(input_sequence);	//bit 4 count
			}
			else
			{
			quad[index][0] += quad_count0(input_sequence);
			quad[index][1] += quad_count1(input_sequence);
			quad[index][2] += quad_count2(input_sequence);
			quad[index][3] += quad_count3(input_sequence);
			quad[index][4] += quad_count4(input_sequence);
			}
			x_moment[index] += (j+0.5)*(training[index][i][j]/255);
			y_moment[index] += (i+0.5)*(training[index][i][j]/255);
			cross_inertia[index] += (i+0.5)*(j+0.5)*(training[index][i][j]/255);
		}
	}
	area[index] = (quad[index][1] + 2*quad[index][2] + 3*quad[index][3] + 4*quad[index][4] + 2*quad[index][0])/4;
	x_moment[index] = (x_moment[index]/area[index] - j_start)/(j_end - j_start);
	y_moment[index] = (y_moment[index]/area[index] - i_start)/(i_end - i_start);
	cross_inertia[index] = cross_inertia[index]/area[index];
	perimeter[index] = (quad[index][1] + quad[index][2] + quad[index][3] + 2*quad[index][0]);
	euler[index] = (quad[index][1] - quad[index][3] - 2*quad[index][0])/4;
	circularity[index] = (4*3.14*area[index])/(perimeter[index]*perimeter[index]);
	i_start=height;j_start=width;i_end=0;j_end=0;
	}
	
	//features of test image
	x_moment[10] = 0;
	y_moment[10] = 0;
	cross_inertia[10] = 0;
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			if (test[i][j] == 1)
			{
				i_temp = i;
				j_temp = j;
			if (i_temp < i_start)
				i_start = i_temp;
			if (i_temp > i_end)
				i_end = i_temp;
				
			if (j_temp < j_start)
				j_start = j_temp;
			if (j_temp > j_end)
				j_end = j_temp;
			}
			input_sequence = 0;
			for (k=i;k<i+2;k++)
			{
				for (l=j;l<j+2;l++)
				{
					input_sequence *=2;
					input_sequence +=(test[k][l]);
				}
			}
			if (i==0 && j==0)
			{	
			quad[10][0] = quad_count0(input_sequence);	//diagonal count
			quad[10][1] = quad_count1(input_sequence);	//bit 1 count
			quad[10][2] = quad_count2(input_sequence);	//bit 2 count
			quad[10][3] = quad_count3(input_sequence);	//bit 3 count
			quad[10][4] = quad_count4(input_sequence);	//bit 4 count
			}
			else
			{
			quad[10][0] += quad_count0(input_sequence);
			quad[10][1] += quad_count1(input_sequence);
			quad[10][2] += quad_count2(input_sequence);
			quad[10][3] += quad_count3(input_sequence);
			quad[10][4] += quad_count4(input_sequence);
			}
			x_moment[10] += (j+0.5)*(test[i][j]);
			y_moment[10] += (i+0.5)*(test[i][j]);
			cross_inertia[10] += (i+0.5)*(j+0.5)*(test[i][j]);
		}
	}
	area[10] = (quad[10][1] + 2*quad[10][2] + 3*quad[10][3] + 4*quad[10][4] + 2*quad[10][0])/4;
	x_moment[10] = (x_moment[10]/area[10] - j_start)/(j_end - j_start);
	y_moment[10] = (y_moment[10]/area[10] - i_start)/(i_end - i_start);
	cross_inertia[10] = cross_inertia[10]/area[10];
	perimeter[10] = (quad[10][1] + quad[10][2] + quad[10][3] + 2*quad[10][0]);
	euler[10] = (quad[10][1] - quad[10][3] - 2*quad[10][0])/4;
	circularity[10] = (4*3.14*area[10])/(perimeter[10]*perimeter[10]);
	
	int error[10],count=0;
	int flag = 0;
	float distance;
	float min_distance;
	for (i=0;i<10;i++)
	{
		error[i] = euler[10] - euler[i];
		if (error[i]==0)
		{
			count++; index=i;
		}
	}

	if (count==1)
		memcpy (output,training[index],height*width);
	else if (count == 2)
	{
		for (i=0;i<10;i++)
		{
			if (error[i] == 0)
			{
				if (x_moment[i]<0.5 && x_moment[10] <0.5)
					memcpy (output,training[i],height*width);
				else if (x_moment[i]>0.5 && x_moment[10] >0.5 )
					memcpy (output,training[i],height*width);
			}
		}
	}
	else
	{
		
		min_distance = abs(y_moment[10] -y_moment[1]);
		index = 1;
		for (i=0; i<10; i++)
		{
			if (error[i]==0)
			{
				distance =abs(y_moment[10] - y_moment[i]);
				if (x_moment[10]<0.45 && y_moment[10]<0.45 && x_moment[i]<0.45 && y_moment[i]<0.45 )
					{memcpy(output,training[i],height*width); flag=1;break;}
				
				else if (distance<min_distance)
				{
					if (distance<0.001)
					{index = i;min_distance=distance;break;}
					else
					{
						if (abs(x_moment[10] - x_moment[i]) < 0.05)
						{index=i; min_distance=distance;}
					}
				}
			}
		}
		if (flag==0)
		memcpy(output,training[index],height*width);
	}
	
	const char output_file[] = output_image;
	file = fopen (output_file, "wb");
	fwrite (test, sizeof (unsigned char), height*width, file);
	fclose(file);
	
	system ("pause");
	return 0;
}