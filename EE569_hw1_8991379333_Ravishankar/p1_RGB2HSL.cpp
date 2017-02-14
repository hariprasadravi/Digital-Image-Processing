
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
		cout << "program_name input_image.raw H_image.raw S_image.raw L_image.raw [BytesPerPixel = 1] [width = 256] [Height = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[5]);
		// Check if size is specified
		if (argc >= 5){
			Sizew = atoi(argv[6]);
			Sizeh = atoi(argv[7]);
			
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




	/// Implementing the transformation formula //////////////////////////

  	unsigned char HImage[Sizeh][Sizew];
  	unsigned char SImage[Sizeh][Sizew];
  	unsigned char LImage[Sizeh][Sizew];
  	float buffer;

  	double M,m,C;
  	double H,L,S;


			for(int i= 0;i <Sizew ;i++){

				for(int j=0;j < Sizeh;j++){

					
					
						M = max(max(Imagedata[j][i][0],Imagedata[j][i][1]),Imagedata[j][i][2])/255.0;
						m = min(min(Imagedata[j][i][0],Imagedata[j][i][1]),Imagedata[j][i][2])/255.0;
						C= M-m;

						L = (M+m)/2;


						// Calculating S
						if(L==0){
							S=0;
						}
						else if(L > 0 && L < 0.5){
							S = C/(2*L);

						}
						else
							S = C/(2-2*L);


						// Calculating H

						if(C==0){
							
							H=0;
							
						}
						else if(M == Imagedata[j][i][0]/255.0){
							
							H = 60 * fmod((double(Imagedata[j][i][1] - Imagedata[j][i][2])/(C *255)),6.0);
							
						}
						else if(M == Imagedata[j][i][1]/255.0){
							H = 60 * (((Imagedata[j][i][2]-Imagedata[j][i][0])/(C * 255)) + 2);
							
						}
						else if(M == Imagedata[j][i][2]/255.0){
							H = 60 * (((Imagedata[j][i][0] - Imagedata[j][i][1])/(C * 255)) + 4); 

						}
						

						
						
						HImage[j][i] = ((H/360.0)*255.0);
	
						SImage[j][i] = (unsigned char)int(S*255);
						LImage[j][i] = (unsigned char)int(L*255);

					
			

			}
		
		}
	
	

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(HImage, sizeof(unsigned char), (Sizew)*(Sizeh), file);
	fclose(file);

	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(SImage, sizeof(unsigned char), (Sizew)*(Sizeh), file);
	fclose(file);

	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(LImage, sizeof(unsigned char), (Sizew)*(Sizeh), file);
	fclose(file);
	return 0;
}