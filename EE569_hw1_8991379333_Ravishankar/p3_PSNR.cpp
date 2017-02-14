// EE569 Homework Assignment #1
// Date:     Sep 18, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
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
		cout << "program_name Original_image.raw Noisy_image.raw [BytesPerPixel = 1] [Width = 256] [Height = 256]" << endl;
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

	///// Reading Original Image
	
	// Allocate image data array

	///// First argument is height and second is width 
	unsigned char Imagedata_original[Sizeh][Sizew][BytesPerPixel];


	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata_original, sizeof(unsigned char), Sizeh*Sizew*BytesPerPixel, file);
	fclose(file);

	///// Reading Noisy Image
	
	// Allocate image data array

	///// First argument is height and second is width 
	unsigned char Imagedata_noisy[Sizeh][Sizew][BytesPerPixel];


	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(Imagedata_noisy, sizeof(unsigned char), Sizeh*Sizew*BytesPerPixel, file);
	fclose(file);
	
	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	double PSNR[3]={0};
	double MSE=0;
	for(int k=0;k<BytesPerPixel;k++){
		MSE=0;
		for(int i=0;i<Sizew;i++){
			for(int j=0;j<Sizeh;j++){
				MSE+= pow((Imagedata_noisy[j][i][k] - Imagedata_original[j][i][k]),2.0);
			}
		}
		MSE/=(Sizew*Sizeh);
		PSNR[k] = 10 * log10((255*255)/MSE);
		cout<<PSNR[k]<<" "<<"MSE = "<<MSE<<endl;
	}

	
	return 0;
}