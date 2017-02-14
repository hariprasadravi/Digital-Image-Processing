
// EE569 Homework Assignment #3
// Date:     Nov 6, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu


#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

Mat PerformFiltering(Mat Image,Mat filter){
    // Boundary extension
    Mat filtered_image = Mat(Image.rows,Image.cols,CV_32FC1,Scalar(0));
    Mat extended_image;
    copyMakeBorder(Image,extended_image,2,2,2,2,BORDER_REFLECT_101);

    // convert from unsigned char to float
    filter.convertTo(filter,CV_32FC1);
    for(int i=2;i<(extended_image.rows-2);i++){
        for(int j=2;j<(extended_image.cols-2);j++){

            // Copy 5x5 moving window
            Mat window = extended_image(cv::Rect(j-2,i-2,5,5)).clone();
            window.convertTo(window,CV_32FC1);
            
            // Compute product
            float prod = (sum(window.mul(filter))[0]);
            
            filtered_image.at<float>(i-2,j-2) = (prod);

        }
    }
    
    filtered_image.convertTo(filtered_image,CV_8UC1);
    return filtered_image;

}

// --------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int NumberOfImages =12;
    int Size = 128;
    string filename;

    // Allocate image data array
    unsigned char Imagedata[NumberOfImages][Size][Size];



    // Read image (filename specified by first argument) into image data matrix
    cv::Scalar temp;
    vector<Mat> Images(12);
  

    // Read all 12 Texture Images
    for(int i=1;i<13;i++){
    	Images[i-1].create(Size,Size, CV_8UC1);
        filename = "Texture"+ to_string(i) +".raw";
        file=fopen(filename.c_str(),"rb");
        fread(Imagedata[i-1], sizeof(unsigned char), Size*Size, file);
        fclose(file);
        memcpy(Images[i-1].data,Imagedata[i-1],Size*Size);


        // Remove DC component for every input image
        temp=cv::mean(Images[i-1]);
        Images[i-1] -=temp;

 
    }	


    // Define Kernels
    vector<vector<float> > mask ={
    	{1,4,6,4,1},
    	{-1,-2,0,2,1},
    	{-1,0,2,0,-1},
    	{-1,2,0,-2,1},
    	{1,-4,6,-4,1},
    };

    // 25 vectors for 25 filters
	vector<Mat> filters(25);
	Mat A,B,B_t;
	A = Mat(5,1,CV_32FC1,Scalar(0));
	A.copyTo(B);
	Mat prod(5,5,CV_32FC1,Scalar(0));
	int level =0;

	// constructing all possible filters ----------------------------------------------------------------------------

    for(int i=0;i<5;i++){
    	
    	memcpy(A.data, mask[i].data(), 5*sizeof(float));

    	for(int j=0;j<5;j++){
    		memcpy(B.data, mask[j].data(), 5*sizeof(float));
    		B_t=B.t();
    		filters[level] = A*B.t();
    		level++;
    		
    	}

    }

    float feature_vector[12][25];

    // filtering using the masks --------------------------------------------------------------------------------------------
    Mat dst;
    for(int i=0;i<12;i++){
    	for(int j=0;j<25;j++){

            // Call function to perform filtering 
    		dst = PerformFiltering(Images[i],filters[j]); 

            // Compute Energy of the filtered output
    		temp =(cv::sum(dst.mul(dst)))/float(Size *Size);

            // Save energ in 2D vector
    		feature_vector[i][j] = float(temp[0]);

    	}


    }
    //--------------------------------------------------------------------------------------------------------------------------
    // normalize feature matrix
    Mat features = Mat(12,25,CV_32FC1,feature_vector);

    Mat min,max;

    // Compute column-wise max and min
    reduce(features,min,0,CV_REDUCE_MIN,-1);
    reduce(features,max,0,CV_REDUCE_MAX,-1);

    // Normalizing
    for(int i=0;i<12;i++){
    	features.row(i) = (features.row(i) - min)/(max - min);
    }

    // Implement PCA --------------------------------------------------------------------------------------------------------------
    Mat output_features;
    PCA pca_analysis(features, Mat(), CV_PCA_DATA_AS_ROW,3);
    pca_analysis.project(features,output_features);

    // implement K-means -------------------------------------------------------------------------------------------------
    Mat labels,centers;
    kmeans(output_features, 4, labels,
            TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 30000, 0.00001),
               10, KMEANS_RANDOM_CENTERS, centers);


    cout<<"Printing the labels"<<endl;
    cout<<labels<<endl;


    return 0;
}

   