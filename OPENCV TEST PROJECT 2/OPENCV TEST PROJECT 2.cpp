#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/features2d/features2d.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;

/** @function main */
int main(int argc, char** argv)
{
	Mat src, src_gray;

	/// Read the image
	src = imread( argv[1], 1 );

	if( !src.data )
	{ return -1; }
	/// Apply the Hough Transform to find the circles
	// HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/16, 2, 32.0, 10, 100 );
	SimpleBlobDetector blob;

	cv::SimpleBlobDetector::Params params;
	params.minDistBetweenBlobs = src.rows/16;
	params.filterByInertia = false;
	params.filterByConvexity = false;
	params.filterByColor = false;
	params.filterByCircularity = false;
	params.filterByArea = false;
	params.minArea = 0;
	params.maxArea = 500.0f;
	// ... any other params you don't want default value

	// set up and create the detector using the parameters
	cv::Ptr<cv::FeatureDetector> blob_detector = new cv::SimpleBlobDetector(params);
	blob_detector->create("SimpleBlob");

	// detect!
	vector<cv::KeyPoint> keypoints;
	blob_detector->detect(src, keypoints);

	// extract the x y coordinates of the keypoints:
	for( size_t i = 0; i < keypoints.size(); i++ ){
		Point center(keypoints[i].pt.x,keypoints[i].pt.y);
		circle( src, center, 3, Scalar(0,255,0), 1, 8, 0 );
	}
		//Point center(src.cols/2, src.rows/2);
		//circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
	imshow( "Blob Detection Test", src );

	/*
	/// Show your results
	namedWindow( "Blob detection", CV_WINDOW_AUTOSIZE );
	imshow( "Blob detection", src );
	*/

	waitKey(0);
	return 0;
}