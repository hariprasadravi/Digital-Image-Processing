// EE569 Homework Assignment #3
// Date:     Nov 6, 2016
// Name:     Hariprasad Ravishankar
// ID:       8991379333
// Email:    hravisha@usc.edu

#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "opencv2/features2d.hpp" 
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

    // Define Hessian
    int minHessian = 400;

    // Create SURF object
    Ptr<Feature2D> detector = xfeatures2d::SURF::create( minHessian );


    // Define Keypoints
    std::vector<KeyPoint> keypoints_1, keypoints_2;

    // Detect Keypoints
    detector->detect( Image_1, keypoints_1 );
    detector->detect( Image_2, keypoints_2 );

    //-- Draw keypoints
    Mat img_keypoints_1; Mat img_keypoints_2;

    drawKeypoints( Image_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    drawKeypoints( Image_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    

    //-- Show detected (drawn) keypoints
    imshow("Keypoints 1", img_keypoints_1 );
    imwrite( "SURF_Keypoints_image1.jpg",img_keypoints_1 );
    imshow("Keypoints 2", img_keypoints_2 );
    imwrite( "SURF_Keypoints_image2.jpg",img_keypoints_2 );
    

    // Compute SURF Descriptor
    Mat SURF_descriptors1,SURF_descriptors2;
    detector->compute(Image_1,keypoints_1,SURF_descriptors1);
    detector->compute(Image_2,keypoints_2,SURF_descriptors2);


    // Match Descriptors
    Mat outputImage;
    vector< DMatch > matches;
    FlannBasedMatcher matcher;
    matcher.match(SURF_descriptors1,SURF_descriptors2,matches);

    double max_dist = 0; double min_dist = 100;

    // Calculate distance between matched key points
    for( int i = 0; i < SURF_descriptors1.rows; i++ ){
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

    for( int i = 0; i < SURF_descriptors1.rows; i++ ){

        if( matches[i].distance <= (2*min_dist) ){
            
            good_matches.push_back( matches[i]);
        }
    
    }

    // Draw Matches
 

    // Draw best matches, leave out other keypoints

    drawMatches( Image_1, keypoints_1, Image_2,keypoints_2,
       good_matches, outputImage, Scalar::all(-1), Scalar::all(-1),
       vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    // // Draw matches
    // drawMatches(Image_1,keypoints_1,Image_2,keypoints_2, matches, outputImage);


    imshow(" Matches ", outputImage);
    waitKey(0);

    imwrite( "SURF_matches.jpg",outputImage );


    return 0;
}

   