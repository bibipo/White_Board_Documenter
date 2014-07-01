#pragma once

#include "Core.h"
#include "Controller.h"

void printApplicationTitle();
void printApplicationMenu();

int main(){
	// Print application title
	printApplicationTitle();

	// Would contain the selection of the user
	int userSelection;

	// Main loop would run until the user would choose valid selection
	do{
		printApplicationMenu();
		cin >> userSelection;

		if (userSelection == FINISH )
			return EXIT_SUCCESS;

		// Create controller
		Controller ctrlr = Controller();

		switch (userSelection)
		{
		case CAPTURE_FROM_CAMREA:
			ctrlr.captureFromCamera();
			return EXIT_SUCCESS;

		case CAPTURE_FROM_FILE:
			// The address to the video file
			std::string fileAddress;

			// Get the file adress from the user
			cout << "Please enter the full path to the video file:" << endl;
			cin >> fileAddress;

			// Initilize capture
			//ctrlr.captureFromFile(fileAddress);
			ctrlr.captureFromFile("v1.wmv");

			return EXIT_SUCCESS;
		}
	}while( true );
}

void printApplicationTitle(){
	printf("%s\n\n","==============================================================================");
	printf("%s%.2f%s\n",	"			   White Board Documenter version:",VERSION,"					");
	printf("%s\n",	"==============================================================================");
}

void printApplicationMenu(){
	printf("Please select the action you would like to preform:\n");
	printf("1. Capture from live stream\n");
	printf("2. Capture from file\n");
	printf("3. Exit\n");
}