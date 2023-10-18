// Editor Function Declaration
#include <iostream>
#include <functional>
#include <cstring>
#include <cmath>
#include "bmplib.cpp"

#define n SIZE
#define PI 3.14159265

// _________________________________ Helping functions _________________________________

// We assign mat2 to mat1
void assign(unsigned char mat1[n][n], unsigned char mat2[n][n])
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			mat1[i][j] = mat2[i][j];
		}
	}
}

// _________________________________ Black&White _________________________________

// Turn grey scale image to black and white image by checking
// every pixel and turn it to the closer color, whether it's
// black or white.
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

// _________________________________ Inverter _________________________________

// Assign each pixel to its compliment.
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

// _________________________________ Merge Images _____________________________________________
// Merge two image by taking the averge of every two corresponding pixels.
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

// _________________________________ Flip _____________________________________________

// Flip images horizontally by swaping pixels about the y-axis
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

// Flip images vertically by swaping pixels about the x-axis
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

// Main flip function choose from it horzontal or vertical flip
void flipFilter(unsigned char imageMatrix[n][n])
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

// _________________________________ Brightness filter _________________________________

// Change images brightness by increasing or decreasing pixel
// grey value.
void brightnessFilter(unsigned char imageMatrix[n][n])
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
			imageMatrix[i][j] = std::min(255, int(imageMatrix[i][j] * degree));
		}
	}
}

// _________________________________ Rotate _________________________________

// Transpose image matrix and an rotated image by 90 degrees
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

// Rotate function by 90, 180, 270, 360 degrees to the right
// using rotateRight().
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

// _________________________________ Edges detection _________________________________

// Detect edges by turing the image to balck and white first,
// then compare each pixel with the next pixel horizontally and
// vertically with the next pixel to find two different
// consecutive pixels and add the black pixel to the result matrix.
void detectImageEdges(unsigned char imageMatrix[n][n])
{
	turnBW(imageMatrix);
	unsigned char helper[n][n];

	// Vertical looping
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n - 1; ++j)
		{
			if (imageMatrix[j][i] && imageMatrix[j + 1][i])
			{
				helper[j][i] = 255;
			}
			else if (!imageMatrix[j][i] && !imageMatrix[j][i])
			{
				helper[j + 1][i] = 255;
			}
			else if (imageMatrix[j][i])
			{
				helper[j][i] = 255, helper[j + 1][i] = 0;
			}
			else
			{
				helper[j][i] = 0, helper[j + 1][i] = 255;
			}
		}
	}

	// Horizontal looping
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n - 1; ++j)
		{
			if (imageMatrix[i][j] && !imageMatrix[i][j + 1])
			{
				helper[i][j + 1] = 0;
			}
			else if (!imageMatrix[i][j] && imageMatrix[i][j + 1])
			{
				helper[i][j] = 0;
			}
		}

	assign(imageMatrix, helper);
}

// _________________________________ Enlarge a quarter _________________________________

// fill the corresponding 2x2 pixels in helper matrix
void fill(unsigned char matrix[n][n], int x, int y, unsigned char val)
{
	x = (x % (n / 2)) * 2; // corresponding x in helper
	y = (y % (n / 2)) * 2; // corresponding y in helper

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			matrix[i + x][j + y] = val;
		}
	}
}

// Choose one of the four image quarters to enlarge it by filling
// the iamge matrix by that quarter.
void enlargeQ(unsigned char imageMatrix[n][n])
{
	int option;
	std::cout << "> Which quarter to enlarge 1, 2, 3 or 4? ";
	std::cin >> option;

	unsigned char helper[n][n];

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

	assign(imageMatrix, helper);
}

// _________________________________ Shrink Filter _________________________________

// Returns the average of pixel block starting from start pixel
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

// Assign average color of each WxW pixel block to the corresponding
// pixel in helper matrix.
void shrinkImage(unsigned char imageMatrix[n][n])
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

	// Take squares of size w in imageMatrix & get average to put in result
	int w = option[2] - '0'; // size of square
	unsigned char result[n][n];
	memset(result, 255, sizeof(result)); // initializing with white color

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

// _________________________________ Mirror Filter _________________________________

// Reflect about x or y axis depending on user input
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
	if (option == 'L' || option == 'R') // if Left or Right take mirror each row
	{
		for (int i = 0; i < n; ++i)
		{
			l = 0, r = n - 1;
			while (l < r)
			{
				// assigning
				if (reverse)
				{
					imageMatrix[i][l++] = imageMatrix[i][r--];
				}
				else
				{
					imageMatrix[i][r--] = imageMatrix[i][l++];
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < n; ++i) // if Up or Down take mirror each column
		{
			l = 0, r = n - 1;
			while (l < r)
			{
				// assigning
				if (reverse)
				{
					imageMatrix[l++][i] = imageMatrix[r--][i];
				}
				else
				{
					imageMatrix[r--][i] = imageMatrix[l++][i];
				}
			}
		}
	}
}

// _________________________________ Shuffle Image Quarters _________________________________

// Convert the points in range [0, 127] to their correspoding points in all quarters
std::pair<int, int> convertPoint(int i, int j, int quarter)
{
	int x = (quarter / 3) * (n / 2);
	int y = ((quarter % 2) ^ 1) * (n / 2);
	return {i + x, j + y};
}

// Copies a quarter (from) in original matrix
// to quarter (to) in result matrix
void emplace(unsigned char original[n][n], unsigned char result[n][n], int from, int to)
{
	for (int i = 0; i < n / 2; ++i)
	{
		for (int j = 0; j < n / 2; ++j)
		{
			// converting each point to its corresponding one in both original and result mat
			auto [oX, oY] = convertPoint(i, j, from);
			auto [resX, resY] = convertPoint(i, j, to);
			// Assigning each point to its corresponding one
			result[resX][resY] = original[oX][oY];
		}
	}
}

// change image quarter to corresponding user input.
void shuffleImage(unsigned char imageMatrix[n][n])
{
	int order[4];
	std::cout << "> Enter new order of quarters: ";
	for (int i = 0; i < 4; ++i)
		std::cin >> order[i];

	// result matrix
	unsigned char helper[n][n];
	for (int i = 0; i < 4; ++i)
	{
		emplace(imageMatrix, helper, order[i], i + 1);
	}
	assign(imageMatrix, helper);
}

// _________________________________ Blur Image _________________________________

// computes each element from all sides multiplied
// by its corresponding weight in weight matrix
int avr(unsigned char imageMatrix[n][n], double weight[], int x, int y)
{

	// Direction matrix to denote all sides (neighbor pixels)
	int dirx[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
	int diry[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

	int finalValue = 0;
	for (int i = 0; i < 9; ++i)
	{
		int nx = dirx[i] + x, ny = diry[i] + y; // a valid possible side
		if (0 <= nx && nx < n && 0 <= ny && ny < n)
		{
			finalValue += imageMatrix[nx][ny] * weight[i];
		}
	}
	return finalValue;
}

// Multiply each neighbor pixel by its corresponding
// weight and assign their sum to current pixel.
void blur(unsigned char imageMatrix[n][n])
{
	// Our result matrix and weight
	unsigned char result[n][n];

	// Notice that center elements are weighter than other
	// That's to make main color appear a little
	double weight[9] = {0.0625, 0.125, 0.0625, 0.125, 0.256, 0.125, 0.0625, 0.125, 0.0625};

	// Apply the filter 3 times to get a greater degree of blur.
	for (int i = 0; i < 3; ++i)
	{

		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				result[i][j] = avr(imageMatrix, weight, i, j);
			}
		}
	}

	assign(imageMatrix, result);
}

// _________________________________ Crop Image _________________________________

// Determine new image borders according to user input specifications
// and make pixels out of limit white.
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
			if (i < x || i > x + h || j < y || j > y + w)
			{
				imageMatrix[i][j] = 255;
			}
		}
	}
}

// _________________________________ Skew _________________________________

// Skew image horizontally by shifting its pixels to the right by a
// user-input degree in a new matrix then shrink the image horizontally
// to fit into the original size.
void skewHorizontal(unsigned char imageMatrix[n][n])
{
	int degree;
	std::cout << "> Please Enter the degree of skewing: ";
	std::cin >> degree;

	// --- Calculate the number of shifted pixels ---
	int shiftPixels = n * tan((float)degree * PI / 180.0);

	// --- Shift each row to a helper matrix ---
	unsigned char helper[n][n + shiftPixels];
	for (int i = 0; i < n; ++i)
	{
		fill(helper[i], helper[i] + n + shiftPixels, 255); // fill with white
	}

	double shift = shiftPixels, x = (double)shiftPixels / n;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			helper[i][j + (int)shift] = imageMatrix[i][j];
		}

		shift -= x;
	}

	// --- Copy to original matrix with compression ---
	for (int i = 0; i < n; ++i)
	{
		fill(imageMatrix[i], imageMatrix[i] + n, 255); // fill with white
	}

	double step = ((double)n + shiftPixels) / n;
	for (int i = 0; i < n; ++i)
	{
		double k = 0.0;
		for (int j = 0; j < n; ++j, k += step)
		{
			imageMatrix[i][j] = helper[i][(int)k];
		}
	}
}

// Skew image vertically by shifting its pixels down by a
// user-input degree in a new matrix then shrink the image vertically
// to fit into the original size.
void skewVertical(unsigned char imageMatrix[n][n])
{
	int degree;
	std::cout << "> Please Enter the degree of skewing: ";
	std::cin >> degree;

	// --- Calculate the number of shifted pixels ---
	int shiftPixels = n * tan((float)degree * PI / 180.0);

	// --- Shift each column to a helper matrix ---
	unsigned char helper[n + shiftPixels][n];
	for (int i = 0; i < n + shiftPixels; ++i)
	{
		fill(helper[i], helper[i] + n, 255);
	}

	double shift = shiftPixels, x = (double)shiftPixels / n;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			helper[j + (int)shift][i] = imageMatrix[j][i];
		}

		shift -= x;
	}

	// --- Copy to original matrix with compressions ---
	for (int i = 0; i < n; ++i)
	{
		fill(imageMatrix[i], imageMatrix[i] + n, 255); // fill with white
	}

	double step = ((double)n + shiftPixels) / n;
	// k -> helper matrix row pointer
	// i -> imageMatrix row pointer
	double k = 0.0;
	for (int i = 0; i < n; ++i, k += step)
	{
		for (int j = 0; j < n; ++j)
		{
			imageMatrix[i][j] = helper[(int)k][j];
		}
	}
}

// _________________________________ Loading & Display _________________________________
void loadImage(unsigned char mainImageMatrix[][SIZE])
{
	char imageName[100];

	std::cout << "Please enter image name: ";
	std::cin >> imageName;

	// adding bitmap extension
	strcat(imageName, ".bmp");
	readGSBMP(imageName, mainImageMatrix);
}

void saveImage(unsigned char mainImageMatrix[][SIZE])
{
	char targetName[100];

	std::cout << "> Please enter target name: ";
	std::cin >> targetName;

	// adding bitmap extension
	strcat(targetName, ".bmp");
	writeGSBMP(targetName, mainImageMatrix);
}

// Menu display function
char optionsDisplay()
{
	// Prints out the menu display

	std::cout << "Select your filter number [0 to exit]: " << std::endl;
	std::cout << "  1) Black & White Filter" << std::endl;
	std::cout << "  2) Invert Filter" << std::endl;
	std::cout << "  3) Merge Filter" << std::endl;
	std::cout << "  4) Flip Filter" << std::endl;
	std::cout << "  5) Darken & Enlighten Filter" << std::endl;
	std::cout << "  6) Rotate Image" << std::endl;
	std::cout << "  7) Detect Image Edges" << std::endl;
	std::cout << "  8) Enlarge Image" << std::endl;
	std::cout << "  9) Shrink Image" << std::endl;
	std::cout << "  a) Mirror Half Image" << std::endl;
	std::cout << "  b) Shuffle Image" << std::endl;
	std::cout << "  c) Blur Image" << std::endl;
	std::cout << "  d) Crop Image" << std::endl;
	std::cout << "  e) Skew Image Right" << std::endl;
	std::cout << "  f) Skew Image Up" << std::endl;
	std::cout << "  s) Save Image to file" << std::endl;
	std::cout << "  0) Exit" << std::endl;
	std::cout << "Your entry: ";
	char option;
	std::cin >> option;

	return option;
}

// We put all the functions in an array of fucntion pointers
// and choose one according to user input.
void takeAction(unsigned char image[n][n], char option)
{
	// arr of pointers to each function
	std::function<void(unsigned char[n][n])> filters[]{
		turnBW, invertFilter, mergeImages, flipFilter,
		brightnessFilter, rotateImage, detectImageEdges,
		enlargeQ, shrinkImage, mirrorImage, shuffleImage,
		blur, cropImage, skewHorizontal, skewVertical};

	if (option == 's')
	{
		saveImage(image);
	}
	else if (option == '0')
	{
		std::cout << "BYE BYE :)";
	}
	else
	{
		if (::isdigit(option))
		{
			filters[option - '0' - 1](image);
		}
		else
		{
			filters[option - 'a' + 9](image);
		}
	}
}