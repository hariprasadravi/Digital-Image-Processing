
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
		cout << "program_name input_image.raw [BytesPerPixel = 1] [Width = 256] [Height =256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[2]);
		// Check if size is specified
		if (argc >= 5){
			Sizew = atoi(argv[3]);
			Sizeh = atoi(argv[4]);
			
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

	/////// Finding components of R,G and B ///////////////////////////////

	
 	unsigned char C[Sizeh][Sizew];
 	unsigned char M[Sizeh][Sizew];
 	unsigned char Y[Sizeh][Sizew];
  	float buffer;
	
	

		for(int i= 0;i <= Sizew ;i++){

			for(int j=0;j <= Sizeh;j++){
					

					C[j][i] = 255 - int(Imagedata[j][i][0]);
					M[j][i] = 255 - int(Imagedata[j][i][1]);
					Y[j][i] = 255 - int(Imagedata[j][i][2]);
					
			


				}
		

		}
	
	
	
	

	// Write image data (filename specified by second argument) from image data matrix

	file = fopen("C.raw","wb");
	fwrite(C, sizeof(unsigned char), (Sizew)*(Sizeh), file);
	fclose(file);

	file = fopen("M.raw","wb");
	fwrite(M, sizeof(unsigned char), (Sizew)*(Sizeh), file);
	fclose(file);

	file = fopen("Y.raw","wb");
	fwrite(Y, sizeof(unsigned char), (Sizew)*(Sizeh), file);
	fclose(file);
	return 0;
}