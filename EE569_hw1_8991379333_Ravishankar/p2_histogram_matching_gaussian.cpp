
// EE569 Homework Assignment #1
// Date:     Sep 18, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
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
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Width = 256] [Height =256]" << endl;
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



	unsigned char NewImagedata[Sizeh][Sizew][BytesPerPixel];

	//////////// Evaluating the threshold based on Gaussian Equation /////////////
	int bucket[256][BytesPerPixel] ={0};
	int p=0;
	double threshold[256] = {0};
	double sum1=0;
	for(int i=0;i<256;i++){
		threshold[i] =  (1/(sqrt(2*pi*20))) * (exp((-1 * pow((i -70),2.0))/(2*pow(20.0,2.0))));
		
		sum1+=threshold[i];

	}
	//////////// Renormalizing ////////////////////////////
	int newsum=0;
	for(int i=0;i<256;i++){

		threshold[i]/=sum1;
		threshold[i]*=Sizew*Sizeh;
		threshold[i] = (int)threshold[i];
		newsum+=(int)threshold[i];

	}

	/////////// Filling the Buckets //////////////
	for(int l = 0;l<BytesPerPixel;l++){
	for(int k = 0;k<256;k++){
		for(int i=0;i<Sizew;i++){
			for(int j = 0;j<Sizeh;j++){
				if((int)Imagedata[j][i][l]==k && bucket[p][l] < threshold[p]){
					bucket[p][l]++;
					NewImagedata[j][i][l] = (unsigned char)p;

				}
				else if((int)Imagedata[j][i][l] == k && bucket[p][l]>= threshold[p] && p<255){
					p++;
					bucket[p][l]++;
					NewImagedata[j][i][l] = (unsigned char)p;
				}
				else if((int)Imagedata[j][i][l] == k && p==255){
					bucket[p][l]++;
					NewImagedata[j][i][l] = (unsigned char)p;
				}

			}
		}
	}
	
	p=0;

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