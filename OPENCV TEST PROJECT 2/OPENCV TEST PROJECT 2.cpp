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
	Mat original = imread( argv[1], 1 );
	if( !src.data )
	{ return -1; }
	/// Apply the Hough Transform to find the circles
	// HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/16, 2, 32.0, 10, 100 );
	cvtColor(src, src, CV_BGR2HSV);    

	for (int i=0; i < src.rows ; i++)
	{
		for(int j=0; j < src.cols; j++)
		{
			// You need to check this, but I think index 1 is for saturation, but it might be 0 or 2
			int idx = 1;
			src.at<cv::Vec3b>(i,j)[idx] = 5;

			// or:
			// img.at<cv::Vec3b>(i,j)[idx] += adds_constant_value;
		}
	}


	threshold( src, src, 8, 255, 0 );
	// HSV back to BGR
	cvtColor(src, src, CV_HSV2BGR);

	Mat img(200, 300, CV_8UC1);

	Mat saturated;

	double saturation = 10;
	double scale = 1;

	// what it does here is dst = (uchar) ((double)src*scale+saturation);
	img.convertTo(saturated, CV_8UC1, scale, saturation);
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
	params.maxArea = 50000.0f;
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
		circle( original, center, 3, Scalar(0,255,255), 5, 8, 0 );
	}
	Mat dst;
	Size size(src_gray.cols/2, src_gray.rows/2);
	resize(original, dst, size, 0, 0, 1);
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