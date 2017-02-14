
// EE569 Homework Assignment #3
// Date:     Nov 6, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu

#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>

using namespace cv;
using namespace std;
using namespace cv::ximgproc;

int main(int argc, char *argv[])

{


	// Read all four images
	Mat Image_1 = imread(argv[1]);
	Mat Image_2 = imread(argv[2]);
	Mat Image_3 = imread(argv[3]);
	Mat Image_4 = imread(argv[4]);

	// ------------------------------------------------------------------------------------------------------------------------------

	// create SIFT object
    cv::Ptr<Feature2D> sift_object = xfeatures2d::SIFT::create();
    std::vector<KeyPoint> keypoints1, keypoints2, keypoints3, keypoints4;
	// ------------------------------------------------------------------------------------------------------------------------------
    

    // Detect all four SIFT keypoints
    sift_object->detect(Image_1, keypoints1);
	sift_object->detect(Image_2, keypoints2);
	sift_object->detect(Image_3, keypoints3);
	sift_object->detect(Image_4, keypoints4);
    
	// ------------------------------------------------------------------------------------------------------------------------------


	// Compute Descriptors for all images
    Mat descriptors1,descriptors2,descriptors3,descriptors4;
    sift_object->compute(Image_1,keypoints1,descriptors1);
    sift_object->compute(Image_2,keypoints2,descriptors2);
    sift_object->compute(Image_3,keypoints3,descriptors3);
    sift_object->compute(Image_4,keypoints4,descriptors4);

	// ------------------------------------------------------------------------------------------------------------------------------

    // Concatenate descriptors of first 3 images into ine Matrix
    Mat tem,features;
    vconcat(descriptors1,descriptors2,tem);
    vconcat(tem,descriptors3,features);

	// ------------------------------------------------------------------------------------------------------------------------------

    // Implement K-means to obtain 8 clusters and cluster centroids
    Mat labels,centers;
    kmeans(features, 8, labels,
            TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 30000, 0.00001),
               10, KMEANS_RANDOM_CENTERS, centers);

 	// ------------------------------------------------------------------------------------------------------------------------------

    // Declare histogram and assign all zeros
    vector < Mat > histogram(4);
    for(int i=0;i<4;i++){
    	histogram[i] = Mat(8,1,CV_32FC1,Scalar(0));

	}
	// ------------------------------------------------------------------------------------------------------------------------------

	// Compute histogram for the first 3 images

    for(long int i=0;i<labels.rows;i++){
    	if (i<descriptors1.rows){

    		histogram[0].at<float>(labels.at<int>(i,0),0)++;
    	}
    	else if(i >= descriptors1.rows && i < (descriptors1.rows + descriptors2.rows)){
    		
    		histogram[1].at<float>(labels.at<int>(i,0),0)++;
    	}
    	else if(i >= (descriptors1.rows + descriptors2.rows) && i < labels.rows){

    		histogram[2].at<float>(labels.at<int>(i,0),0)++;
    	}
    }

	// ------------------------------------------------------------------------------------------------------------------------------

    // Compute histogram for test image
    Mat squared;
    vector <float> dist;

   	// Use Euclidean distance for distance metric
    for(long int i=0; i<descriptors4.rows ; i++){

    	for(int j=0;j< centers.rows;j++){
    		pow((descriptors4.row(i) - centers.row(j)),2,squared);
    		dist.push_back(sum(squared)[0]);
    	}

    	histogram[3].at<float>((min_element(dist.begin(),dist.end()) - dist.begin()),0)++; // finds min location
    	dist.clear();
    }

	// ------------------------------------------------------------------------------------------------------------------------------

    // Compare histograms based on Euclidean distance 
    for(long int i=0; i< 3 ; i++){

    	pow(histogram[3] - histogram[i],2,squared);
    	dist.push_back(sum(squared)[0]);

    }

	// ------------------------------------------------------------------------------------------------------------------------------

    // Determine closest match
    int classification_label;
    classification_label = (min_element(dist.begin(),dist.end()) - dist.begin());
    cout<< "closest image is " <<argv[classification_label+1]<<endl;


    // Display matched image
	Mat MatchedImage = imread(argv[classification_label+1]);
    imshow(" Matched Image ", MatchedImage );
    waitKey(0);

	// ------------------------------------------------------------------------------------------------------------------------------

    return 0;
}

   