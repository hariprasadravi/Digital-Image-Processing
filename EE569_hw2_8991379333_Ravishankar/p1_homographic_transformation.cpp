// EE569 Homework Assignment #2
// Date:     Oct 9, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Sizeh = 256;
	int Sizew = 256;
	int puzzle_w,puzzle_h;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Width = 256] [Height = 256] [Logo Image] [Width = 256] [Height = 256]" << endl;
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
			puzzle_w = atoi(argv[7]);
			puzzle_h = atoi(argv[8]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata[Sizeh][Sizew][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Sizew*Sizeh*BytesPerPixel, file);
	fclose(file);


	//////////

		// Allocate image data array
	unsigned char PuzzleImagedata[puzzle_h][puzzle_w][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[6],"rb"))) {
		cout << "Cannot open file: " << argv[6] <<endl;
		exit(1);
	}
	fread(PuzzleImagedata, sizeof(unsigned char), puzzle_h*puzzle_w*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	unsigned char NewImagedata[Sizeh][Sizew][BytesPerPixel];
	double x_,y_,w_,a,b;
	int nearest_x,nearest_y;
	int value;

	for(int k=0;k<BytesPerPixel;k++){
		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){

				// conditions for coordinates inside the Trapezoid
				if((j+1.269*i-1104.461 > 0) && (j-10*i+3854 < 0) && (j+0.8347*i -978.313 < 0) && (j -5.647*i +2658 > 0)){

					// Transform matrix multiplication
					w_ = 1/(0.0037*i -1.028);
					x_ = 1.4976*i*w_ + 1.1267*j*w_ -1265.49*w_;
					y_ = -1.4502*i*w_ +0.2637*j*w_ +691.51*w_;
					

					// interpolating from Logo image
					a = x_ - (int)x_;
					b = y_ - (int)y_;
					nearest_x = (int)x_;
					nearest_y = (int)y_;

					value = (int)(((1-b)*(((1-a)*(int)PuzzleImagedata[nearest_x][nearest_y][k]) + (a*(int)PuzzleImagedata[nearest_x+1][nearest_y][k]))) + b*(((1-a)*(int)PuzzleImagedata[nearest_x][nearest_y+1][k]) + (a*(int)PuzzleImagedata[nearest_x+1][nearest_y+1][k])));
					
					if(value >2 && value !=255){
						NewImagedata[i][j][k] = value;
						
					}
					else{
						NewImagedata[i][j][k] = Imagedata[i][j][k];
					}
					
				}

				// copying non trapezoid values
				else{
					NewImagedata[i][j][k] = Imagedata[i][j][k];
				}
			
			}
		}
	}


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(NewImagedata, sizeof(unsigned char), Sizew*Sizeh*BytesPerPixel, file);
	fclose(file);

	return 0;
}
