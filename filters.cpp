
// Editor Function Declaration
#include <iostream>
#include <cstring>
#include "bmplib.cpp"

// Black&White _________________________________________
void turnBW(unsigned char imageMatrix[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (imageMatrix[i][j] > 127)
				imageMatrix[i][j] = 255;
			else
				imageMatrix[i][j] = 0;
		}
	}
}

// Flilp _____________________________________________
void horizontalFlip(unsigned char imageMatrix[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{
		int l = 0, r = SIZE - 1;
		while (l < r)
		{
			std::swap(imageMatrix[i][l++], imageMatrix[i][r--]);
		}
	}
}

void verticalFlip(unsigned char imageMatrix[SIZE][SIZE])
{
	for (int col = 0; col < SIZE; ++col)
	{
		int upRow = 0, downRow = SIZE - 1;
		while (upRow < downRow)
		{
			std::swap(imageMatrix[upRow++][col], imageMatrix[downRow--][col]);
		}
	}
}

void flipFilter(unsigned char imageMatrix[SIZE][SIZE])
{
}

// Inverter _____________________________________________
void invertFilter(unsigned char imageMatrix[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			imageMatrix[i][j] = 255 - imageMatrix[i][j];
		}
	}
}

// Merge Images _____________________________________________
void mergeImages(unsigned char imageMatrix[SIZE][SIZE])
{
	char otherImageName[100];
	unsigned char otherMatrix[SIZE][SIZE];

	// Take the other image name
	std::cout << "  Enter image name to merge with: ";
	std::cin >> otherImageName;

	// add .bmp to it
	strcat(otherImageName, ".bmp");
	readGSBMP(otherImageName, otherMatrix);

	// Merging
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			imageMatrix[i][j] = (otherMatrix[i][j] + imageMatrix[i][j]) / 2;
		}
	}
}

// Rotate _____________________________________________
void rotateRight(unsigned char imageMatrix[SIZE][SIZE])
{
	unsigned char tempImage[SIZE][SIZE];

	// for each row in main image
	for (int i = 0, tempCol = SIZE; i < SIZE; ++i, --tempCol)
		// copy each row in image to be last column in newImage
		for (int j = 0; j < SIZE; ++j)
			tempImage[j][tempCol] = imageMatrix[i][j];

	// Copying new image to main image
	for (int i = 0; i < SIZE; ++i)
		for (int j = 0; j < SIZE; ++j)
			imageMatrix[i][j] = tempImage[i][j];
}

void rotateImage(unsigned char imageMatrix[SIZE][SIZE])
{
	int degree;
	std::cout << "  Rotate 90, 180, 270, or 360? ";
	std::cin >> degree;

	// Diving by 90 to get exact number of right rotations
	degree /= 90;

	while (degree--)
		rotateRight(imageMatrix);
}

// Brightness filter _____________________________________
void brightnessFilter(unsigned char imageMatrix[SIZE][SIZE])
{
	char option;
	std::cout << "  (D)arken or (L)ighten Image? ";
	std::cin >> option;

	// Deciding what degree of lightening and darkening
	int degree = (option == 'D' ? -50 : 50);

	// Add degree of brightness to each pixel
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			imageMatrix[i][j] = std::max(0, std::min(255, imageMatrix[i][j] + degree));
		}
	}
}

// Edges detection _______________________________________
void detectImageEdges(unsigned char imageMatrix[SIZE][SIZE])
{
	turnBW(imageMatrix);
	unsigned char helper[SIZE][SIZE];
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE - 1; ++j)
		{
			if (imageMatrix[j][i] && imageMatrix[j + 1][i])
				helper[j][i] = 255;
			else if (!imageMatrix[j][i] && !imageMatrix[j][i])
				helper[j + 1][i] = 255;
			else if (imageMatrix[j][i])
				helper[j][i] = 255, helper[j + 1][i] = 0;
			else
				helper[j][i] = 0, helper[j + 1][i] = 255;
		}
	}

	for (int i = 0; i < SIZE; ++i)
		for (int j = 0; j < SIZE - 1; ++j)
		{
			if (imageMatrix[i][j] && !imageMatrix[i][j + 1])
				helper[i][j + 1] = 0;
			else if (!imageMatrix[i][j] && imageMatrix[i][j + 1])
				helper[i][j] = 0;
		}

	for (int i = 0; i < SIZE; ++i)
		for (int j = 0; j < SIZE; ++j)
			imageMatrix[i][j] = helper[i][j];
}

// Enlarge a quarter _____________________________________
void enlargeQ(unsigned char imageMatrix[SIZE][SIZE])
{
	int option;
	cout << "Which quarter to enlarge 1, 2, 3 or 4? ";
	cin >> option;

	unsigned char helper[SIZE][SIZE];
	int n = SIZE;
	auto x = (option / 3) * (n / 2),
		 y = ((option % 2) ^ 1) * (n / 2);

	for (int i = x; i < x + n / 2; ++i)
		for (int j = y; j < y + n / 2; ++j)
		{
			helper[(i - x) * 2][(j - y) * 2] = imageMatrix[i][j];
			if (i < n - 1)
				helper[(i - x) * 2 + 1][(j - y) * 2] = imageMatrix[i][j];
			if (j < n - 1)
				helper[(i - x) * 2][(j - y) * 2 + 1] = imageMatrix[i][j];
			if (i < n - 1 && j < n - 1)
				helper[(i - x) * 2 + 1][(j - y) * 2 + 1] = imageMatrix[i][j];
		}

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			imageMatrix[i][j] = helper[i][j];
}

void blur(unsigned char imageMatrix[SIZE][SIZE])
{
	unsigned char helper[SIZE][SIZE];
	// UL, U, UR, L, R, DL, D, DR
	// pair<int, int> dir[] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
	for (int i = 0; i < SIZE; ++i)
		for (int j = 0; j < SIZE; ++j)
		{
			imageMatrix[i][j] = (imageMatrix[i][j] + 127) / 2;
		}
}