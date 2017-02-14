

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


// Function to compute energy of all pixels in a filtered image
vector<double> GetFeatures(Mat filtered_image, int windowSize){
    vector<double> energy;
    Mat extended_image;
    copyMakeBorder(filtered_image,extended_image,windowSize/2,windowSize/2,windowSize/2,windowSize/2,BORDER_REFLECT_101);
    for(int i=windowSize/2;i<extended_image.rows -windowSize/2;i++){
        for(int j=windowSize/2;j<extended_image.cols-windowSize/2;j++){
            Mat window = extended_image(cv::Rect(j-windowSize/2,i-windowSize/2,windowSize,windowSize)).clone();
            
            window.convertTo(window,CV_32FC1);

            energy.push_back(sum(window.mul(window))[0]/(float)windowSize*windowSize);
        }
    }

    return energy;
}
// -------------------------------------------------------------------------------------------------------------------------------

// Function to perform filtering
Mat PerformFiltering(Mat Image,Mat filter){
    // Boundary extension
    Mat filtered_image = Mat(Image.rows,Image.cols,CV_32FC1,Scalar(0));
    Mat extended_image;
    copyMakeBorder(Image,extended_image,2,2,2,2,BORDER_REFLECT_101);


    //Convert from unsigned char to float for computation
    filter.convertTo(filter,CV_32FC1);
    for(int i=2;i<(extended_image.rows-2);i++){
        for(int j=2;j<(extended_image.cols-2);j++){

        	// Copy a moving 5x5 window
            Mat window = extended_image(cv::Rect(j-2,i-2,5,5)).clone();
            window.convertTo(window,CV_32FC1);
            

            // Perform convolution
            double prod = (sum(window.mul(filter))[0]);
            
            filtered_image.at<float>(i-2,j-2) = (prod);

        }
    }
    
    filtered_image.convertTo(filtered_image,CV_8UC1);
    return filtered_image;

}
//------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;

    int BytesPerPixel = 3;
    int Sizeh = 256;
    int Sizew = 256;


    // Check for proper syntax
    if (argc < 5){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw [BytesPerPixel] [Width = 256] [Height = 256]" << endl;
        return 0;
    }
    
  
    BytesPerPixel = atoi(argv[2]);
    Sizew = atoi(argv[3]);
    Sizeh = atoi(argv[4]);

    // Allocate image data array
    unsigned char Imagedata[Sizeh][Sizew];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Sizew*Sizeh, file);
    fclose(file);
    
    // --------------------------------------------------------------------------------------------------
    Mat Image = Mat(Sizeh,Sizew,CV_8UC1,cv::Scalar(0));
    memcpy(Image.data,Imagedata,Sizeh*Sizew);

    // Remove DC component
    Image = Image - cv::mean(Image);
    
    // Define filter kernels
    vector<vector<float> > mask ={
    	{1,4,6,4,1},
    	{-1,-2,0,2,1},
    	{-1,0,2,0,-1},
    	{-1,2,0,-2,1},
    	{1,-4,6,-4,1},
    };

    // 25 filters as vectors
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

    // filtering using the masks --------------------------------------------------------------------------------------------
    Mat dst;
    Mat filtered_image;
    vector<vector<double> > features;
	for(int j=0;j<25;j++){

        // Perform filtering of the image

	    filtered_image = PerformFiltering(Image,filters[j]);


	    // Save the filtered image
        imwrite("Filtered_Image_" + to_string(j) + ".jpg", filtered_image);


        // compute energy feature for every pixel as linear vector
        features.push_back(GetFeatures(filtered_image,49));
	}


	// Copy feature vector to mat file
    Mat feature_matrix = Mat(Sizew*Sizeh,25,CV_32FC1);

    for(int i=0;i<Sizeh*Sizew;i++){
        for(int j=0;j<25;j++){
            feature_matrix.at<float>(i,j) = features[j][i];
        }
    }

    features.clear();

    //--------------------------------------------------------------------------------------------------------------------------
    // normalize feature matrix
    
    Mat min,max;

    // Compute column-wise min and max
    reduce(feature_matrix,min,0,CV_REDUCE_MIN,-1);
    reduce(feature_matrix,max,0,CV_REDUCE_MAX,-1);

    // Normalize
    for(int i=0;i<feature_matrix.rows;i++){
    	feature_matrix.row(i) = (feature_matrix.row(i) - min)/(max - min);
    }


    // Implement PCA --------------------------------------------------------------------------------------------------------------
    Mat output_features;
    PCA pca_analysis(feature_matrix, Mat(), CV_PCA_DATA_AS_ROW,3);
    pca_analysis.project(feature_matrix,output_features);



    // implement K-means -------------------------------------------------------------------------------------------------

    int clusters;
    string filename = argv[1];

    if(filename == "Comb_2.raw"){
        clusters = 6;
    }
    else{
        clusters = 4;
    }



    Mat labels,centers;
    kmeans(output_features, clusters, labels,
            TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 30000, 0.0001),
               10, KMEANS_RANDOM_CENTERS, centers);


    // Reshape labels to a 2D image
    labels = labels.reshape(0,Sizeh);

    // multiply by multiplication factor to get grayscale image
    labels = labels * (255/(clusters-1));
    labels.convertTo(labels, CV_8UC1);

    // Display image
    namedWindow("Image");
    imshow("Image",labels);
    waitKey(0);
    cvDestroyWindow("Image");

    // Save segmented image
    imwrite("Segmented_Image.jpg",labels);


    return 0;
}

   