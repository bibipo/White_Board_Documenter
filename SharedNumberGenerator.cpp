#include "SharedNumberGenerator.h"

SharedNumberGenerator::SharedNumberGenerator(string serverName)
{
	_currentFrame = -1; // first board will take this plus one => 0
}

string SharedNumberGenerator::intToString(int number)
{
	stringstream ss;  //create a stringstream
	ss << number;	//add number to the stream
	return ss.str();	//return a string with the contents of the stream
}

int SharedNumberGenerator::increaseCurrentFrame()
{
	// lock mutex
	boost::unique_lock<boost::mutex> scoped_lock(mtx);

	_currentFrame++;
	return _currentFrame;
}