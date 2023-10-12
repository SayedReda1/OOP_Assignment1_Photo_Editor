// Editor Function Declaration
#include <iostream>
#include <cstring>
#include "bmplib.cpp"

#define n SIZE

// a funtion to assign given mat2 to given mat1
void assign(unsigned char mat1[n][n], unsigned char mat2[n][n]) {
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			mat1[i][j] = mat2[i][j];
		}
	}
}

// Black&White _________________________________________
void turnBW(unsigned char imageMatrix[n][n])
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (imageMatrix[i][j] > 127)
				imageMatrix[i][j] = 255;
			else
				imageMatrix[i][j] = 0;
		}
	}
}

// Flip _____________________________________________
void horizontalFlip(unsigned char imageMatrix[n][n])
{
	for (int i = 0; i < n; ++i)
	{
		int l = 0, r = n - 1;
		while (l < r)
		{
			std::swap(imageMatrix[i][l++], imageMatrix[i][r--]);
		}
	}
}

void verticalFlip(unsigned char imageMatrix[n][n])
{
	for (int col = 0; col < n; ++col)
	{
		int upRow = 0, downRow = n - 1;
		while (upRow < downRow)
		{
			std::swap(imageMatrix[upRow++][col], imageMatrix[downRow--][col]);
		}
	}
}

void flipFilter(unsigned char imageMatrix[n][n])
{
	// Take input
	char option;
	std::cout << "> (H)orizontally or (V)ertically? ";
	std::cin >> option;
	option = ::toupper(option);		// in case the user enters lowercase letters

	switch (option)
	{
		case 'H':
		{
			horizontalFlip(imageMatrix);
			break;
		}
		case 'V':
		{
			verticalFlip(imageMatrix);
			break;
		}
		default:
		{
			std::cout << "! Invalid Input" << std::endl;
		}
	}
}

// Inverter _____________________________________________
void invertFilter(unsigned char imageMatrix[n][n])
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			// Takes the complement of the color number
			imageMatrix[i][j] = 255 - imageMatrix[i][j];
		}
	}
}

// Merge Images _____________________________________________
void mergeImages(unsigned char imageMatrix[n][n])
{
	char otherImageName[100];
	unsigned char otherMatrix[n][n];

	// Take the other image name
	std::cout << "> Enter image name to merge with: ";
	std::cin >> otherImageName;

	// add .bmp to it
	strcat(otherImageName, ".bmp");
	readGSBMP(otherImageName, otherMatrix);

	// Merging
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			imageMatrix[i][j] = (otherMatrix[i][j] + imageMatrix[i][j]) / 2;
		}
	}
}

// Rotate _____________________________________________
void rotateRight(unsigned char imageMatrix[n][n])
{
	unsigned char helper[n][n];

	// for each row in main image
	for (int i = 0; i < n; ++i)
	{
		// copy each row in image to be last column in newImage
		for (int j = 0; j < n; ++j)
		{
			helper[j][n - i - 1] = imageMatrix[i][j];
		}
	}

	assign(imageMatrix, helper);
}

void rotateImage(unsigned char imageMatrix[n][n])
{
	int degree;
	std::cout << "> Rotate 90, 180, 270, or 360? ";
	std::cin >> degree;

	// Diving by 90 to get exact number of right rotations
	degree /= 90;

	while (degree--)
	{
		rotateRight(imageMatrix);
	}
}

// Brightness filter _____________________________________
void brightnessFilter(unsigned char imageMatrix[n][n])
{
	char option;
	std::cout << "> (D)arken or (L)ighten Image? ";
	std::cin >> option;

	// Deciding to decrease to half or adding half
	double degree = (option == 'D' ? 0.5 : 1.5);

	// Add degree of brightness to each pixel
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			imageMatrix[i][j] = std::min(255, int(imageMatrix[i][j] * degree));
		}
	}
}

// Edges detection _______________________________________
void detectImageEdges(unsigned char imageMatrix[n][n])
{
	turnBW(imageMatrix);
	unsigned char helper[n][n];
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n - 1; ++j)
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

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n - 1; ++j)
		{
			if (imageMatrix[i][j] && !imageMatrix[i][j + 1])
				helper[i][j + 1] = 0;
			else if (!imageMatrix[i][j] && imageMatrix[i][j + 1])
				helper[i][j] = 0;
		}

	assign(imageMatrix, helper);
}

// Enlarge a quarter _____________________________________
void fill(unsigned char matrix[SIZE][SIZE], int x, int y, unsigned char val) {
	// fill the corresponding 4 pixels in helper matrix
	x = (x % (n/2)) * 2;
	y = (y % (n/2)) * 2;

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			matrix[i+x][j+y] = val;
		}
	}
}
void enlargeQ(unsigned char imageMatrix[n][n])
{
	int option;
	std::cout << "> Which quarter to enlarge 1, 2, 3 or 4? ";
	std::cin >> option;

	unsigned char helper[n][n];

	// starting point on image
	auto x = (option / 3) * (n / 2),
		 y = ((option % 2) ^ 1) * (n / 2);

	for (int i = x; i < x + n / 2; ++i)
	{
		for (int j = y; j < y + n / 2; ++j)
		{
			fill(helper, i, j, imageMatrix[i][j]);
		}
	}

	assign(imageMatrix, helper);
}

// Shrink Filter ________________________________________
int avr(unsigned char imageMatrix[n][n], std::pair<int, int> start, int width)
{
	int sum = 0;
	for (int i = start.first; i < start.first + width; ++i)
	{
		for (int j = start.second; j < start.second + width; ++j)
		{
			sum += imageMatrix[i][j];
		}
	}

	return sum / (width * width);
}

void shrinkImage(unsigned char imageMatrix[n][n])
{
	char option[10];
	std::cout << "> Shrink to (1/2), (1/3), or (1/4)? ";
	std::cin >> option;
	// check for input validation
	if (strncmp(option, "1/2", 3) && strncmp(option, "1/3", 3) && strncmp(option, "1/4", 3))
	{
		std::cout << "! Invalid Input" << std::endl;
		return;
	}

	// Take squares of size w in imageMatrix & get average to put in result
	int w = option[2] - '0'; // size of square
	unsigned char result[n][n];
	memset(result, 255, sizeof result); // initializing with white color

	for (int i = 0; i < n - w + 1; i += w)
	{
		for (int j = 0; j < n - w + 1; j += w)
		{
			// Assigning cur result pixel with the average of the square
			result[i / w][j / w] = avr(imageMatrix, {i, j}, w);
		}
	}
	assign(imageMatrix, result);
}

// Mirror Filter____________________________________________
void mirrorImage(unsigned char imageMatrix[n][n])
{
	// input
	char option;
	std::cout << "> Which half to mirror? -- (L)eft, (R)ight, (U)p, (D)own: ";
	std::cin >> option;
	option = ::toupper(option); // turn to uppercase

	// determines if to assign left arrow to right arrow or vice versa
	bool reverse = (option == 'R' || option == 'D');

	int l, r;
	if (option == 'L' || option == 'R')		// if Left or Right take mirror each row
	{
		l = 0, r = n - 1;
		for (int i = 0; i < n; ++i)
		{
			while (l < r)
			{
				// assigning
				if (reverse)
					imageMatrix[i][l++] = imageMatrix[i][r--];
				else
					imageMatrix[i][r--] = imageMatrix[i][l++];
			}
		}
	}
	else
	{
		for (int i = 0; i < n; ++i)		// if Up or Down take mirror each column
		{
			while (l < r)
			{
				// assigning
				if (reverse)
					imageMatrix[l++][i] = imageMatrix[r--][i];
				else
					imageMatrix[r--][i] = imageMatrix[l++][i];
			}
			l = 0, r = n - 1;
		}
	}
}

// Shuffle Image Quarters ______________________________
std::pair<int, int> convertPoint(int i, int j, int quarter)
{
	// Convert the points in range [0, 127] to their correspoding points in all quarters
	int x = (quarter / 3) * (n/2);
	int y = ((quarter % 2)^1) * (n/2);
	return {i+x, j+y};
}

void emplace(unsigned char original[n][n], unsigned char result[n][n], int from, int to) {
	// Copies a quarter (from) in original matrix
	// to quarter (to) in result matrix
	for (int i = 0; i < n/2; ++i)
	{
		for (int j = 0; j < n/2; ++j)
		{
			// converting each point to its corresponding one in both original and result mat
			auto [oX, oY] = convertPoint(i, j, from);
			auto [resX, resY] = convertPoint(i, j, to);
			// Assigning each point to its corresponding one
			result[resX][resY] = original[oX][oY];
		}
	}
}

void shuffleImage(unsigned char imageMatrix[n][n])
{
	int order[4];
	std::cout << "> Enter new order of quarters: ";
	for (int i = 0; i < 4; ++i) std::cin >> order[i];

	// result matrix
	unsigned char helper[n][n];
	for (int i = 0; i < 4; ++i)
	{
		emplace(imageMatrix, helper, order[i], i+1);
	}
	assign(imageMatrix, helper);
}

// Blur Image __________________________________________
int avr(unsigned char imageMatrix[n][n], double weight[3][3], int i, int j) {
	// computes each element from all sides multiplied
	// by its corresponding weight in weight matrix

	// Direction matrix to denote all sides (neighbor pixels)
	int dir[][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

	double finalValue = 0.0;	// used double in order to not lose any numbers
	for (int x = 0; x < 9; ++x)
	{
		int nx = dir[x][0] + i, ny = dir[x][1] + j;		// a valid possible side
		if (0 <= nx && nx < n && 0 <= ny && ny < n)
		{
			// weight i = x / 3 | weight j = x % 3
			finalValue += imageMatrix[nx][ny] * weight[x / 3][x % 3];
		}
	}
	return finalValue;
}
void blur(unsigned char imageMatrix[n][n])
{
	// Our result matrix and weight
	unsigned char result[n][n];

	// Notice that center elements are weighter than other
	// That's to make main color appear a little
	double weight[3][3] = {
		{0.0625, 0.125, 0.0625},
		{0.125, 0.256, 0.125},
		{0.0625, 0.125, 0.0625}
	};


	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			result[i][j] = avr(imageMatrix, weight, i, j);
		}
	}

	assign(imageMatrix, result);
}

// Crop Image _____________________________________________
void cropImage(unsigned char imageMatrix[n][n])
{
	int x, y, w, h;
	std::cout << "> Enter x, y, width, and height: ";
	std::cin >> x >> y >> w >> h;

	// croped image limits:
	// x <= i <= x + h
	// y <= j <= y + w
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			// Turn any pixel out the limits into white
			if ((x <= i && i <= x + h) && (y <= j && j <= y + w))
				continue;
			imageMatrix[i][j] = 255;
		}
	}
}