
// FCAI - OOP Programming - 2023 - Assignment 1
// Program Name:			Grayscale Photo Editor
// Last Modified Date:		18/10/2023
// Author1:					Elsayed Reda		(20220066)	(sydr38734@gmail.com)
// Author2:					Hesham Abdelnaser	(20220373)	(abdelnaserhesham166@gmail.com)
// Section:					S3
// TA:						NA
// Purpose:					Learning Problem solving and how to manage projects

#include "filters.cpp" // has everything
#include <functional>

unsigned char imageMatrix[SIZE][SIZE];


int main()
{
	// Welcoming
	std::cout << "User Beeh, Aamel Eeh ^_^\n\n";
	loadImage(imageMatrix);	// load image bitmap

	// keep taking input till user inputs '0'
	char option;
	do {
		option = optionsDisplay();						// Displays menu window
		takeAction(imageMatrix, option);				// takes user input and apply appropriate filter

	} while (option != '0');

	return 0;
}