#pragma once
#include "Core.h"

class NoiseCleaner{
public:
	std::vector<std::string> readImageList(std::string fileAddress);
	std::string cleanImage(std::string fileAdd,int index = 0);
	cv::Mat clean(const cv::Mat &src,const int contourTresh);

private:
	void colorImage(cv::Mat& src,const cv::Mat& mask,int color);
	int getImageBg(const cv::Mat &img);
	cv::Mat incomplement( cv::Mat &img );
};
