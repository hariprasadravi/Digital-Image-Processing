// EE569 Homework Assignment #2
// Date:     Oct 9, 2016
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
	int Sizeh = 256;
	int Sizew = 256;
	
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
			Sizeh = atoi(argv[4]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata[Sizeh][Sizew][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Sizeh*Sizew*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////


	// JJN

	// Copying mage data to bigger matrix to accomadate 5x5 window size
	float NewImagedata[Sizeh+4][Sizew+4][BytesPerPixel];

	for(int k=0;k<BytesPerPixel;k++){
		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){
				NewImagedata[i+2][j+2][k]=(float)Imagedata[i][j][k];
			}
		}
	}

	////___________________________________________
	// Serpentine scanning 

	double error;
	int initial;
	for(int k=0;k<BytesPerPixel;k++){
		for(int i=2;i<Sizeh-2;i++){

			if(i%2 == 0){										// check for even rows

				for(int j=2;j<Sizew-2;j++){


					initial = NewImagedata[i][j][k];
					if((int)NewImagedata[i][j][k] > 127){		// thresholding
						NewImagedata[i][j][k] = 255;
					}
					else{
						NewImagedata[i][j][k] = 0;

					}
					error = initial - NewImagedata[i][j][k];		// calculating error

					NewImagedata[i][j+1][k] += ((7*error)/48);
					NewImagedata[i][j+2][k] += ((5*error)/48);		// diffusing error to unprocessed pixels
					NewImagedata[i+1][j][k] += ((7*error)/48);
					NewImagedata[i+1][j+1][k] += ((5*error)/48);
					NewImagedata[i+1][j+2][k] += ((3*error)/48);
					NewImagedata[i+2][j][k] += ((5*error)/48);
					NewImagedata[i+2][j+1][k] += ((3*error)/48);
					NewImagedata[i+2][j+2][k] += ((1*error)/48);
					NewImagedata[i+1][j-2][k] += ((3*error)/48);
					NewImagedata[i+1][j-1][k] += ((5*error)/48);
					NewImagedata[i+2][j-1][k] += ((3*error)/48);
					NewImagedata[i+2][j-2][k] += ((1*error)/48);
						

				}
			}
			else{
				for(int j=Sizew-3;j>=2;j--){					// repeating for odd rows
					
					initial = NewImagedata[i][j][k];
					if((int)NewImagedata[i][j][k] > 127){
						NewImagedata[i][j][k] = 255;
					}
					else{
						NewImagedata[i][j][k] = 0;

					}
					error = initial - NewImagedata[i][j][k];
					
					NewImagedata[i][j-1][k] += ((7*error)/48);
					NewImagedata[i][j-2][k] += ((5*error)/48);
					NewImagedata[i+1][j][k] += ((7*error)/48);
					NewImagedata[i+1][j+1][k] += ((5*error)/48);
					NewImagedata[i+1][j+2][k] += ((3*error)/48);
					NewImagedata[i+2][j][k] += ((5*error)/48);
					NewImagedata[i+2][j+1][k] += ((3*error)/48);
					NewImagedata[i+2][j+2][k] += ((1*error)/48);
					NewImagedata[i+1][j-2][k] += ((3*error)/48);
					NewImagedata[i+1][j-1][k] += ((5*error)/48);
					NewImagedata[i+2][j-1][k] += ((3*error)/48);
					NewImagedata[i+2][j-2][k] += ((1*error)/48);
	

				}
			}
		}
	}
 	//____________________________________________________________

 	// copying computed values to new matrix by discarding two columns and rows pixels on all 4 sides

	unsigned char ditheredImage[Sizeh][Sizew][BytesPerPixel];

	for(int k=0;k<BytesPerPixel;k++){
		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){

				ditheredImage[i][j][k] = (unsigned char)NewImagedata[i+2][j+2][k];
				
			}
		}
	}




	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(ditheredImage, sizeof(unsigned char), Sizeh*Sizew*BytesPerPixel, file);
	fclose(file);

	return 0;
}
