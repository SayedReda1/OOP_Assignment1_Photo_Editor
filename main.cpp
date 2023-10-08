
// FCAI - OOP Programming - 2023 - Assignment 1
// Program Name:			Grayscale Photo Editor
// Last Modified Date:		4/10/2023
// Author1:					Elsayed Reda		(20220066)
// Author2:					Hesham Abdelnaser	(20220373)
// TA:						XX
// Purpose:					Learning Problem solving and how to manage projects

#include "filters.cpp" // has everything

unsigned char mainImageMatrix[SIZE][SIZE];

void loadImage();
void saveImage();
char optionsDisplay();

int main()
{
	std::cout << "User Beeh Aamel Eeh ^_^\n\n";
	loadImage();

	char option;
	do
	{
		option = optionsDisplay();
		switch (option)
		{
		case '1':
		{
			turnBW(mainImageMatrix);
			break;
		}
		case '2':
		{
			invertFilter(mainImageMatrix);
			break;
		}
		case '3':
		{
			mergeImages(mainImageMatrix);
			break;
		}
		case '4':
		{
			flipFilter(mainImageMatrix);
			break;
		}
		case '5':
		{
			brightnessFilter(mainImageMatrix);
			break;
		}
		case '6':
		{
			rotateImage(mainImageMatrix);
			break;
		}
		case '7':
		{
			detectImageEdges(mainImageMatrix);
			break;
		}
		case 's':
		{
			saveImage();
			break;
		}
		case '0':
		{
			std::cout << "Byee :)" << std::endl;
		}
		}
	} while (option - '0');

	return 0;
}

// ------------------- Functions Declaration ------------------------
void saveImage()
{
	char targetName[100];

	std::cout << "  Please enter target name: ";
	std::cin >> targetName;

	// adding bitmap extension
	strcat(targetName, ".bmp");
	writeGSBMP(targetName, mainImageMatrix);
}

void loadImage()
{
	char imageName[100];

	std::cout << "Please enter image name: ";
	std::cin >> imageName;

	// adding bitmap extension
	strcat(imageName, ".bmp");
	readGSBMP(imageName, mainImageMatrix);
}

char optionsDisplay()
{
	std::cout << "Select your filter number [0 to exit]: " << std::endl;
	std::cout << "  1) Black & White Filter" << std::endl;
	std::cout << "  2) Invert Filter" << std::endl;
	std::cout << "  3) Merge Filter" << std::endl;
	std::cout << "  4) Flip Filter" << std::endl;
	std::cout << "  5) Darken & Enlighten Filter" << std::endl;
	std::cout << "  6) Rotate Image" << std::endl;
	std::cout << "  7) Detect Image Edges" << std::endl;
	// std::cout << "  8) Enlarge Image" << std::endl;
	// std::cout << "  9) Shrink Image" << std::endl;
	// std::cout << "  a) Mirror 1/2 Image" << std::endl;
	// std::cout << "  b) Shuffle Image" << std::endl;
	// std::cout << "  c) Blur Image" << std::endl;
	// std::cout << "  d) Crop Image" << std::endl;
	// std::cout << "  e) Skew Image Right" << std::endl;
	// std::cout << "  f) Skew Image Up" << std::endl;
	std::cout << "  s) Save Image to file" << std::endl;
	std::cout << "  0) Exit" << std::endl;
	std::cout << "Your entry: ";
	char option;
	std::cin >> option;

	return option;
}