#include "Board.h"

Board::Board(SharedNumberGenerator *numberGenerator){
	_textAdded = false;
	_numberGenerator = numberGenerator;
	_currentFrameNumber = _numberGenerator->increaseCurrentFrame(); // get next available frame
	
}

void Board::setSubBoard(cv::Mat bg)
{
	Rect roi = Rect(0,0,1,1);
	BoardCalibration* calibrator = new BoardCalibration;

	_boardRegion = calibrator->calibrate(bg);
	_currentFrame = _currentFrame(_boardRegion);
	_result = _result(_boardRegion);
	_lastFrame = (_lastFrame)(_boardRegion);

	_colorResult = imread("bg.jpg")(_boardRegion);// not good should have created new mat

	// init colorResult to white.
	initColorResult();

	delete calibrator;
}

void Board::initColorResult(){
	for (int row =0 ; row < _colorResult.rows ; ++row) {
		for (int col = 0 ; col < _colorResult.cols ; ++col) {
				_colorResult.at<cv::Vec3b>(row,col)[0] = 255;
				_colorResult.at<cv::Vec3b>(row,col)[1] = 255;
				_colorResult.at<cv::Vec3b>(row,col)[2] = 255;
		} // end of col                   
	}//end of row*/
}
void Board::updateBoard(Mat frame)
{
	// Cleaning fitter
	NoiseCleaner cleaner; 

	// True = the first image.
	static bool first = true; 

	// Save the color image
	Mat colorFrame = frame(_boardRegion); 

	// Convert to gray
	cv::cvtColor(frame, frame, CV_BGR2GRAY); 

	stringstream filename;

	// Create the mask
	frame = getBoardText(frame(_boardRegion)); 

	setCurrentFrame(frame);

	// Case of first image
	if (first){
		setResult(getCurrentFrame());
		first = false;
	}

	filename << _currentFrameNumber << ".jpg";

	// There was no deletion
	if (isBoardErased()){ 
		// Update the current frame
		setCurrentFrame(cleaner.clean(frame,3));

		_textAdded = true;

		// Set result
		setResult(getCurrentFrame());

		// Update the color result
		updateColoredResult(colorFrame); 
		
		// The image that would be saved to disk
		cv::Mat imageToSave = cv::Mat();
		
		// Save the color result
		imwrite(fileDirectory+"/"+filename.str(),_colorResult);
	}
	else if (_textAdded && isNewBoard(getCurrentFrame()) ){
		//Save board to file.
		cout << "new board detected, saved file\n";

		imwrite(fileDirectory+"/"+filename.str(),_colorResult);

		// advance the next frame number
		_currentFrameNumber = _numberGenerator->increaseCurrentFrame(); 
		cout << "Assigned Frame #" << _currentFrameNumber << endl;

		// reset result
		setResult(getCurrentFrame()); 
		_textAdded = false;
	}

	setLastFrame(getCurrentFrame());

	setCurrentFrame( getCurrentFrame()-getCurrentFrame() ); // reset current frame to zeros

}

void Board::updateColoredResult(cv::Mat &colorFrame){

	for (int row =0 ; row < colorFrame.rows ; ++row) {
		for (int col = 0 ; col < colorFrame.cols ; ++col) {
			if ( _result.at<uchar>(row,col) == 255){ // if mask is 1
					if(_colorResult.at<cv::Vec3b>(row,col)[0] == 255){ // and color is 0 then update color
						_colorResult.at<cv::Vec3b>(row,col)[0] = colorFrame.at<cv::Vec3b>(row,col)[0];
						_colorResult.at<cv::Vec3b>(row,col)[1] = colorFrame.at<cv::Vec3b>(row,col)[1];
						_colorResult.at<cv::Vec3b>(row,col)[2] = colorFrame.at<cv::Vec3b>(row,col)[2];
					}
			}// end of if
			else if (_colorResult.at<cv::Vec3b>(row,col)[0] != 255){
				_colorResult.at<cv::Vec3b>(row,col)[0] = 255;
				_colorResult.at<cv::Vec3b>(row,col)[1] = 255;
				_colorResult.at<cv::Vec3b>(row,col)[2] = 255;
			}
		} // end of col                   
	}//end of row*/
}
void Board::displayResult(){
	stringstream winName;
	winName << "board" << _boardNum;
	imshow(winName.str(),this->_colorResult);
}

float Board::getFramesWhiteDiff( Mat& frame1, Mat& frame2 )
{
	Histogram histo;
	float white1 = histo.getWhitePercentage(frame1);
	float white2 = histo.getWhitePercentage(frame2);
	return white1-white2;
}

cv::Mat Board::getBoardText( cv::Mat & newBoard ,int thresh)
{
	// image of the object.
	cv::Mat obj 

	// image of the text with the object.
	,txt	

	// image of the sharpening result.
	,sharpText 

	// image of the blurred background.
	,bluredBg 

	// would contain the final result.
	,finalResult, 

	// would contain the previous current image.
	last,			

	// would contain the background.
	tempBg;			

	// Get the current background
	tempBg = _background(_boardRegion);

	last = getLastFrame();

	// blurring the the new board and background.
	cv::medianBlur(newBoard,obj,15); 
	cv::medianBlur(tempBg,bluredBg,15);

	cv::absdiff(obj,bluredBg,obj);
	cv::absdiff(newBoard,tempBg,txt);

	//sharpening the text
	cv::GaussianBlur(txt,sharpText,cv::Size(0,0),3);
	cv::addWeighted(txt,1.5,sharpText,-0.5,0,sharpText);

	//convert images to binary
	cv::threshold(obj,obj,thresh,255,CV_THRESH_BINARY);
	cv::threshold(sharpText,sharpText,thresh,255,CV_THRESH_BINARY);

	// show the image on window
	expandObject(obj);

	return specialOperator(last,obj,sharpText,getResult());
}

cv::Mat Board::specialOperator(cv::Mat &last,cv::Mat &obj,cv::Mat &sharpText,cv::Mat &result){
	cv::Mat finalResult;

	finalResult = incomplement(last) & result & sharpText & incomplement(obj);
	finalResult = finalResult | (incomplement(last) & incomplement(result) & sharpText & incomplement(obj));
	finalResult = finalResult | (last & result & sharpText & obj);
	finalResult = finalResult | (last & result & sharpText & incomplement(obj));
	finalResult = finalResult | (last & incomplement(result) & sharpText & incomplement(obj));

	return finalResult;
}
cv::Mat Board::incomplement( cv::Mat &img )
{
	// copy the origianl image
	cv::Mat result		= img.clone();

	// create a matrix contains only 255 
	cv::Mat max_value	= 255 * cv::Mat::ones(img.size(),img.type());

	return max_value - result;
}

void Board::expandObject( cv::Mat &image ,int radios)
{
	cv::Mat result = image.clone(); // clone the original image.

	for (int row =0 ; row < image.rows ; ++row) {
		for (int col = 0 ; col < image.cols ; ++col) {

			if ( image.at<uchar>(row,col) == 255 &&
				isEdge(image,cv::Point(row,col)) ){ // if the pixel is white.
					for(int j = row-radios ; j < row+radios ; ++j){
						for(int i = col-radios ; i < col+radios ; ++i){ 
							if ( isInRange(j,i,image.rows,image.cols)
								&& image.at<uchar>(j,i) == 0)
								result.at<uchar>(j,i)= 255;// color the pixel white.
						}// end of segment.
					}

					col +=radios;
			}// end of if
		} // end of col                   
	}//end of row
	image = result;
}


bool Board::isInRange( int row,int col,int numOfRows,int numOfCols )
{
	return row > 0 && row < numOfRows && col > 0 && col < numOfCols;
}


bool Board::isEdge( const cv::Mat & img,cv::Point p,int whiteValue)
{
	int row = p.x,
		col = p.y;

	for(int i = row-2 ; i < row+2 ; ++i )
		for(int j = col-2 ; j < col+2 ; ++j)
			if (isInRange(i,j,img.rows,img.cols) &&
				img.at<cv::Vec3b>(i,j)[0] == whiteValue )
				return true;

	return false;
}


bool Board::isEdge( const cv::Mat & img,cv::Point p )
{
	int row = p.x,
		col = p.y;

	for(int i = row-2 ; i < row+2 ; ++i )
		for(int j = col-2 ; j < col+2 ; ++j)
			if (isInRange(i,j,img.rows,img.cols) &&
				img.at<uchar>(i,j) == 0 )
				return true;

	return false;
}

bool Board::isNewBoard( Mat& board )
{
	Histogram histo;
	return histo.getWhitePercentage(board) < 0.01 ; 
}


