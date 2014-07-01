#include "NoiseCleaner.h"

/* 
The function would recive a binary image and a contour treshhold.
the function would erase all the contour that contains less then the tresh.
*/
cv::Mat NoiseCleaner::clean(const cv::Mat &src,const int contourTresh){
	cv::Mat image = src.clone(),//original image
		mask = image.clone();

	// Get the contours of the connected components
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(mask, 
		contours, // a vector of contours 
		CV_RETR_EXTERNAL, // retrieve the external contours
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

	std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
	for ( ; itContours!=contours.end(); ++itContours) {
		// if contour is too small color the contour mask accordingly
		if (itContours->size() < contourTresh){
			cv::floodFill(image,(*itContours)[0],0);
		}
	}

	return (image);
}

/* 
The function recive the name of the file as a string and the index of the file.
The function would clean the image from noises.
*/
std::string NoiseCleaner::cleanImage(std::string fileAdd,int index){
	std::string address("images/"); // the folder of the images
	cv::Mat image= cv::imread(address + fileAdd,0),//original image
		mask(image.size(),CV_8U,cv::Scalar(255)),// image for mask
		originalImage(image.size(),CV_8U,cv::Scalar(255)); // the original image

	// convert the index of the image to string
	char indexChar[5]; 
	itoa(index,indexChar,10);
	std::string indexString(indexChar);

	originalImage = image.clone(); // deep clone of original image.

	int imageBackgroundColor = getImageBg(image); // calculate backgournd color

	// save the original image
	cv::imwrite(indexString+"src"+".jpg",image);

	// blurring the image inorder to clean noise
	cv::medianBlur(image,image,9); 

	cv::imwrite("image after blur.jpg",image);

	// convert to binary
	cv::threshold(image,image,165,255,CV_THRESH_BINARY);
	image = incomplement(image);

	cv::imwrite("image after thresh and ivert.jpg",image);
	mask = image.clone();

	// Get the contours of the connected components
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(image, 
		contours, // a vector of contours 
		CV_RETR_EXTERNAL, // retrieve the external contours
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

	/*cv::namedWindow("Before");
	cv::imshow("Before",mask);*/

	// Print contours' length
	std::cout << "Contours: " << contours.size() << std::endl;
	std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
	for ( ; itContours!=contours.end(); ++itContours) {
		// if contour is too small color the contour mask accordingly
		if (itContours->size() < 20){
			cv::floodFill(mask,(*itContours)[0],0);
		}
	}

	cv::imwrite("mask after filling.jpg",mask);
	colorImage(originalImage,mask,imageBackgroundColor);

	// Saving the result image
	cv::imwrite(indexString+"result"+".jpg",originalImage);
	return (indexString+"result"+".jpg");
	cv::waitKey();
}

// calculate the incomplemnt image
cv::Mat NoiseCleaner::incomplement( cv::Mat &img )
{
	cv::Mat result = img.clone();

	for(int row = 0 ; row < img.rows ; ++row)
		for(int col = 0 ; col < img.cols ; ++col){
			result.at<uchar>(row,col) = 255 - img.at<uchar>(row,col); 
		}

		return result;
}


/* The function would recive an image.
The function would calculate the backgournd of the image by 
caculating the avarge value of the all the pixels.*/
int NoiseCleaner::getImageBg(const cv::Mat &img){
	int bgColor = 0;
	for(int row = 0 ; row < img.rows ; ++row)
		for(int col = 0 ; col < img.cols ; ++col){
			bgColor += img.at<uchar>(row,col);
		}
		return bgColor/(img.rows*img.cols);
}

/* The function would recive a source image, a mask of the image's
contours and the image that would replace the pixels that are not in
the mask.*/
void NoiseCleaner::colorImage(cv::Mat& src,const cv::Mat& mask,int color){
	for(int row = 0 ; row < src.rows ; ++row)
		for(int col = 0 ; col < src.cols ; ++col){
			if (mask.at<uchar>(row,col) == 0){
				src.at<uchar>(row,col) = color;
			}
		}
}

std::vector<std::string> NoiseCleaner::readImageList(std::string fileAddress){
	std::ifstream myReadFile;
	std::vector<std::string> list;

	myReadFile.open(fileAddress);

	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			std::string temp;
			myReadFile >> temp;
			list.push_back(temp);
		}
	}

	return list;
}

