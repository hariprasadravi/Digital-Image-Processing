
// EE569 Homework Assignment #1
// Date:     Sep 18, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu


#include <stdio.h>
#include <iostream>
#include <stdlib.h>


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
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Width = 256] [Height= 256]" << endl;
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

	int index=0;
	unsigned long int sum = 0;
	double count[256][BytesPerPixel] ={0};

	// Pre-assigning 

	for(int k=0;k<BytesPerPixel;k++){
		for(int i=0;i< 256;i++){
			count[i][k]=0;
		}
	}


	// Calculating the Probability of Colour Intensity //
	for(int k=0; k< BytesPerPixel;k++){
		for(int i = 0; i < Sizew; i++){
			for(int j=0;j<Sizeh;j++){
				index = Imagedata[j][i][k];
				count[index][k]+=1;



			}
		}

	}


	// Calculating the Cumulative Probability //
	for(int k=0;k<BytesPerPixel;k++){
		for(int i=0;i<256;i++){

			count[i][k]/= (Sizew*Sizeh);
			if(i>=1){
				count[i][k]+=count[i-1][k];

			}


		}
	}

	for(int k=0;k<BytesPerPixel;k++){
		for(int i=0;i<256;i++){

			count[i][k]*=255;

		}
	}




	// // Copying to new image //
	unsigned char NewImagedata[Sizeh][Sizew][BytesPerPixel];
	for(int k=0;k<BytesPerPixel;k++){
		for(int i = 0;i<Sizew;i++){
			for(int j = 0;j<Sizeh;j++){
				
					NewImagedata[j][i][k]=(unsigned char)(int)count[int(Imagedata[j][i][k])][k];
				

			}
		}
	}
	
	

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(NewImagedata, sizeof(unsigned char), (Sizew)*(Sizeh)*BytesPerPixel, file);
	fclose(file);
	return 0;
}