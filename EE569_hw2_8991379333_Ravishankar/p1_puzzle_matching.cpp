

// EE569 Homework Assignment #2
// Date:     Oct 9, 2016
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
	int Sizeh = 256;
	int Sizew = 256;
	int puzzle_w,puzzle_h;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Width = 256] [Height = 256] [Puzzle Image] [Width = 256] [Height = 256]" << endl;
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

	/// variables to capture coordinates 
	vector<int> x1,y1,x2,y2;
	
	for(int i = 1; i<puzzle_h-1;i++){
		for(int j=1;j<puzzle_w -1;j++){

			// left corners
			if((int)PuzzleImagedata[i][j][0] == 255 && (int)PuzzleImagedata[i][j][1] == 255 && (int)PuzzleImagedata[i][j][2] == 255 && (int)PuzzleImagedata[i][j-1][0] != 255 && (int)PuzzleImagedata[i][j-1][1] != 255 && (int)PuzzleImagedata[i][j-1][2] != 255){
				x1.push_back(i);
				y1.push_back(j);

			}

			// right corners
			else if((int)PuzzleImagedata[i][j][0] == 255 && (int)PuzzleImagedata[i][j][1] == 255 && (int)PuzzleImagedata[i][j][2] == 255 && (int)PuzzleImagedata[i][j+1][0] != 255 && (int)PuzzleImagedata[i][j+1][1] != 255 && (int)PuzzleImagedata[i][j+1][2] != 255){
				x2.push_back(i);
				y2.push_back(j);
			}
		}
	}

	cout<<"The four co-ordinates in puzzle image in clockwise order are (" <<x1[0]<<","<<y1[0]<<") , ("<<x2[0]<<","<<y2[0]<<") , ("<<x2.back()<<","<<y2.back()<<") , ("<<x1.back()<<","<<y1.back()<<")"<<endl;


	// ______________________________________________________________________________________________________________
	int n_x1[4];
	int n_y1[4];
	int n_x2[4];
	int n_y2[4];

	float average =0;
	int flag =0;
	double sum2 =0;
	int save_i,count=0;

	// Scanning column-wise 

	for(int i=0;i<Sizeh;i++){
		count =0;
		for(int j=0;j<Sizew;j++){

			// checking for first occurance of non 255 pixel
			if((int)Imagedata[i][j][0] != 255 && (int)Imagedata[i][j][1] != 255 && (int)Imagedata[i][j][2] != 255 && flag==0){

				// saving coordinate location

				n_x1[0] = i;
				n_y1[0] = j;
				flag =1;
	

				//point 1 for Hillary

			}

			// checking for row of white pixels

			else if((int)Imagedata[i][j][0] == 255 && (int)Imagedata[i][j][1] == 255 && (int)Imagedata[i][j][2] == 255 && flag==1){
				count++;
				save_i = i;
			}

			// checking for non 255 pixels that are less than 175

			else if(flag == 2 && (int)Imagedata[i][j][0] <= 175 && (int)Imagedata[i][j][1] <= 170 && (int)Imagedata[i][j][2] <= 170){

					flag =3;
		
					n_x2[0] = i;
					n_y2[0] =j;

					// point 3 for Trump
			}

			// checking for row of white pixels separating Hillary and Trump images 
			else if((int)Imagedata[i][j][0] == 255 && (int)Imagedata[i][j][1] == 255 && (int)Imagedata[i][j][2] == 255 && flag==3){
				count++;
				save_i = i;
			}
		}

		// Stepping one row back to search for first non 255 pixel for second coordinate location
		if(count == 500 && flag == 1){
			for(int j=0;j<Sizew-1;j++){
				if((int)Imagedata[save_i - 1][j][0] != 255 && (int)Imagedata[save_i - 1][j][1] != 255 && (int)Imagedata[save_i - 1][j][2] != 255 && (int)Imagedata[save_i - 1][j+1][0] == 255 && (int)Imagedata[save_i - 1][j+1][1] == 255 && (int)Imagedata[save_i - 1][j+1][2] == 255){

					flag =2;

					n_x1[1] = save_i -1;
					n_y1[1] = j;
					// point 3 for hillary
				}
			}
		}

		// repeatinf for Trump image
		if(count == 500 && flag == 3){

			for(int j=0;j<Sizew-1;j++){

				if((int)Imagedata[save_i - 2][j][0] <=200  && (int)Imagedata[save_i - 2][j][1] < 140 && (int)Imagedata[save_i - 2][j][2] <= 120){			

					flag = 4;
					n_x2[1] = save_i -2;
					n_y2[1] = j;
					break;
					// point 1 for Trump
				}
			}
		}

	}



	//-----------------------------------------------------------------------------------------------------------------------------------------
	///////// Scanning Row-wise //////////////////
	
	
	flag =0;
	
	int save_j;
	count=0;
	for(int j=0;j<Sizew;j++){
		count =0;
		for(int i=0;i<Sizeh;i++){

			// checking for first occurance of non 255 pixel
			if((int)Imagedata[i][j][0] != 255 && (int)Imagedata[i][j][1] != 255 && (int)Imagedata[i][j][2] != 255 && flag==0){


				flag =1;

				// saving coordinate location
				n_x1[2] = i;
				n_y1[2] = j;

				//point 4 for Hillary

			}
			// checking for column of white pixels
			else if((int)Imagedata[i][j][0] == 255 && (int)Imagedata[i][j][1] == 255 && (int)Imagedata[i][j][2] == 255 && flag==1){
				count++;
				save_j = j;
			}


			// checking for non 255 pixels that are less than 175
			else if(flag == 2 && (int)Imagedata[i][j][0] <= 210 && (int)Imagedata[i][j][1] <= 170 && (int)Imagedata[i][j][2] <= 170){

					flag =3;
		
					n_x2[2] = i;
					n_y2[2] = j;

					// point 2 for Trump
			}

			// checking for column of white pixels separating Hillary and Trump images 			
			else if((int)Imagedata[i][j][0] == 255 && (int)Imagedata[i][j][1] == 255 && (int)Imagedata[i][j][2] == 255 && flag==3){
				count++;
				save_j = j;
			}
		}

		// Stepping one column back to search for first non 255 pixel for second coordinate location
		if(count == 500 && flag == 1){
			for(int i=0;i<Sizeh-1;i++){
				if((int)Imagedata[i][save_j-1][0] != 255 && (int)Imagedata[i][save_j-1][1] != 255 && (int)Imagedata[i][save_j-1][2] != 255 && (int)Imagedata[i+1][save_j-1][0] == 255 && (int)Imagedata[i+1][save_j-1][1] == 255 && (int)Imagedata[i+1][save_j-1][2] == 255){

					flag =2;

					n_x1[3] = i;
					n_y1[3] = save_j - 1;
					// point 2 for hillary
				}
			}
		}
		// repeating for Trumo
		if(count == 500 && flag == 3){

			for(int i=0;i<Sizeh-1;i++){

				if((int)Imagedata[i][save_j-2][0] <=200  && (int)Imagedata[i][save_j-2][1] < 140 && (int)Imagedata[i][save_j-2][2] <= 120 && (int)Imagedata[i+1][save_j-2][0] ==255  && (int)Imagedata[i+1][save_j-2][1] == 255 && (int)Imagedata[i+1][save_j-2][2] == 255){


					flag = 4;
					n_x2[3] = i;
					n_y2[3] = save_j - 2;
					break;
					// point 4 for Trump
				}
			}
		}

	}

	cout<<"The four co-ordinates for Hillary in clockwise mapping order are (" <<n_x1[0]<<","<<n_y1[0]<<") , ("<<n_x1[3]<<","<<n_y1[3]<<") , ("<<n_x1[1]<<","<<n_y1[1]<<") , ("<<n_x1[2]<<","<<n_y1[2]<<")"<<endl;


	cout<<"The four co-ordinates for Trump in clockwise mapping order are (" <<n_x2[1]<<","<<n_y2[1]<<") , ("<<n_x2[2]<<","<<n_y2[2]<<") , ("<<n_x2[0]<<","<<n_y2[0]<<") , ("<<n_x2[3]<<","<<n_y2[3]<<")"<<endl;

	// ------------------------------------------------------------------------------------------------------------------------------------------------
	// Initialize output file
	unsigned char NewImagedata[puzzle_h][puzzle_w][BytesPerPixel];
	double x_,y_,a,b;
	int nearest_x,nearest_y;
	string trump = argv[6];
	for(int k=0;k<BytesPerPixel;k++){
		for(int i=0;i<puzzle_h;i++){
			for(int j=0;j<puzzle_w;j++){
				if((int)PuzzleImagedata[i][j][0] == 255 && (int)PuzzleImagedata[i][j][1] == 255 && (int)PuzzleImagedata[i][j][2] == 255){


					// // Parameters for Hillary
					x_ = 1.4596*i + 0.3889*j -207.0732;
					y_ = -0.3939*i + 1.4646*j -104.2020;



					//Parameters for Trump
					if(trump == "Trump.raw"){

						x_ = -0.0657*i -0.7424*j + 506.7601;
						y_ = 0.7323*i -0.0657*j + 147.6237;
					}


					// interpolation
					a = x_ - (int)x_;
					b = y_ - (int)y_;
					nearest_x = (int)x_;
					nearest_y = (int)y_;

						
					NewImagedata[i][j][k] = (unsigned char)(int)(((1-b)*(((1-a)*(int)Imagedata[nearest_x][nearest_y][k]) + (a*(int)Imagedata[nearest_x+1][nearest_y][k]))) + b*(((1-a)*(int)Imagedata[nearest_x][nearest_y+1][k]) + (a*(int)Imagedata[nearest_x+1][nearest_y+1][k])));

				}
				else{
					NewImagedata[i][j][k] = PuzzleImagedata[i][j][k];
				}
			}
		}
	}
	// ----------------------------------------------------------------------------------------------------------------------------------------
	/// Applying 3 x 3 median filter on the boundary to smoothen boundary 
	vector<int> v;
	for(int k=0;k<BytesPerPixel;k++){
		for(int i=0;i<puzzle_h;i++){
			for(int j=0;j<puzzle_w;j++){

				if((i == x1[0] && j>= y1[0] && j<= y2[0]) || (j== y2[0] &&  i>= x2[0] && i<= x2.back()) || (i==x2.back() && j>= y1.back() && j<= y2.back()) || (j== y1.back() && i>=x1[0] && i<=x1.back())){
					
					for(int a = -2;a<3;a++){
						for(int b=-2;b<3;b++){
							v.push_back(NewImagedata[i+a][j+b][k]);
							
							
						}
					}
					sort(v.begin(),v.end());
					NewImagedata[i][j][k]=(unsigned char)v[12];
					
					v.clear();
				}
			}
		}
	}


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(NewImagedata, sizeof(unsigned char), puzzle_w*puzzle_h*BytesPerPixel, file);
	fclose(file);

	return 0;
}
