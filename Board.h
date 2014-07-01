//*********************************************************************
//								Board.h
//*********************************************************************
#pragma once
#include "BoardCalibration.h"
#include "Histogram.h"
#include "NoiseCleaner.h"
#include "SharedNumberGenerator.h"

class Board{
	private:
		// The empty background of the image.
		Mat _background;  

		// The colored result
		Mat _colorResult; 

		// The accumulated current frame with the text with 
		// out the object.
		Mat _currentFrame; 
						
		// The previous current frame.
		Mat _lastFrame;	 

		// The current result.
		Mat _result;		

		// Represents the sub-board region
		Rect _boardRegion;		

		// The board number
		int _boardNum; 

		// Indicates if text added to board
		bool _textAdded; 

		// Socket connection to the server
		SharedNumberGenerator* _numberGenerator;

		// The currentFrameNumber;
		int _currentFrameNumber;

		// The path to the lecture directory
		std::string fileDirectory;

		// Get the current time and date
		std::string getCurrentDateTime();

		//*********************************************************************
		// The function represents the operator we built to extract the new 
		// text from the image without the object in the frame.
		//*********************************************************************
		cv::Mat specialOperator(cv::Mat &last,cv::Mat &obj,cv::Mat &sharpText,cv::Mat &result);

		//*********************************************************************
		// The function would check if an creased was occurred.
		//*********************************************************************
		bool isBoardErased(){return (getFramesWhiteDiff(_lastFrame,_currentFrame) < -0.02);};

		//*********************************************************************
		// The function would receive a board image.
		// The function would check if it's a new board.
		//*********************************************************************
		bool isNewBoard(Mat& board);

		//*********************************************************************
		// The function would receive two images.
		// The function would return the white difference between the 
		// two images.
		//*********************************************************************
		float getFramesWhiteDiff(Mat& frame1, Mat& frame2);

		//*********************************************************************
		// The function would receive a row board image.
		// The function would return the current accumulated board's
		// with the text without the object.
		//*********************************************************************
		cv::Mat Board::getBoardText( cv::Mat & newBoard ,int thresh = 40);

		//*********************************************************************
		// The function would receive a binary image the contain the recognized 
		// object and a radios.
		// The function would expend the recognized object in the range of the
		// given value.
		//*********************************************************************
		void expandObject( cv::Mat &image ,int radios = 40);

		//*********************************************************************
		// The function would receive a image and a point(pixel) in the image.
		// The function would check if the point is an "edge" i.e. one of the 
		// point neighbors have value of "0".
		//*********************************************************************
		bool isEdge( const cv::Mat & img,cv::Point p,int whiteValue);
		bool isEdge( const cv::Mat & img,cv::Point p );

		//*********************************************************************
		// The function would recive a row and col that are corospanded to x 
		// and y coordinates.
		// The function would check if the given coordinated are within the 
		// boarder of the image. 
		// The function would return a value accordingly.
		//*********************************************************************
		bool isInRange(int row,int col,int numOfRows,int numOfCols);
		
		//*********************************************************************
		// The function would receive a binary source image.
		// The function would return the incomplement image of the source 
		// image.
		//*********************************************************************
		cv::Mat incomplement(cv::Mat &img);

		void updateColoredResult(cv::Mat &colorFrame);
		void initColorResult();

		//*********************************************************************
		// The function would receive an image.
		// The function return the avarage value of the image
		//*********************************************************************
		int calcAvarageValue(cv::Mat image);

	public:
		//*********************************************************************
		// default constractor,
		//*********************************************************************
		Board(SharedNumberGenerator *numberGen);

		//*********************************************************************
		// The function would set the subBoard area of the board using 
		// calibration object.
		//*********************************************************************
		void setSubBoard();

		//*********************************************************************
		// The function would receive a raw image of the board.
		// the function would update the board's properties.
		//*********************************************************************
		void updateBoard( Mat frame);
		
		//*********************************************************************
		// Setter and getters
		//*********************************************************************

		Mat getSubBoard(int boardNum);
		void setSubBoard(cv::Mat bg);

		cv::Rect getBoardRegion(){return _boardRegion;};
		void setActiveBoard(int boardNum);
		void displayBoard(int boardNum);

		void setCurrentFrame(Mat frame){ _currentFrame = frame.clone(); };
		Mat getCurrentFrame(){ return _currentFrame; };

		void setLastFrame(Mat frame){ _lastFrame = frame.clone(); };
		Mat getLastFrame(){ return _lastFrame; };

		void setResult(Mat result){ _result = result.clone(); };
		Mat getResult(){ return _result; };

		void setBackground(Mat bg){ _background = bg.clone();};
		Mat getBackground(){ return _background; };

		void setBoardNum(int num){ _boardNum = num; };
		int getBoardNum(){ return _boardNum; };

		void setFileDirectory(std::string fileDirectory){ this->fileDirectory = fileDirectory; };
		void displayResult();
};