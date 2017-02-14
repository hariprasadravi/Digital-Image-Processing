// EE569 Homework Assignment #2
// Date:     Oct 9, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;


// function to construct Bayes index matrix


vector<vector<int> > dithering(vector<vector<int> > I,int size){
	vector<vector<int> > I_1(size, vector<int>(size));
	int inc=0;
	for(int k=0;k<=size/2;k+=size/2){
		for(int l=0;l<=size/2;l+=size/2){

			if(k==0 && l==0){
				inc =0;							// first quadrant
			}
			else if(k==0 && l==size/2){
				inc = 2;						// second quadrant
			}		
			else if(k==size/2 && l==0){
				inc = 3;						// third quadrant
			}
			else{
				inc=1;							// fourth quadrant
			}
			for(int i=0;i<(size/2);i++){
				for(int j=0;j<(size/2);j++){
					I_1[i+k][j+l] = 4*I[i][j] + inc;
				}
			}
		}
	}

	return(I_1);
	
}

//------------------------------
int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Sizeh = 256;
	int Sizew = 256;
	int N =	2; // Default
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Width = 256] [Height =256] [N- enter 0 for A4]" << endl;
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
			N = atoi(argv[6]);
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

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	

	vector<vector<int> > I;

		I = {
			{0,2},
			{3,1}
		};
		
	if(N ==2){
		I = {
			{0,2},
			{3,1}
		};
	 }
	else if(N>2){
		for(int i=4;i<=N;i*=2){
			I= dithering(I,i);
		}
	}
	else if(N ==0){
		N = 4;
		I ={
			{14,10,11,15},
			{9,3,0,4},
			{8,2,1,5},
			{13,7,6,12}
		};
	}


	// -----------------------------------------------------------------------------------------------

	// Threshold matrix computation
	double T[3][N][N];
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			
			T[0][i][j] = ((I[i][j]+ 0.5) / (N*N)) * 85;
			T[1][i][j] = ((I[i][j]+ 0.5) / (N*N)) * 170;
			T[2][i][j] = ((I[i][j]+ 0.5) / (N*N)) * 255;
		}
		cout<<endl;
	}


	// --------------------------------------------------------------------------------
	//  range based thresholding
	unsigned char NewImagedata[Sizeh][Sizew][BytesPerPixel];

	for(int k=0;k<BytesPerPixel;k++){
		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){
				if((int)Imagedata[i][j][k] >= 0 && (int)Imagedata[i][j][k] < T[0][i%N][j%N]){
					NewImagedata[i][j][k] = 0;
				}
				else if((int)Imagedata[i][j][k] >= T[0][i%N][j%N] && (int)Imagedata[i][j][k] < T[1][i%N][j%N]){
					NewImagedata[i][j][k] = 85;
				}
				else if((int)Imagedata[i][j][k] >= T[1][i%N][j%N] && (int)Imagedata[i][j][k] < T[2][i%N][j%N]){
					NewImagedata[i][j][k] = 170;
				}
				else if((int)Imagedata[i][j][k] >= T[2][i%N][j%N]){
					NewImagedata[i][j][k] = 255;
				}
			}
		}
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(NewImagedata, sizeof(unsigned char), Sizeh*Sizew*BytesPerPixel, file);
	fclose(file);

	return 0;
}
