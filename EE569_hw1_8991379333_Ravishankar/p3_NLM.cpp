
// EE569 Homework Assignment #1
// Date:     Sep 18, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <ctime>
#define pi 3.14

using namespace std;



int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Sizeh = 250;
	int Sizew = 300;
	
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Width = 256] [Height = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			Sizew = atoi(argv[4]);
			Sizeh = atoi(argv[5]);
			
		}
	}
	
	// Allocate image data array

	///// First argument is height and second is width 
	unsigned char Imagedata[Sizeh][Sizew][BytesPerPixel];


	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Sizeh*Sizew*BytesPerPixel, file);
	fclose(file);
	
	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	////// Padding the Image with 10 pixels on every edge //////////

	int h;
	unsigned char Padded_Image[Sizeh+20][Sizew+20][BytesPerPixel] ={0};

	
	
	for(int k=0 ; k< BytesPerPixel;k++){
		h=29;
		for(int j=0;j<Sizeh;j++){
			for(int i=0;i<Sizew;i++){
				Padded_Image[j+10][i+10][k]= Imagedata[j][i][k];

				if(j<10){
					Padded_Image[9-j][i+10][k]= Imagedata[j][i][k];
				}
				if(j>=Sizeh-10){
					Padded_Image[j+h][i+10][k] = Imagedata[j][i][k];
				}
			}
			if(j>=Sizeh-10){
					h-=2;
				}
		}
	}
	

	int neww = Sizew+20;
	int newh = Sizeh +20;
	int dec=1;
	int l;
	for(int i =10,l=neww-11; i<20,l>=neww-20; i++,l--){
		for(int j=0;j<(newh);j++){
			for(int k=0;k<BytesPerPixel;k++){
				Padded_Image[j][i-dec][k]=Padded_Image[j][i][k];
				Padded_Image[j][l+dec][k]=Padded_Image[j][l][k];


			}
		
		}
		dec+=2;
	}





	// Non-Local Means
	unsigned char Filtered_Image[Sizeh][Sizew][BytesPerPixel];
	int pivot[7][7]={0};
	double weight=0,weight_sum=0;
	double sum=0,total_sum=0;
	

	
	int sigma = 30; 
	double h_para = 0.35*sigma;
	clock_t begin = clock();
	for(int k=0;k<BytesPerPixel;k++){
		cout<<"Performing Filtering on Channel "<<k<<endl;
		for(int new_row = 0; new_row< Sizeh; new_row++){
			for(int new_col = 0; new_col<Sizew;new_col++){
				

				for(int i=0;i<7;i++){
					for(int j=0;j<7;j++){
						pivot[i][j] = Padded_Image[i+7 + new_row][j+7 +new_col][k];                     //fixed patch
					}
				}



				for(int l= new_row ;l<15 + new_row;l++){
					for(int w= new_col;w<15 + new_col ;w++){

							for(int i=0;i<7;i++){
								for(int j=0;j<7;j++){	
											
										sum+=pow((pivot[i][j] - Padded_Image[i+l][j+w][k]),2);            // moving patch
										
									
															
								} // End of j
								
							} // end of i
						

							sum/=49;
							weight = exp(((-1)*max((sum-(2*sigma*sigma)),0.0))/(h*h));

							weight_sum += weight;
							total_sum+= weight*Padded_Image[l][w][k];
							sum=0;
			
					} // End of w
				} // End of l 
		
				Filtered_Image[new_row][new_col][k] = total_sum/weight_sum;
				weight_sum = 0;
				total_sum=0;
				
			}
		
		}
	}
	clock_t end = clock();
  	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

  	cout<<"Elapsed Time = "<<elapsed_secs<< "seconds"<<endl;
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Filtered_Image, sizeof(unsigned char), (Sizew)*(Sizeh)*BytesPerPixel, file);
	fclose(file);
	return 0;
}