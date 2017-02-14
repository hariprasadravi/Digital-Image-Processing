// EE569 Homework Assignment #2
// Date:     Oct 9, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <unordered_map>
#include <vector>
#include "Shrink.h"

using namespace std;


// function to perform binary to interger conversion

int binary2int(vector<int> array){
	int integer_value = 0;
	for(int i=0;i<9;i++){
		integer_value+=pow(2,8-i)*array[i];
	}
	return integer_value;
}
//------------------------------------------------------------------------------------------------
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
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel] [Width = 256] [Height = 256]" << endl;
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
			Sizew = atoi(argv[3]);
			Sizeh = atoi(argv[4]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata[Sizeh][Sizew];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Sizew*Sizeh, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	// Converting image to binary representation for processing ///

	for(int i=0;i<Sizeh;i++){
		for(int j=0;j<Sizew;j++){
			if((int)Imagedata[i][j] == 0){
				Imagedata[i][j] = 0;   // Converting to Binary
			}
			else{
				Imagedata[i][j] = 1; 
			}
		}
	}	



	////// Shrinking /////////////////



	// Hashmap to map all binary masks to 1 for stage 1 filter
	unordered_map<int,bool> shrink_1;
	
	for(int i=0;i<58;i++){


		shrink_1[binary2int(Shrink_stage1[i])] = true;
	}


	// Hashmap to map all binary masks to 1 for stage 2 filter
	unordered_map<int,bool> shrink_2;
	
	for(int i=0;i<332;i++){
		
		shrink_2[binary2int(Shrink_stage2[i])] = true;
	}


	// Pre-assigning G matrix to image data
	unsigned char G_matrix[Sizeh][Sizew];

	for(int i=0;i<Sizeh;i++){
		for(int j=0;j<Sizew;j++){
			G_matrix[i][j] = Imagedata[i][j];
		}
	}

	//---------------------------------------------------------------
	unsigned char initial[Sizeh][Sizew];
	unsigned char M_matrix[Sizeh][Sizew];

	vector<int> block_matrix;
	// ----------------------------------------------------------------------------------------------------
	/// Generate stage 1 output //

	long int iter =1;
	while (iter != 0){
		iter =0;

		// initializing for condition checking at the end of shrinking

		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){
				initial[i][j] = G_matrix[i][j];
			}
		}


		// Clearing all indices in M Matrix
		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){
				M_matrix[i][j] = 0;
			}
		}


		// Initiating Stage 1 his or miss operation

		for(int i=1;i<Sizeh-1;i++){
			for(int j=1;j<Sizew-1;j++){

				if((int)G_matrix[i][j] == 1){      // checking if center is 1

					for(int l=-1;l<2;l++){			// 3 x 3 window


							for(int k=-1;k<2;k++){

								block_matrix.push_back(G_matrix[i+l][j+k]);	
							}
					}

					if(shrink_1[binary2int(block_matrix)] == true){ 	// Hit

						M_matrix[i][j] = 1;               // Placing 1 at centre
					}
					else{
														// Miss
						M_matrix[i][j] = 0;				// Placing 0 at center
					}

					block_matrix.clear();				// clearing vector for next iteration

				}
				else{
					M_matrix[i][j] = 0;
				}
			}
		}

		// -------------------------------------------------------------------------------------------
		// Stage 2 /////


		for(int i=1;i<Sizeh-1;i++){
			for(int j=1;j<Sizew-1;j++){
				if((int)M_matrix[i][j] == 1){				// check for 1 at centre
					
					for(int l=-1;l<2;l++){					/// 3 x 3 window

							for(int k=-1;k<2;k++){

								block_matrix.push_back(M_matrix[i+l][j+k]);	
							}
					}

					if(shrink_2[binary2int(block_matrix)] == 0){  		// Miss
						
						G_matrix[i][j] = 0;    						// Replacing with background
					}
					else{
						G_matrix[i][j] = 1; 						// preservind value
					}


					block_matrix.clear();						// clearing


				}

			}
		}


		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){
				iter+= initial[i][j] - G_matrix[i][j];				// checking for change in generated output
			}
		}

	}

	// ----------------------------------------------------------------------------------------------------------

	// converting to 8 bit for viewing

	int count = 0;
	for(int i=0;i<Sizeh;i++){
		for(int j=0;j<Sizew;j++){
			if((int)G_matrix[i][j] == 0){
				G_matrix[i][j]= 0; 			/// black
			}
			else{
				count++;
				G_matrix[i][j] = 255;    	/// white
			}
 
		}
	}

	// -------------------------------------------------------------------------

	cout<<"Number of Grains in the image = "<<count<<endl;
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(G_matrix, sizeof(unsigned char), (Sizew)*(Sizeh), file);
	fclose(file);

	return 0;
}
