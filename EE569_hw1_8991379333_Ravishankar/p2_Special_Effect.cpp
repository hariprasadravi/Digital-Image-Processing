
// EE569 Homework Assignment #1
// Date:     Sep 18, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <unordered_map>


using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel_reference, BytesPerPixel_input;
	int Sizeh_reference;
	int Sizew_reference;
	int Sizeh_input,Sizew_input;
	
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name Reference_image [BytesPerPixel = 1] [Width = 256] [Height= 256] input_image [BytesPerPixel = 1] [Width = 256] [Height= 256] output_image" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel_reference = 1; // default is grey image
	} 
	else {
		BytesPerPixel_reference = atoi(argv[2]);
		BytesPerPixel_input = atoi(argv[6]);
		// Check if size is specified
		if (argc >= 5){
			Sizew_reference = atoi(argv[3]);
			Sizeh_reference = atoi(argv[4]);
			Sizew_input = atoi(argv[7]);
			Sizeh_input = atoi(argv[8]);



			
		}
	}
	
	// Allocate image data array for reference

	///// First argument is height and second is width 
	unsigned char Imagedata_reference[Sizeh_reference][Sizew_reference][BytesPerPixel_reference];


	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata_reference, sizeof(unsigned char), Sizeh_reference*Sizew_reference*BytesPerPixel_reference, file);
	fclose(file);

	// Allocate image data array for input

	///// First argument is height and second is width 
	unsigned char Imagedata_input[Sizeh_input][Sizew_input][BytesPerPixel_input];


	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[5],"rb"))) {
		cout << "Cannot open file: " << argv[5] <<endl;
		exit(1);
	}
	fread(Imagedata_input, sizeof(unsigned char), Sizeh_input*Sizew_input*BytesPerPixel_input, file);
	fclose(file);
	
	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	///////// Calculating probabilty function for Reference
	int index=0;

	float count_reference[256][3] = {0};
	for(int k=0;k<BytesPerPixel_reference;k++){
		for(int i = 0; i < Sizew_reference; i++){
			for(int j=0;j<Sizeh_reference;j++){
				index = int(Imagedata_reference[j][i][k]);
				count_reference[index][k]+=1;


			}
		}
	}

	// CDF
	for(int k=0;k<BytesPerPixel_reference;k++){
		for(int i=0;i<256;i++){
			count_reference[i][k]/= (Sizew_reference * Sizeh_reference);
			if(i>=1){
				count_reference[i][k]+=count_reference[i-1][k];

			}
			
		}
	}

	////////

	// Calculating Probability for input

	float count_input[256][3] = {0};

	for(int k=0;k<BytesPerPixel_input;k++){
		for(int i = 0; i < Sizew_input; i++){
			for(int j=0;j<Sizeh_input;j++){
				index = int(Imagedata_input[j][i][k]);
				count_input[index][k]+=1;


			}
		}
	}

	// CDF
	for(int k=0;k<BytesPerPixel_input;k++){
		for(int i=0;i<256;i++){
			count_input[i][k]/= (Sizew_input * Sizeh_input);
			if(i>=1){
				count_input[i][k]+=count_input[i-1][k];

			}
			
		}
	}

	////////////////////////////////////////// Mapping Function ///////////////////////////////////////////////////

	unordered_map<int,unordered_map<int,int>> mymap;
	int j;
	for(int k=0;k<3;k++){
		j=0;
		for(int i = 0;i<256;i++){

			if(count_input[i][k]<= count_reference[j][k]){
				mymap[i][k]=j;
			}
			else{
				while(count_input[i][k] > count_reference[j][k]){
					j++;
				}
				if((count_reference[j][k] - count_input[i][k]) > (count_input[i][k] - count_reference[j-1][k])){
					mymap[i][k] = j-1 ;
				}
				else{
					mymap[i][k] = j;
				}
			}

		}
	}


	////////////////////////////////////// New Image /////////////////////////////////////////////

	unsigned char NewImagedata[Sizeh_input][Sizew_input][BytesPerPixel_input];
	
	for(int l=0;l<256;l++){
		for(int k=0;k< BytesPerPixel_input;k++){
			for(int i=0 ;i <Sizew_input ; i++){
				for(int j=0;j<Sizeh_input;j++){
					if(Imagedata_input[j][i][k] == l){
						NewImagedata[j][i][k] = mymap[l][k]; 
					}

				}
			}
		}
	}	




	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[9],"wb"))) {
		cout << "Cannot open file: " << argv[9] << endl;
		exit(1);
	}
	fwrite(NewImagedata, sizeof(unsigned char), (Sizew_input)*(Sizeh_input)*BytesPerPixel_input, file);
	fclose(file);
	return 0;
}