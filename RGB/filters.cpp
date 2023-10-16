// Editor Function Declaration
#include <iostream>
#include <cstring>
#include <cmath>
#include <numeric>
#include "bmplib.cpp"

#define n SIZE
#define PI 3.14159265

// --------- Helping functions -----------
void assignPixel(unsigned char pix1[3], unsigned char pix2[3]) {
	for (int i = 0; i < 3; ++i) {
		pix1[i] = pix2[i];
	}
}

void assignMat(unsigned char mat1[n][n][3], unsigned char mat2[n][n][3])
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			assignPixel(mat1[i][j], mat2[i][j]);
		}
	}
}

// Black&White _________________________________________
int sum(unsigned char pixel[3])
{
	int sum = 0;
	for (int i = 0; i < 3; ++i) {
		sum += pixel[i];
	}
	return sum;
}

void turnBW(unsigned char imageMatrix[n][n][3])
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int avr = sum(imageMatrix[i][j]) / 3;	// rgb average
			if (avr > 127)
				imageMatrix[i][j][0] = imageMatrix[i][j][1] = imageMatrix[i][j][2] = 255;
			else
				imageMatrix[i][j][0] = imageMatrix[i][j][1] = imageMatrix[i][j][2] = 0;
		}
	}
}


// Inverter _____________________________________________
void invertFilter(unsigned char imageMatrix[n][n][3])
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			// Takes the complement of the color number
			imageMatrix[i][j][0] = 255 - imageMatrix[i][j][0];
			imageMatrix[i][j][1] = 255 - imageMatrix[i][j][1];
			imageMatrix[i][j][2] = 255 - imageMatrix[i][j][2];
		}
	}
}

// Merge Images _____________________________________________
void mergeImages(unsigned char imageMatrix[n][n][3])
{
	char otherImageName[100];
	unsigned char otherMatrix[n][n][3];

	// Take the other image name
	std::cout << "> Enter image name to merge with: ";
	std::cin >> otherImageName;

	// add .bmp to it
	strcat(otherImageName, ".bmp");
	readRGBBMP(otherImageName, otherMatrix);

	// Merging
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			// Average of each color component
			imageMatrix[i][j][0] = (otherMatrix[i][j][0] + imageMatrix[i][j][0]) / 2;
			imageMatrix[i][j][1] = (otherMatrix[i][j][1] + imageMatrix[i][j][1]) / 2;
			imageMatrix[i][j][2] = (otherMatrix[i][j][2] + imageMatrix[i][j][2]) / 2;
		}
	}
}

// Flip _____________________________________________
void horizontalFlip(unsigned char imageMatrix[n][n][3])
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

void verticalFlip(unsigned char imageMatrix[n][n][3])
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

void flipFilter(unsigned char imageMatrix[n][n][3])
{
	// Take input
	char option;
	std::cout << "> (H)orizontally or (V)ertically? ";
	std::cin >> option;
	option = ::toupper(option); // in case the user enters lowercase letters

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

// Brightness filter _____________________________________
void brightnessFilter(unsigned char imageMatrix[n][n][3])
{
	char option;
	std::cout << "> (D)arken or (L)ighten Image? ";
	std::cin >> option;
	option = ::toupper(option);

	// Deciding to decrease to half or adding half
	double degree = (option == 'D' ? 0.5 : 1.5);

	// Add degree of brightness to each pixel
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			imageMatrix[i][j][0] = std::min(255, int(imageMatrix[i][j][0] * degree));
			imageMatrix[i][j][2] = std::min(255, int(imageMatrix[i][j][2] * degree));
			imageMatrix[i][j][1] = std::min(255, int(imageMatrix[i][j][1] * degree));
		}
	}
}

// Rotate _____________________________________________
void rotateRight(unsigned char imageMatrix[n][n][3])
{
	unsigned char helper[n][n][3];

	// for each row in main image
	for (int i = 0; i < n; ++i)
	{
		// copy each row in image to be last column in newImage
		for (int j = 0; j < n; ++j)
		{
			assignPixel(helper[j][n - i - 1], imageMatrix[i][j]);
		}
	}

	assignMat(imageMatrix, helper);
}

void rotateImage(unsigned char imageMatrix[n][n][3])
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

// Edges detection _______________________________________
void detectImageEdges(unsigned char imageMatrix[n][n][3])
{
	turnBW(imageMatrix);
	unsigned char helper[n][n][3];
	unsigned char black[]{0, 0, 0}, white[]{255, 255, 255};

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n - 1; ++j)
		{
			if (imageMatrix[j][i][0] && imageMatrix[j + 1][i][0])
			{
				assignPixel(helper[j][i], white);
			}
			else if (!imageMatrix[j][i][0] && !imageMatrix[j][i][0])
			{
				assignPixel(helper[j + 1][i], white);
			}
			else if (imageMatrix[j][i][0])
			{
				assignPixel(helper[j][i], white);
				assignPixel(helper[j + 1][i], black);
			}
			else
			{
				assignPixel(helper[j][i], black);
				assignPixel(helper[j + 1][i], white);
			}
		}
	}

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n - 1; ++j)
		{
			if (imageMatrix[i][j][0] && !imageMatrix[i][j + 1][0])
			{
				assignPixel(helper[i][j + 1], black);
			}
			else if (!imageMatrix[i][j][0] && imageMatrix[i][j + 1][0])
			{
				assignPixel(helper[i][j], black);
			}
		}
	}

	assignMat(imageMatrix, helper);
}

// Enlarge a quarter _____________________________________
void fill(unsigned char matrix[n][n][3], int x, int y, unsigned char val[3])
{
	// fill the corresponding 2x2 pixels in helper matrix
	x = (x % (n / 2)) * 2;	// corresponding x in helper
	y = (y % (n / 2)) * 2;	// corresponding y in helper

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			assignPixel(matrix[i + x][j + y], val);
		}
	}
}

void enlargeQ(unsigned char imageMatrix[n][n][3])
{
	int option;
	std::cout << "> Which quarter to enlarge 1, 2, 3 or 4? ";
	std::cin >> option;

	unsigned char helper[n][n][3];

	// starting point on image
	auto x = (option / 3) * (n / 2),
		 y = ((option % 2) ^ 1) * (n / 2);

	// looping over selected quarter and enlarging each pixel 
	// to a 2x2 pixel in helper using fill
	for (int i = x; i < x + n / 2; ++i)
	{
		for (int j = y; j < y + n / 2; ++j)
		{
			fill(helper, i, j, imageMatrix[i][j]);
		}
	}

	assignMat(imageMatrix, helper);
}

// Shrink Filter ________________________________________
void shrinkImage(unsigned char imageMatrix[n][n][3])
{
	char option[5];
	std::cout << "> Shrink to (1/2), (1/3), or (1/4)? ";
	std::cin >> option;
	// check for input validation
	if (strncmp(option, "1/2", 3) != 0 && strncmp(option, "1/3", 3) != 0 && strncmp(option, "1/4", 3) != 0)
	{
		std::cout << "! Invalid Input" << std::endl;
		return;
	}

	// Take each WxW squares in imageMatrix and get its middle color
	// Assign that middle in result as one pixel
	// neglect all other WxW pixels

	int W = option[2] - '0'; 	// size of square to compress
	unsigned char result[n][n][3];
	memset(result, 255, sizeof(result)); 	// initializing with white color

	for (int i = 0; i < n - W + 1; i += W)
	{
		for (int j = 0; j < n - W + 1; j += W)
		{
			// Assigning cur result pixel with the average of the square
			assignPixel(result[i/W][j/W], imageMatrix[i+1][j+1]);
		}
	}
	assignMat(imageMatrix, result);
}

// Mirror Filter____________________________________________
void mirrorImage(unsigned char imageMatrix[n][n][3])
{
	// input
	char option;
	std::cout << "> Which half to mirror? -- (L)eft, (R)ight, (U)p, (D)own: ";
	std::cin >> option;
	option = ::toupper(option); // turn to uppercase

	// determines if to assign left arrow to right arrow or vice versa
	bool reverse = (option == 'R' || option == 'D');

	int l, r;
	if (option == 'L' || option == 'R')
	{
		// if Left or Right take mirror each row
		for (int i = 0; i < n; ++i)
		{
			l = 0, r = n - 1;
			while (l < r)
			{
				// assigning
				if (reverse)
				{
					assignPixel(imageMatrix[i][l++], imageMatrix[i][r--]);
				}
				else
				{
					assignPixel(imageMatrix[i][r--], imageMatrix[i][l++]);
				}
			}
		}
	}
	else
	{
		// if Up or Down take mirror each column
		for (int i = 0; i < n; ++i)
		{
			l = 0, r = n - 1;
			while (l < r)
			{
				// assigning
				if (reverse)
				{
					assignPixel(imageMatrix[l++][i], imageMatrix[r--][i]);
				}
				else
				{
					assignPixel(imageMatrix[r--][i], imageMatrix[l++][i]);
				}
			}
		}
	}
}

// Shuffle Image Quarters ______________________________
std::pair<int, int> convertPoint(int i, int j, int quarter)
{
	// Convert the points in range [0, 127] to their correspoding points in all quarters
	int x = (quarter / 3) * (n / 2);
	int y = ((quarter % 2) ^ 1) * (n / 2);
	return {i + x, j + y};
}

void emplace(unsigned char original[n][n][3], unsigned char result[n][n][3], int from, int to)
{
	// Copies a quarter (from) in original matrix
	// to quarter (to) in result matrix
	for (int i = 0; i < n / 2; ++i)
	{
		for (int j = 0; j < n / 2; ++j)
		{
			// converting each point to its corresponding one in both original and result mat
			auto [oX, oY] = convertPoint(i, j, from);
			auto [resX, resY] = convertPoint(i, j, to);
			// Assigning each point to its corresponding one
			assignPixel(result[resX][resY], original[oX][oY]);
		}
	}
}

void shuffleImage(unsigned char imageMatrix[n][n][3])
{
	int order[4];
	std::cout << "> Enter new order of quarters: ";
	for (int i = 0; i < 4; ++i)
		std::cin >> order[i];

	// result matrix
	unsigned char helper[n][n][3];
	for (int i = 0; i < 4; ++i)
	{
		emplace(imageMatrix, helper, order[i], i + 1);
	}
	assignMat(imageMatrix, helper);
}

// Blur Image __________________________________________
void avr(unsigned char imageMatrix[n][n][3], unsigned char result[3], int x, int y)
{
	// Direction matrix to denote all sides (neighbor pixels)
	int dirx[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
	int diry[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

	int count = 0;		// counting number of components found for division
	int temp[3]{0};		// for color components sum
	for (int i = 0; i < 9; ++i)
	{
		int nx = dirx[i] + x, ny = diry[i] + y; // a valid possible side
		if (0 <= nx && nx < n && 0 <= ny && ny < n)
		{
			temp[0] += imageMatrix[nx][ny][0];
			temp[1] += imageMatrix[nx][ny][1];
			temp[2] += imageMatrix[nx][ny][2];
			count++;
		}
	}
	result[0] = temp[0]/count;
	result[1] = temp[1]/count;
	result[2] = temp[2]/count;
}

void blur(unsigned char imageMatrix[n][n][3])
{
	// Our result matrix and weight
	unsigned char result[n][n][3];

	// for all pixels, get average of each color component from neighbors
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			avr(imageMatrix, result[i][j], i, j);
		}
	}

	assignMat(imageMatrix, result);
}

// Crop Image _____________________________________________
void cropImage(unsigned char imageMatrix[n][n][3])
{
	int x, y, w, h;
	std::cout << "> Enter x, y, width, and height: ";
	std::cin >> x >> y >> w >> h;

	// croped image limits:
	// x <= i <= x + h
	// y <= j <= y + w
	unsigned char white[3]{255, 255, 255};
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			// Turn any pixel out the limits into white
			if (i < x || i > x + h || j < y || j > y + w) {
				assignPixel(imageMatrix[i][j], white);
			}
		}
	}
}

// Skew Horizontal & Vertical __________________________
void assignMatValue(unsigned char mat[n][n][3], unsigned char value)
{
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			mat[i][j][0] = value;
			mat[i][j][1] = value;
			mat[i][j][2] = value;
		}
	}
}
void skewHorizontal(unsigned char imageMatrix[n][n][3])
{
	int degree;
	std::cout << "> Please Enter the degree of skewing: ";
	std::cin >> degree;

	// --- Calculate the number of shifted pixels ---
	int shiftPixels = n * tan((float)degree * PI / 180.0);

	// --- Shift each row to a helper matrix ---
	unsigned char helper[n][n + shiftPixels][3];
	// fill with white
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n+shiftPixels; ++j)
		{
			helper[i][j][0] = helper[i][j][1] = helper[i][j][2] = 255;
		}
	}

	double shift = shiftPixels, x = (double)shiftPixels / n;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			assignPixel(helper[i][j + (int)shift], imageMatrix[i][j]);
		}

		shift -= x;
	}

	// --- Copy to original matrix with compression ---
	assignMatValue(imageMatrix, 255);	// fill with white

	double step = ((double)n + shiftPixels) / n;
	for (int i = 0; i < n; ++i)
	{
		double k = 0.0;
		for (int j = 0; j < n; ++j, k += step)
		{
			assignPixel(imageMatrix[i][j], helper[i][(int)k]);
		}
	}
}

void skewVertical(unsigned char imageMatrix[n][n][3])
{
	int degree;
	std::cout << "> Please Enter the degree of skewing: ";
	std::cin >> degree;

	// --- Calculate the number of shifted pixels --- 
	int shiftPixels = n * tan((float)degree * PI / 180.0);

	// --- Shift each column to a helper matrix ---
	unsigned char helper[n + shiftPixels][n][3];
	// filling with white
	for (int i = 0; i < n+shiftPixels; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			helper[i][j][0] = helper[i][j][1] = helper[i][j][2] = 255;
		}
	}

	double shift = shiftPixels, x = (double)shiftPixels / n;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			assignPixel(helper[j + (int)shift][i], imageMatrix[j][i]);
		}

		shift -= x;
	}

	// --- Copy to original matrix with compressions ---
	assignMatValue(imageMatrix, 255);	// fill with white

	double step = ((double)n + shiftPixels) / n;
	// k -> helper matrix row pointer
	// i -> imageMatrix row pointer
	double k = 0.0;
	for (int i = 0; i < n; ++i, k += step)
	{
		for (int j = 0; j < n; ++j)
		{
			assignPixel(imageMatrix[i][j], helper[(int)k][j]);
		}
	}
}