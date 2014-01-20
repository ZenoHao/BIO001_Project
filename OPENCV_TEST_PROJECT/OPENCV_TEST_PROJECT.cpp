#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
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

	/// Convert it to gray
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

	threshold( src, src, 15, 255, 0 );
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

	vector<Vec3f> circles;

	/// Apply the Hough Transform to find the circles
	HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/16, 2, 32.0, 2, src_gray.rows/16 );

	/// Draw the circles detected

	for( size_t i = 0; i < circles.size(); i++ )
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
		// circle outline
		circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
	}

	/// Show your results
	namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
	imshow( "Hough Circle Transform Demo", src );

	waitKey(0);
	return 0;
}