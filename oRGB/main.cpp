// Copyright (C) 2020 Sirn Nguyen Truong <sonnt174@gmail.com>

// oRGB.cpp : Defines the entry point for the console application.
//

#define cimg_use_jpeg
#include <iostream>
#include "Eigen\Dense"
#include <vector>
#include <math.h>
#include "ORGBSpace.h"
#include "Utility.h"
#include "windows.h"

using namespace cimg_library;
using namespace std;

/*
	Description: helper function to handle input from user
	Input: filepath
	Output: 0 - invalid input
			1 - valid input
*/
int handleInput(std::string filepath) {
	if (filepath == "quit") {
		exit(0);
		return 0;
	}

	if (false == Utility::isFileExist(filepath)) {
		cout << " File \"" << filepath << "\" is not found.\n";
		cout << " You are in: "; system("cd");
		cout << " Please try with other.\n";
		return 0;
	}

	if ("jpg" != Utility::getFileExtension(filepath)) {
		cout << " Not support for file format: *." << Utility::getFileExtension(filepath) << "\n";
		cout << " As of now this program only supports *.jpg format\n";
		return 0;
	}

	return 1;
}


/*
	Description: main function
	Input: 
	Output:
*/
int main() {
	std::string filepath;
	while (true) {

		cout << "\n\nEnter file path of image(*jpg) or \"quit\" to exit: \n  ";
		cin >> filepath;
		if (0 == handleInput(filepath)) {
			continue;
		}
		cout << " processing..." << filepath << "\n";

		CImg<unsigned char> image;
		try {
			image = CImg<unsigned char>(filepath.c_str());
		}
		catch (exception e){
			continue;
		}

		// driver code to change rgb to orgb and then adjust the color
		// display and save the output to "Result" folder
		int width = image.width();
		int height = image.height();
		std::vector<unsigned char> rgb(width*height * 3);
		rgb = Utility::cimgToRgb(image);
		ORGBSpace orgbspace(rgb);
		float mean = orgbspace.calcOverallMeanValue();
		cout << " Offset = " << mean << endl;
		CImg<unsigned char> grid;
		float offsets[2] = { 0, -1 };
		for (int rg = 1; rg > -2; rg--) {
			CImg<unsigned char> row;
			for (int yb = -1; yb < 2; yb++) {
				offsets[0] = yb * mean;
				offsets[1] = rg * mean;
				std::vector<unsigned char> adjrgb = orgbspace.adjustOverall_toRGB(offsets);
				CImg<unsigned char> nwimg = Utility::rgbToCImg(adjrgb, width, height);
				row = row.append(nwimg);
			}
			grid = grid.append(row, 'y');
		}
		
		std::string filename = Utility::getFileName(filepath);
		Utility::saveResultImage(grid, filename);
		grid.display();
	}

	return 0;
}