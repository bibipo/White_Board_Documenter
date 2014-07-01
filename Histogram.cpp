#include "Histogram.h"

Histogram::Histogram(){
	_histSize[0]=256;
	_channels[0]=0;
	_ranges[0]=0.0;
	_ranges[1]=256.0;
	_histRange[0] = _ranges;
}

Histogram::Histogram(int bins, cv::Point2f range, int channels){
	_histSize[0]=bins;
	_channels[0]=channels;
	_ranges[0]=range.x;
	_ranges[1]=range.y;
	_histRange[0] = _ranges;
}

cv::Mat Histogram::getHist(const cv::Mat &image){
	cv::Mat hist;
	cv::calcHist(&image, 1, _channels, cv::Mat(), hist, 1, _histSize, _histRange);
	return hist;
}


float Histogram::getWhitePercentage(const cv::Mat &image){
	cv::Mat histo;
	Histogram hist;
	histo=hist.getHist(image);

	return (histo.at<float>(255)/(image.rows*image.cols))*100;

}

float Histogram::getBlackPercentage(const cv::Mat &image){
	return 100-getWhitePercentage(image);
}