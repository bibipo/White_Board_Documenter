#pragma once

//*********************************************************************
//								Macros Section
//*********************************************************************
#define CAPTURE 1
#define CAPTURE_FROM_CAMREA 1
#define CAPTURE_FROM_FILE 2
#define FINISH 3
#define RUN_FOREVER 0
#define ENTER_KEY_VALUE 13
#define VERSION 1.0

//*********************************************************************
//								Include Section
//*********************************************************************
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <conio.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <time.h>

//*********************************************************************
//								Using Section
//*********************************************************************
using namespace cv;
using namespace std;
