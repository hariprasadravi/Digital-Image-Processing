// EE569 Homework Assignment #2
// Date:     Oct 9, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu


#include "Shrink.h"



using namespace std;


// Function to convert Binary to Integer 

int binary2int(vector<int> array){
	int integer_value = 0;
	for(int i=0;i<9;i++){
		integer_value+=pow(2,8-i)*array[i];
	}
	return integer_value;
}



//--------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;

	int Sizeh = 256;
	int Sizew = 256;


	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [Width = 256] [Height = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		 // default is grey image
	} 
	else {
		
		// Check if size is specified
		if (argc >= 2){
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

	// Inverting object and background

	unsigned char NewImagedata[Sizeh][Sizew];
	for(int i=0;i<Sizeh;i++){
		for(int j=0;j<Sizew;j++){
			if((int)Imagedata[i][j] == 0){
				NewImagedata[i][j] = 1;   // Background is object
			}
			else{
				NewImagedata[i][j] = 0; /// Butterfly is background
			}
		}
	}

	// -----------------------------------------------------------------------------------------------------------



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
			G_matrix[i][j] = NewImagedata[i][j];
		}
	}

	//---------------------------------------------------------------

	unsigned char M_matrix[Sizeh][Sizew];

	vector<int> block_matrix;
		

	unsigned char initial[Sizeh][Sizew];

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

				if((int)G_matrix[i][j] == 1){						// checking if center is 1

					for(int l=-1;l<2;l++){							// 3 x 3 window


							for(int k=-1;k<2;k++){

								block_matrix.push_back(G_matrix[i+l][j+k]);	
							}
					}

					if(shrink_1[binary2int(block_matrix)] == true){ 	// Hit

						M_matrix[i][j] = 1;               				// Placing 1
					}
					else{
																		// Miss
						M_matrix[i][j] = 0;								// Placing 0
					}

					block_matrix.clear();

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
				if((int)M_matrix[i][j] == 1){						// check for 1 at centre
					
					for(int l=-1;l<2;l++){							/// 3 x 3 window

							for(int k=-1;k<2;k++){

								block_matrix.push_back(M_matrix[i+l][j+k]);	
							}
					}

					if(shrink_2[binary2int(block_matrix)] == 0){  				// Miss
						
						G_matrix[i][j] = 0;   							 // Replacing with background
					}
					else{												// Hit
						G_matrix[i][j] = 1; 							// Preserve 1	
					}


					block_matrix.clear();


				}

			}
		}
		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){
				iter+= initial[i][j] - G_matrix[i][j];							// checking for change in generated output
			}
		}

	}


	///////////// Shrinking completed ////////////////////


	// ----------------------------------------------------------------------------------------------------------



	/// Hole Filling //////////////
	
	// Discarding the Boundary pixels due to shrinking of background and using new matrix

	unsigned char new_G[Sizeh][Sizew]={0};
	for(int i=10;i<Sizeh-1;i++){
		for(int j=16;j<Sizew-1;j++){
			if((int)G_matrix[i][j] == 1 && (int)G_matrix[i][j-1] == 0 && (int)G_matrix[i][j+1] == 0 && (int)G_matrix[i+1][j] == 0 && (int)G_matrix[i+1][j+1] == 0 && (int)G_matrix[i+1][j-1] == 0 && (int)G_matrix[i-1][j] == 0 && (int)G_matrix[i-1][j+1] == 0 && (int)G_matrix[i-1][j-1] == 0){
				new_G[i][j] = 1;

			}
			else{
				new_G[i][j] = 0;
			}
 
		}
	}

	// --------------------------------------------------------------------------------------------------------------------------

	// Binary Mask for Hole Filling 
	int B[3][3] = {
		{0,1,0},
		{1,1,1},
		{0,1,0}
	};

	int overlap =0;
	iter =1;

	// Intiating hole filling 
	while (iter != 0){
		iter =0;

		// Storing intial matrix
		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){
				initial[i][j] = new_G[i][j];
			}
		}

		// Checking overlap with B matrix
		for(int i=1;i<Sizeh-1;i++){
			for(int j=1;j<Sizew-1;j++){
				for(int a = -1;a<2;a++){
					for(int b=-1;b<2;b++){
						overlap += B[a+1][b+1]*new_G[i+a][j+b];
					}
				}
				if(overlap > 0 && Imagedata[i][j] == 0){      			// Overlap present and intersection with A complement
					new_G[i][j] = 1;
				} 
				overlap =0;												// Clear overlap for next iteration
			}
		}

		// check for change in generated output

		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){
				iter+= initial[i][j] - new_G[i][j];
			}
		}
		// next iteration

	}
	


	// Union with A 

	for(int i=0;i<Sizeh;i++){
		for(int j=0;j<Sizew;j++){
			new_G[i][j]+=Imagedata[i][j];
 
		}
	}	


	////// Hole filling completed /////////// ------------------------------------------------------------------------------------


		// Median filter
	unsigned char newFiltered_Image[Sizeh][Sizew];
	vector<int> v;
	for(int k=0;k<1;k++){
		for(int l=2;l< Sizeh-2;l++){
			for(int w=2;w< Sizew-2;w++){

				// 5 x 5 filter
				for(int i=-2;i<3;i++){
					for(int j=-2;j<3;j++){
						v.push_back(new_G[i+l][j+w]);


					}
				}
				// sorting
				sort(v.begin(),v.end());
				// median
				newFiltered_Image[l][w] = (unsigned char)v[12];
				v.clear();
			}
		}


		// copying for cacading filter
		for(int l=0;l< Sizeh;l++){
			for(int w=0;w< Sizew;w++){
				new_G[l][w] = newFiltered_Image[l][w];
			}
		}

	}


	// converting to 8 bit representation for viewing 	
	for(int i=0;i<Sizeh;i++){
		for(int j=0;j<Sizew;j++){
			if((int)new_G[i][j] == 0){
				new_G[i][j]= 0; /// black
			}
			else
				new_G[i][j] = 255;    /// white
 
		}
	}


	//Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(new_G, sizeof(unsigned char), (Sizew)*(Sizeh), file);
	fclose(file);

	return 0;
}
