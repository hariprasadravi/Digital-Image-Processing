// EE569 Homework Assignment #2
// Date:     Oct 9, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <math.h>
#include <cmath>
using namespace std;



int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	
	int Sizeh1 = 256;
	int Sizeh2 = 256;
	int Sizew1 = 256;
	int Sizew2 = 256;
	int Sizeph = 256;
	int Sizepw = 256;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image1_thinning.raw input_image1_skeleton.raw [Width = 256] [Height = 256] input_image2_thinning.raw input_image2_skeleton.raw [Width = 256] [Height = 256] probe_thinning.raw probe_skeleton.raw [Width = 256] [Height = 256] " << endl;
		return 0;
	}
	
	// Check if image is grayscale or color

	else {

		// Check if size is specified
		if (argc >= 5){
			Sizew1 = atoi(argv[3]);
			Sizeh1 = atoi(argv[4]);
			Sizew2 = atoi(argv[7]);
			Sizeh2 = atoi(argv[8]);
			Sizepw = atoi(argv[11]);
			Sizeph = atoi(argv[12]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata1[2][Sizeh1][Sizew1];

	 // First index is thinning and second index is Skeleton
	// Read image (filename specified by first argument) into image data matrix
	for(int i=0;i<2;i++){
		if (!(file=fopen(argv[i+1],"rb"))) {
			cout << "Cannot open file: " << argv[i+1] <<endl;
			exit(1);
		}
		fread(Imagedata1[i], sizeof(unsigned char), Sizeh1*Sizew1, file);
		fclose(file);
	}

	// Allocate image data array
	unsigned char Imagedata2[2][Sizeh2][Sizew2];

	// Read image (filename specified by first argument) into image data matrix
	for(int i=5;i<7;i++){
		if (!(file=fopen(argv[i],"rb"))) {
			cout << "Cannot open file: " << argv[i] <<endl;
			exit(1);
		}
		fread(Imagedata2[i-5], sizeof(unsigned char), Sizeh2*Sizew2, file);
		fclose(file);
	}

		// Allocate image data array
	unsigned char TestImagedata[2][Sizeph][Sizepw];

	// Read image (filename specified by first argument) into image 	data matrix

	for(int i =9;i<11;i++){
		if (!(file=fopen(argv[i],"rb"))) {
			cout << "Cannot open file: " << argv[i] <<endl;
			exit(1);
		}
		fread(TestImagedata[i-9], sizeof(unsigned char), Sizeph*Sizepw, file);
		fclose(file);
	}

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////



	int count_4[2][3] = {0};		// to count number of 4 connected pixels

	unordered_map<int,unordered_map<int,bool> > skip,skip2,skip3;		// hashmap to skip specific white pixels


	int T_junctions[2][3] = {0};				// number of T junctions
	int endpoints[2][3] = {0};					// number of endpoints
	int count;
	count=0;
	int countend =0;




	////// Butterfly image ///////

	for(int k=0;k<2;k++){
		for(int i=1;i<Sizeh1-1;i++){
			for(int j=1;j<Sizew1-1;j++){
				if((int)Imagedata1[k][i][j] ==255){						// Check for white pixel

					for(int a=-1;a<2;a++){								// 3 x 3 window around white pixel
						for(int b=-1;b<2;b++){


							if((int)Imagedata1[k][i+a][j+b] == 255 && (a !=0 || b!=0) && skip[i+a][j+b] != true){		// condition for T junction

								count++;							// increment counter
								
							}
							if((int)Imagedata1[k][i+a][j+b] == 255 && (a !=0 || b!=0)){		// condition for  endpoints
								countend++;
							}

							if((int)Imagedata1[k][i+a][j+b] == 255 && (a*b ==0)){			// condition for 4 connectivity
								count_4[k][0]++;
							}

						}
					}
				
				}	
				if(count >= 3){											// Three or more  neighbouring pixels
				
					for(int a=0;a<2;a++){								// Assign 1 to those pixels
						for(int b=-1;b<2;b++){

							if((int)Imagedata1[k][i+a][j+b] == 255){

								skip[i+a][b+j] = true;					// true
								
							}
						}
					}

					T_junctions[k][0]++;					// increment no of T junctions
				}
				else if(countend == 1){						// check for 1 neighbouring pixel
					
					endpoints[k][0]++;						// increment endpoint counter
				}
				countend=0;									// clear counters
				count=0;
			}
		}
		skip.clear();								// clear memory

		// Repeat for Skeleton image
	}

	// Repeat Steps for Fly and Probe image


	//////// Fly image ///////////////////



	for(int k=0;k<2;k++){
		for(int i=1;i<Sizeh2-1;i++){
			for(int j=1;j<Sizew2-1;j++){
				if((int)Imagedata2[k][i][j] ==255){

					for(int a=-1;a<2;a++){
						for(int b=-1;b<2;b++){

							if((int)Imagedata2[k][i+a][j+b] == 255 && (a !=0 || b!=0) && skip2[i+a][j+b] != true){
								count++;
							}
							if((int)Imagedata2[k][i+a][j+b] == 255 && (a !=0 || b!=0)){
								countend++;
							}

							if((int)Imagedata2[k][i+a][j+b] == 255 && (a*b ==0)){
								count_4[k][1]++;
							}

						}
					}


				}
				if(count >= 3){

					T_junctions[k][1]++;
					for(int a=0;a<2;a++){	
						for(int b=-1;b<2;b++){

							if((int)Imagedata2[k][i+a][j+b] == 255){

								skip2[i+a][b+j] = true;
								
							}
						}
					}
				}
				else if(countend == 1){

					endpoints[k][1]++;
				}
				countend=0;
				count=0;
			}
		}	
		skip2.clear();
	}

	////////// Probe Image ///////////////////


	for(int k=0;k<2;k++){
		for(int i=1;i<Sizeph-1;i++){
			for(int j=1;j<Sizepw-1;j++){
				if((int)TestImagedata[k][i][j] ==255){

					for(int a=-1;a<2;a++){
						for(int b=-1;b<2;b++){

							if((int)TestImagedata[k][i+a][j+b] == 255 && (a !=0 || b!=0) && skip3[i+a][j+b] != true){
								count++;
							}
							if((int)TestImagedata[k][i+a][j+b] == 255 && (a !=0 || b!=0)){
								countend++;
							}

							if((int)TestImagedata[k][i+a][j+b] == 255 && (a*b ==0)){
								count_4[k][2]++;
							}

						}
					}

				}
				if(count >= 3){

					for(int a=0;a<2;a++){	
						for(int b=-1;b<2;b++){

							if((int)TestImagedata[k][i+a][j+b] == 255){

								skip3[i+a][b+j] = true;
								
							}
						}
					}
					T_junctions[k][2]++;
		
				}
				else if(countend == 1){
					endpoints[k][2]++;

				}
				countend=0;
				count=0;
			}
		}
		skip3.clear();
	}




	// Print Featues for Thinning image
	cout<<"Features of Thinning images "<<endl;
	cout<<"\t\tT Junctions\t\tEnd Points\t4 Connectivity"<<endl;
	cout<<"Butterfly\t\t"<<T_junctions[0][0]<<"\t\t"<<endpoints[0][0]<<"\t\t"<<count_4[0][0]<<endl;
	cout<<"Fly\t\t\t"<<T_junctions[0][1]<<"\t\t"<<endpoints[0][1]<<"\t\t"<<count_4[0][1]<<endl;
	cout<<"Probe\t\t\t"<<T_junctions[0][2]<<"\t\t"<<endpoints[0][2]<<"\t\t"<<count_4[0][2]<<endl;

	cout<<endl<<endl;



	// print for Skeleton image
	cout<<"Features of Skeleton images "<<endl;
	cout<<"\t\tT Junctions\t\tEnd Points\t4 Connectivity"<<endl;
	cout<<"Butterfly\t\t"<<T_junctions[1][0]<<"\t\t"<<endpoints[1][0]<<"\t\t"<<count_4[1][0]<<endl;
	cout<<"Fly\t\t\t"<<T_junctions[1][1]<<"\t\t"<<endpoints[1][1]<<"\t\t"<<count_4[1][1]<<endl;
	cout<<"Probe\t\t\t"<<T_junctions[1][2]<<"\t\t"<<endpoints[1][2]<<"\t\t"<<count_4[1][2]<<endl;





	// Classifier condition
	
	if((abs(endpoints[0][2] - endpoints[0][0]) < abs(endpoints[0][2] - endpoints[0][1])) && (abs(T_junctions[1][2]/(float)T_junctions[0][2] - T_junctions[1][0]/(float)T_junctions[0][0]) < abs(T_junctions[1][2]/(float)T_junctions[0][2] - T_junctions[1][1]/(float)T_junctions[0][1]) ) && (abs(count_4[1][2]/(float)count_4[0][2] - count_4[1][0]/(float)count_4[0][0]) < abs(count_4[1][2]/(float)count_4[0][2] - count_4[1][1]/(float)count_4[0][1]) )){ 

		cout<<"Probe image belongs to Class - Butterfly"<<endl;
	}
	else{
		cout<<"Probe image belongs to Class - Fly"<<endl;
	}







	return 0;
}
