#pragma once
#include "Core.h"

class SharedNumberGenerator{
private:
	// Mutex to prevent race conditions
	boost::mutex mtx; 

	// The current frame number
	int _currentFrame; 

public:
	/************************************************************************/
	/* Constructor. Initializes socket object as null.
	/************************************************************************/
	SharedNumberGenerator(string serverName = "UNDEFINED");

	/************************************************************************/
	/* Increases the frame number and returns the current frame number after 
	/* the increment.
	/* NOTE: this function is thread-safe
	/*
	/* @return the current frame after the increment.
	/************************************************************************/
	int increaseCurrentFrame();

	/************************************************************************/
	/* Helper function for converting int to string.
	/*
	/* @in number - the number to convert to string.
	/* @return the int given as string.
	/************************************************************************/
	string intToString(int number);

	/************************************************************************/
	/* Returns the current frame number.
	/* 
	/* @return the current frame number.
	/************************************************************************/
	int getCurrentFrame(){ return _currentFrame; }

};
