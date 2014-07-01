#pragma once
#include "Core.h"
//-------------------------------------------------------------------------
// get histogram of one gray scale image.
// usage:
//		cv::Mat img = imread("image.jpg,0); // gray  scale
//		cv::threshold(img,img,TRESHOLD,255,CV_THRESH_BINARY); // to binary
//		cv::Mat histo; // will hold to histogram
//		Histogram hist; // use default c-tor - grayscale 1 channel (0-255)
//		histo=hist.getHist(img); // get the histogram 
// 
//		//print pixels quantity:
//
//		cout << "0 Values: " << histo.at<float>(0) << endl;
//		cout << "255 Values: " << histo.at<float>(255) << endl;
//
//-------------------------------------------------------------------------

class Histogram{
private:
	int _histSize[1]; // number of bins
	float _ranges[2]; // min and max pixel values
	const float * _histRange[1];
	int _channels[1];

public:
	// default constructor - GRAYSCALE IMAGE (0-255)
	Histogram();
	// constructor
	Histogram(int bins, cv::Point2f range, int channels=0);

	// will receive image and return its histogram
	cv::Mat getHist(const cv::Mat &image);

	// return the percentage of white pixels (255) compares to blacks (0)
	float getWhitePercentage(const cv::Mat &image);

	// return the percentage of black pixels (0) compares to blacks (255)
	float getBlackPercentage(const cv::Mat &image);

};