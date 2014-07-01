#include "Controller.h"

Controller::Controller(){
	// set the current frame to 0
	_currentFrame = 0;

	// create the controller boards
	createBoards();
}

Controller::~Controller()
{
	// remove boards
	vector<Board *>::iterator iter = _boards.begin();
	for(;iter != _boards.end() ; ++iter )
		delete *iter;
}

void Controller::captureFromFile(string filename){

	// Flag true if earase was detected
	bool eraseDetected = false;

	// Flag for the first image
	bool first=true;

	// Flag true if text was added
	bool textAdded=false;

	// Connect to streame
	cv::VideoCapture capture(filename);

	// Test source
	if (!capture.isOpened()){
		cout << "Error capture from file\n";
		return;
	}

	// Set the frame rate per second
	int fps=30;

	// The delay rate between each sample
	int delay = 1000/fps;
	bool stop = false;
	cv::Mat frame;

	// Take background image
	if (!capture.read(frame)){
		cout << "Error opening video" << endl;
		return;
	}

	// Convert to black and white
	imwrite("bg.jpg",frame);
	frame = imread("bg.jpg",0);

	// Initialize the boards with the backgound image
	initBoards(frame);

	// Calibrate the boards 
	calibrateBoards(frame);
	createWindows();

	cv::namedWindow("File",CV_WINDOW_AUTOSIZE);

	// Would count the number of frames
	int counter=0;

	// Would contain the threads that would update the boards
	vector < boost::thread * > threads;

	// Main loop
	while (!stop){
		if (!capture.read(frame))
			break;

		imshow("File",frame);

		if (counter % fps == 0){
			vector<Board *>::iterator iter = _boards.begin();

			// Iterate over the boards and update each board in the vector
			for(;iter != _boards.end() ; ++iter){
				// Create new thread to update the board
				boost::thread * tempThread = new boost::thread(&Controller::updateBoard,this,(*iter),frame);
				threads.push_back(tempThread);
			}

			for(int i = 0 ; i < threads.size() ; ++i){
				// Wait to the thread to finish
				threads[i]->join();

				// Display the result
				_boards[i]->displayResult();

				// Delete the thread
				delete threads[i];
			}

			threads.clear();

		}// Counter

		
		// If exit key recived then stop
		if (cv::waitKey(delay)>=0)
			stop=true;

		counter++;
	}

	// Release the capture source
	capture.release();
}


void Controller::updateBoard(Board * board,cv::Mat frame){
	board->updateBoard(frame);
}

void Controller::captureFromCamera(){

	// Flag true if earase was detected
	bool eraseDetected	= false;

	// Flag for the first image
	bool first			= true;

	// Flag true if text was added
	bool textAdded		= false;

	CvCapture* capture;

	// Get the background's image
	Mat frame = getBG();

	// Get input from camera
	capture = cvCreateCameraCapture(0); 

	// Set camera resolution (1280x720) - 720p
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 1280 ); 
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 720 );

	// Define temp image
	IplImage* image;

	// Flag if to keep main loop runing
	bool stop = false;

	// Save the background image
	imwrite("bg.jpg",frame);
	frame = imread("bg.jpg",0);

	// initialize the boards with the backgound image
	initBoards(frame);

	// calibrate the boards 
	calibrateBoards(frame);
	createWindows();

	// Would contain the threads that would update the boards
	vector < boost::thread * > threads;

	// Main loop
	while (!stop){
		// Take snap shoot
		image = cvQueryFrame(capture); 

		// In case of error quit
		if (!image) 
			break;

		// Convert the image to cv::Mat format
		frame = Mat(image);

		// Iterate over the boards and update each board in the vector
		vector<Board *>::iterator iter = _boards.begin();
		for(;iter != _boards.end() ; ++iter){
			// Create new thread to update the board
			boost::thread * tempThread = new boost::thread(&Controller::updateBoard,this,(*iter),frame);
			threads.push_back(tempThread);
		}

		for(int i = 0 ; i < threads.size() ; ++i){
			// Wait for the thread to finish
			threads[i]->join();

			// Display the result
			_boards[i]->displayResult();

			// Delete the thread
			delete threads[i];
		}

		threads.clear();

		// If exit key recived then stop
		if (cv::waitKey(10)>=0)
			stop=true;
	}

	// Release image also
	cvReleaseCapture( &capture ); 
}

void Controller::createWindows(){
	int numOfBoards = _boards.size();
	stringstream winName;

	for (int i=0; i<numOfBoards; i++){
		winName << "board" << i+1;
		namedWindow(winName.str(),CV_WINDOW_AUTOSIZE);
		moveWindow(winName.str(),0,0);
		winName.str("");
	}
}

void Controller::calibrateBoards(cv::Mat background){
	vector<Board *>::iterator iter = _boards.begin();

	for(; iter != _boards.end() ; ++iter){
		(*iter)->setSubBoard(background);
	}
}

cv::Mat Controller::getBG()
{
	// input from user
	int keyPressed;

	// get camera input
	CvCapture* capture = cvCaptureFromCAM( 0 ); //0=default, -1=any camera, 1..99=your camera;
	if(!capture) 
		cout << "No camera detected" << endl;

	// set camera resolution (1280x720) - 720p
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 1280 ); 
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 720 );

	cout << "Press ENTER to set background" << endl;
	cvNamedWindow( "Setting background", 1 );
	IplImage* iplImg;
	cv::Mat frame;

	while(1){
		// get frame from camera
		iplImg = cvQueryFrame( capture );

		// display the frame
		cvShowImage( "Setting background", iplImg );

		// get input from user
		keyPressed=waitKey(40);
		if (keyPressed == ENTER_KEY_VALUE){ // if keypressed == ENTER
			// convert the frame from IplImage* to Mat
			cv::Mat frame ( cvarrToMat(iplImg,true) );

			// release resources
			cvReleaseCapture( &capture );
			destroyWindow("Setting background");
			return frame;
		}
	}
}


void Controller::createBoards()
{
	// The number of boards that the user would write on
	int numOfBoards;

	// Recive input from user
	cout << "Please enter the number of board you would like to create:\n";
	cin >> numOfBoards;

	// Create boards Objects
	for(int i = 0 ; i < numOfBoards ; ++i)
		_boards.push_back(new Board(&_numberGenerator));
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string Controller::currentDateTime(){
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    
    // for more information about date/time format
	strftime(buf, sizeof(buf), "%d%b%Y-%H-%M-%S", &tstruct);

    return buf;
}

void Controller::initBoards( cv::Mat img )
{
	std::string fileDirectory = currentDateTime();

	boost::filesystem::path dir(fileDirectory);
	if(boost::filesystem::create_directory(dir)) {
		std::cout << "Success" << "\n";
	}

	int boardNum=1;
	vector<Board *>::iterator iter = _boards.begin();
	for(;iter != _boards.end() ; ++iter ){
		(*iter)->setBackground(img);
		(*iter)->setCurrentFrame(img-img);
		(*iter)->setResult(img-img);
		(*iter)->setLastFrame(img-img);
		(*iter)->setBoardNum(boardNum++);
		(*iter)->setFileDirectory(fileDirectory);
	}
}