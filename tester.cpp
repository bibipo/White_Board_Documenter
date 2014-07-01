/*#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include <iostream>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include "Histogram.h"
#include "Controller.h"

using namespace cv;
using namespace std;

#define CAPTURE 1


int main(){
	if (CAPTURE){
		Controller * ctrlr = new Controller();

		//ctrlr->captureFromFileMultiThreads("presentationx4.wmv");
		//ctrlr->captureFromFileMultiBoards("board1.wmv");
		ctrlr->captureFromCamera();

		delete ctrlr;
	}
	else{
		Client *c = new Client("Linear Algebra 2");
		if (c->openSocket() == SOCKET_ERROR){
			cout << "failed opening socket\n";
			exit(1);
		}

		cout << "press any key to continue..." << endl;

		getch(); // wait for key pressed

		cout << "sending files" << endl;

		for (int i=0; i<2; i++){
			stringstream filename;
			filename << i << ".jpg";
			c->sendFile(filename.str());
			Sleep(2000);
		}

		getch(); // wait for key pressed
		
		c->closeSocket();
	
		delete c;
	}
}

void resize(){
	// Load images in the C++ format
	cv::Mat img = cv::imread("1.jpg");
	cv::Mat src = cv::imread("small.jpg");

	cv::namedWindow( "resize image", CV_WINDOW_AUTOSIZE );

	for (int i = 0 ; i < 2 ; ++i){
		cv::imshow("resize image",src);
		cv::waitKey();

		cv::Size newImageSize = cv::Size(img.rows,img.cols);
		// Resize src to img size
		cv::resize(src, src, newImageSize);


		cv::imshow("resize image",src);
		cv::waitKey();
	}
}*/