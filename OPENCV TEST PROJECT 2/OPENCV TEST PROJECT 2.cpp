#include "stdafx.h"
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
//#include <boost/filesystem.hpp>

using namespace std;
using namespace cv;

/** @function main */
int main(int argc, char** argv)
{
	Mat src, src_gray;
	/// Read the image
	src = imread( argv[1], 1 );
	// Copy is used to mark detected blobs without using original image. 
	// This may be over precautionary and subject to change
	Mat copy = imread( argv[1], 1 );
	if( !src.data )
	{ return -1; }

	cvtColor(src, src, CV_BGR2HSV);    

	for (int i=0; i < src.rows ; i++)
	{
		for(int j=0; j < src.cols; j++)
		{
			int idx = 1;
			src.at<cv::Vec3b>(i,j)[idx] = 5;
		}
	}

	threshold( src, src, 12, 255, 0 );
	// HSV back to BGR
	cvtColor(src, src, CV_HSV2BGR);

	Mat saturated;

	double saturation = 10;
	double scale = 1;

	// what it does here is dst = (uchar) ((double)src*scale+saturation);
	cvtColor( src, src_gray, CV_BGR2GRAY );

	/// Reduce the noise so we avoid false circle detection
	GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );
	SimpleBlobDetector blob;

	cv::SimpleBlobDetector::Params params;
	params.minDistBetweenBlobs = src.rows/16;
	params.filterByInertia = false;
	params.filterByConvexity = false;
	params.filterByColor = false;
	params.filterByCircularity = false;
	params.filterByArea = true;
	params.minArea = src.rows/30*src.rows/30;
	params.maxArea = src.rows/2*src.rows/2;
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
		circle( copy, center, 3, Scalar(0,0,255), 5, 8, 0 );
	}
	Mat dst;
	Size size(src_gray.cols/2, src_gray.rows/2);
	resize(copy, dst, size, 0, 0, 1);
	imshow( "Blob Detection Test", dst );
	resize(src_gray, dst, size, 0, 0, 1);
	imshow( "Threshold applied", dst );

	/*
	/// Show your results
	namedWindow( "Blob detection", CV_WINDOW_AUTOSIZE );
	imshow( "Blob detection", src );
	*/

	waitKey(0);
	return 0;
}