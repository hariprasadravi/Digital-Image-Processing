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

using namespace cv;
using namespace std;
using namespace cv::ximgproc;

int main(int argc, char *argv[])

{



	// Read Jeep and Bus Images
	Mat Image_1 = imread(argv[1]);
	Mat Image_2 = imread(argv[2]);


	// Create SIFT object
    cv::Ptr<Feature2D> sift_object = xfeatures2d::SIFT::create();

    //SIFT Keypoint vectors
    std::vector<KeyPoint> SIFT_keypoints1, SIFT_keypoints2;
    
    //Detect Keypoints
    sift_object->detect(Image_1, SIFT_keypoints1);
	sift_object->detect(Image_2, SIFT_keypoints2);
    
	// Display Image with Keypoints
    Mat outputImage;
    drawKeypoints(Image_1,SIFT_keypoints1,outputImage);
    imshow(" Keypoints 1 ", outputImage);
    imwrite( "SIFT_Keypoints_image1.jpg",outputImage );

    drawKeypoints(Image_2,SIFT_keypoints2,outputImage);
    imshow(" Keypoints 2", outputImage);
    imwrite( "SIFT_Keypoints_image2.jpg",outputImage );

    // Compute SIFT descriptors
    Mat SIFT_descriptors1,SIFT_descriptors2;
    sift_object->compute(Image_1,SIFT_keypoints1,SIFT_descriptors1);
    sift_object->compute(Image_2,SIFT_keypoints2,SIFT_descriptors2);

    // Compute Matches
    vector< DMatch > matches;
    FlannBasedMatcher matcher;
    matcher.match(SIFT_descriptors1,SIFT_descriptors2,matches);

    
	double max_dist = 0; double min_dist = 100;

	// Calculate distance between matched key points
	for( int i = 0; i < SIFT_descriptors1.rows; i++ ){
		double dist = matches[i].distance;
		
		// Calclulate least distance
		if( dist < min_dist ){
			min_dist = dist;
		} 

		// Caluculate large distances
		if( dist > max_dist ){
		 	max_dist = dist;
		}
	}

	// Define Dmatch vector to keep good matches

	// Keep Keypoints that are very similar. i.e Euclidean Distance is less than 2*min distance
	std::vector< DMatch > good_matches;

	for( int i = 0; i < SIFT_descriptors1.rows; i++ ){

		if( matches[i].distance <= (2*min_dist) ){
	 		
			good_matches.push_back( matches[i]);
		}
	
	}

	// Draw Matches


	// Draw best matches, leave out other keypoints

	drawMatches( Image_1, SIFT_keypoints1, Image_2,SIFT_keypoints2,
	   good_matches, outputImage, Scalar::all(-1), Scalar::all(-1),
	   vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
 
    imshow(" Image Matches ", outputImage);
    waitKey(0);
    imwrite( "SIFT_matches.jpg",outputImage );


    return 0;
}

   