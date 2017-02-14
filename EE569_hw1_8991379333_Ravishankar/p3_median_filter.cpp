
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

	/////// Padding the Image with 1 pixel on all edges //////////////


	unsigned char Padded_Image[Sizeh+2][Sizew+2][BytesPerPixel] ={0};
	for(int i =0; i<(Sizew); i++){
		for(int j=0;j<(Sizeh);j++){
			for(int k=0;k<BytesPerPixel;k++){
				Padded_Image[j+1][i+1][k]= Imagedata[j][i][k];
					if(j==0)
						Padded_Image[j][i+1][k] = Imagedata[j][i][k];
					else if(i==0){
						Padded_Image[j+1][i][k] = Imagedata[j][i][k];

					}
					else if(i == (Sizew-1)){
						Padded_Image[j+1][i+2][k] = Imagedata[j][i][k];
					}
					else if(j == (Sizeh-1)){
						Padded_Image[j+2][i+1][k] = Imagedata[j][i][k];
					}
					
					if(i==0 && j==0)
						Padded_Image[j][i][k] = (Imagedata[j+1][i][k] + Imagedata [j+1][i+1][k] + Imagedata[j][i][k])/3;
					else if(i==0 && j==Sizeh-1)
						Padded_Image[j+2][i][k] = (Imagedata[j][i+1][k] + Imagedata [j-1][i][k] + Imagedata[j-1][i+1][k])/3;
					else if(j==0 && i== Sizew-1)
						Padded_Image[j][i+2][k] = (Imagedata[j+1][i][k] + Imagedata [j][i-1][k] + Imagedata[j+1][i-1][k])/3;
					else if(j==Sizeh-1 && i== Sizew-1)
						Padded_Image[j+2][i+2][k] = (Imagedata[j][i-1][k] + Imagedata [j-1][i][k] + Imagedata[j-1][i-1][k])/3;

			}
		
		}
	}


	// Median filter
	unsigned char newFiltered_Image[Sizeh][Sizew][BytesPerPixel];
	vector<int> v;
	for(int k=0;k<3;k++){
		for(int l=0;l< Sizew;l++){
			for(int w=0;w< Sizeh;w++){
				for(int i=0;i<3;i++){
					for(int j=0;j<3;j++){
						v.push_back(Padded_Image[i+l][j+w][k]);


					}
				}
				// sorting
				sort(v.begin(),v.end());
				// median
				newFiltered_Image[l][w][k] = (unsigned char)v[4];
				v.clear();
			}
		}
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(newFiltered_Image, sizeof(unsigned char), (Sizew)*(Sizeh)*BytesPerPixel, file);
	fclose(file);
	return 0;
}