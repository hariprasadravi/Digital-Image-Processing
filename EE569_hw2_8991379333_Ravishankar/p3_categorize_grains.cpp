// EE569 Homework Assignment #2
// Date:     Oct 9, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu

#include "Thinning.h"
#include <string>

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

	int Sizeh = 256;
	int Sizew = 256;


	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw [Width = 256] [Height = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		 // default is grey image
	} 
	else {
		
		// Check if size is specified
		if (argc >= 2){
			Sizew = atoi(argv[2]);
			Sizeh = atoi(argv[3]);
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

	// candidate coordinates for grains of each type 
	int coord[11][2]={
		{12,48},
		{85,314},
		{15,542},
		{140,97},
		{162,294},
		{196,529},
		{355,57},
		{346,318},
		{288,531},
		{392,160},
		{426,396},
	};

	unsigned char CroppedImages[58][45][11];
	

	// Cropping images
	for(int i=0;i<11;i++){
		
		for(int h=0;h<58;h++){
			for(int w=0;w<45;w++){
				
				CroppedImages[h][w][i] = Imagedata[(coord[i][0] + h)][(coord[i][1] + w)];
			}
		}
	}
	///-------------------------------------------------------------------------------------------

	// Converting image to binary representation for processing ///

	vector< pair<float,int> > total_area;
	float area = 0;
	for(int grain=0;grain<11;grain++){
		for(int i=0;i<58;i++){
			for(int j=0;j<45;j++){
				if((int)CroppedImages[i][j][grain] == 0){
					CroppedImages[i][j][grain] = 0;   					// Converting to Binary
				}
				else{
					area++;												// counting white pixels
					CroppedImages[i][j][grain] = 1; 
				}
			}
		}


	total_area.push_back(make_pair(area,grain));						// pushing area of each grain to a vector
	area=0;
	}


	//--------------------------------------------------------------------------------

	////// thinning /////////////////



	
	// Hashmap to map all binary masks to 1 for stage 1 filter
	unordered_map<int,bool> thin_1;
	
	for(int i=0;i<46;i++){


		thin_1[binary2int(Thinning_stage1[i])] = true;
	}

	// Hashmap to map all binary masks to 1 for stage 2 filter
	unordered_map<int,bool> thin_2;
	
	for(int i=0;i<332;i++){
		
		thin_2[binary2int(Thinning_stage2[i])] = true;
	}


	// redefining new size 

	Sizeh = 58;
	Sizew = 45;

	unsigned char G_matrix[Sizeh][Sizew];
	unsigned char temp_matrix[Sizeh][Sizew];		
	unsigned char M_matrix[Sizeh][Sizew];
	int len = 0;
	vector< pair<int,int> > length;
	vector<int> block_matrix;
		
	long int iter =1;
	unsigned char initial[Sizeh][Sizew];

	for(int grain=0;grain<11;grain++){


		// Pre-assigning G matrix to image data

		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){
				G_matrix[i][j] = CroppedImages[i][j][grain];
				temp_matrix[i][j] = CroppedImages[i][j][grain] * 255;			// for writing cropped image file
			}
		}
		


		// Generating unique file name
		string filename = "Rice_Grain_binary "+ to_string(grain+1) + ".raw";

		file=fopen(filename.c_str(),"wb");

		// write grain image to file	
		fwrite(temp_matrix, sizeof(unsigned char), (Sizew)*(Sizeh), file);
		fclose(file);

		// --------------------------------------------------------------------------------

		/// Generate stage 1 output //

		iter = 1;
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

					if((int)G_matrix[i][j] == 1){			// checking if centre is 1

						for(int l=-1;l<2;l++){				// 3x3 window


								for(int k=-1;k<2;k++){

									block_matrix.push_back(G_matrix[i+l][j+k]);	
								}
						}

						if(thin_1[binary2int(block_matrix)] == true){ // Hit

							M_matrix[i][j] = 1;               // Placing 1
						}
						else{
															// Miss
							M_matrix[i][j] = 0;				// Placing 0
						}

						block_matrix.clear();

					}
					else{
						M_matrix[i][j] = 0;
					}
				}
			}


			// Stage 2 /////


			for(int i=1;i<Sizeh-1;i++){
				for(int j=1;j<Sizew-1;j++){
					if((int)M_matrix[i][j] == 1){				// checking if centre is 1
						
						for(int l=-1;l<2;l++){

								for(int k=-1;k<2;k++){

									block_matrix.push_back(M_matrix[i+l][j+k]);	
								}
						}

						if(thin_2[binary2int(block_matrix)] == 0){  	// Miss
							
							G_matrix[i][j] = 0;    					// Replacing with background
						}
						else{										// hit
							G_matrix[i][j] = 1; 					//	preserve value
						}


						block_matrix.clear();


					}

				}
			}
			for(int i=0;i<Sizeh;i++){
				for(int j=0;j<Sizew;j++){
					iter+= initial[i][j] - G_matrix[i][j];			// check for iteration condition
				}
			}

		}

		
		///////////// Thinning completed ////////////////////


		// ---------------------------------------------------------------------

		// converting to 8 bit for viewing

		for(int i=0;i<Sizeh;i++){
			for(int j=0;j<Sizew;j++){
				if((int)G_matrix[i][j] == 0){
					G_matrix[i][j]= 0; /// black
				}
				else{
					len++;						// computing length of thinning image
					G_matrix[i][j] = 255;    /// white
				}
	 
			}
		}
		length.push_back(make_pair(len,grain));		// pushing length
		total_area[grain].first/=len;				// compute area/ length

		len =0;									//clear length
		



		//Write image data from image data matrix
		filename = "Thinning_grain "+ to_string(grain+1) + ".raw";

		file=fopen(filename.c_str(),"wb");

		
		fwrite(G_matrix, sizeof(unsigned char), (Sizew)*(Sizeh), file);
		fclose(file);
	}

	cout<<"Ascending order of grain length is as follows "<<endl;
	
	sort(length.begin(), length.end());						// sorting in ascending order

	for(int i = 0; i < length.size(); i++)
	{
     cout <<"Grain category " <<length[i].second + 1 << " - > length = "<<length[i].first<<" pixels"<<endl;
	}

	cout<<"Narrowest to widest grain order based on area / length ratio is as follows "<<endl;
	
	sort(total_area.begin(), total_area.end());					// sorting

	for(int i = 0; i < total_area.size(); i++)
	{
     cout <<"Grain category " <<total_area[i].second + 1 << " - > Area / length = "<<total_area[i].first<<endl;
	}
	return 0;
}