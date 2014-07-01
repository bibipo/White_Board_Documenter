#pragma once
#include "Board.h"

class Controller{
private:
	//*********************************************************************
	// The function would recive an image of the empty backgound.
	// The function would iterate over the boards buffer and would define according
	// the user input the area of the board.
	//*********************************************************************
	void calibrateBoards(cv::Mat background);

	//*********************************************************************
	// Thread method. The function would update the board date according to the
	// given image.
	//*********************************************************************
	void updateBoard(Board * board,cv::Mat frame);

	//*********************************************************************
	// Create the controller boards
	//*********************************************************************
	void createBoards();

	//*********************************************************************
	// Create the display windows
	//*********************************************************************
	void createWindows();

	//*********************************************************************
	// The function would initialize all the boards in the class buffer
	//*********************************************************************
	void initBoards(cv::Mat img);

	//*********************************************************************
	// The function would return string containing the current time and 
	// date.
	//*********************************************************************
	const std::string currentDateTime();

	//*********************************************************************
	// The function would return an image with empty backgournd ==> 
	// background without objects on.
	//*********************************************************************
	cv::Mat getBG();
	
	// Contains all the boards that were defined by the user
	vector<Board *> _boards;

	// The current board (accumulator)
	int _currentFrame; 

	// Thread-safe shared number generator
	SharedNumberGenerator _numberGenerator;

public:
	//*********************************************************************
	// Constractor
	//*********************************************************************
	Controller();

	//*********************************************************************
	// Destractor
	//*********************************************************************
	~Controller();
	
	//*********************************************************************
	// Capture and process input from live stream
	//*********************************************************************
	void captureFromCamera();

	//*********************************************************************
	// Capture and process input from recorded media video using multithreading
	//*********************************************************************
	void captureFromFile(string filename);	
};