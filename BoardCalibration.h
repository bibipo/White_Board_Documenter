#pragma once
#include "Core.h"

using namespace std;
using namespace cv;

static void my_mouse_callback(int event, int x, int y, int flags, void* param);

class BoardCalibration{
private:


public:
	Rect _box;
	bool _drawingBox;

	BoardCalibration();
	Rect calibrate(Mat& image);
	void drawBox(IplImage* img);
};
