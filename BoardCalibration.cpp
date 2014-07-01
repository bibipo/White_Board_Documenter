#include "BoardCalibration.h"

struct Helper
{
	IplImage * pI;
	BoardCalibration * pObj;
};

BoardCalibration::BoardCalibration(){
	_box = Rect(0,0,1,1);
	_drawingBox = false;
}

void BoardCalibration::drawBox( IplImage* img){
	cvRectangle (img, cvPoint(_box.x,_box.y), cvPoint(_box.x+_box.width,_box.y+_box.height), cvScalar(0xff,0x00,0x00), 2);
}

Rect BoardCalibration::calibrate(Mat& image){
	IplImage * img = new IplImage(image);
	string winName = "Win";

	Helper * p = new Helper;
	p->pI = img;
	p->pObj = this;

	namedWindow(winName);

	IplImage *temp = cvCloneImage(img);
	
	cvSetMouseCallback(winName.c_str(), my_mouse_callback, (void *)p);

	while (1){

		imshow(winName, Mat(img));
		cvCopyImage(img,temp);

		if( _drawingBox ){
			drawBox(temp);
		}

		imshow(winName, Mat(temp));

		if (waitKey(1)>=0){
			drawBox(img); // keep draw (selected box)
			break;
		}
	}

	destroyWindow(winName);
	delete p;
	delete img;
	return _box;
}

static void my_mouse_callback(int event, int x, int y, int flags, void* param){

	Helper * p = (Helper *)param;
	IplImage* image = p->pI;
	BoardCalibration * pBC = p->pObj;

	switch( event ) {
	case CV_EVENT_MOUSEMOVE: {
		if( pBC->_drawingBox ) {
			pBC->_box.width  = x-pBC->_box.x;
			pBC->_box.height = y-pBC->_box.y;
		}
							 }
							 break;
	case CV_EVENT_LBUTTONDOWN: {
		pBC->_drawingBox = true;
		pBC->_box = Rect( x, y, 0, 0 );
							   }
							   break;   
	case CV_EVENT_LBUTTONUP: {
		pBC->_drawingBox = false; 
		if( pBC->_box.width<0 ) { 
			pBC->_box.x+=pBC->_box.width;  
			pBC->_box.width *=-1; 
		}
		if( pBC->_box.height<0 ) { 
			pBC->_box.y+=pBC->_box.height; 
			pBC->_box.height*=-1; 
		}
		//pBC->drawBox(image);  // keep last draw on screen
		// display rectangle coordinates
		//cout << "TopLeft: (" << pBC->_box.x << "," << pBC->_box.y << "), BottomRight: (" 
		//	<< pBC->_box.x+pBC->_box.width << "," << pBC->_box.y+pBC->_box.height << ")" << endl;
							 }
							 break;
	}
}