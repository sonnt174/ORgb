// Copyright (C) 2020 Sirn Nguyen Truong <sonnt174@gmail.com>

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "CImg.h"
#include "Eigen\Dense"
#include <vector>
#include <math.h>
#include "ORGBSpace.h"
#include <fstream>

using namespace cimg_library;

namespace Utility {
	/*
		Description: Utility function to convert rgb data to CImg<unsigned char>
		Input: RGB data, width, height
		Output: CImg
	*/
	CImg<unsigned char> rgbToCImg( const std::vector<unsigned char> &rgb, int width, int height);

	/*
		Description: Utility function to decode RGB data from CImg
		Input: CImg
		Output: RGB data
	*/
	std::vector<unsigned char> cimgToRgb(const CImg<unsigned char> &cimg);

	/*
		Description: Utility function to check wether file is existing or not
		Input: file name
		Output: true  - file existing
		false - file is not exitsing
	*/
	bool isFileExist(const std::string& name);

	/*
		Description: Utility function to check wether file is existing or not
		Input: file name
		Output: true  - file existing
		false - file is not exitsing
	*/
	std::string getFileName(const std::string& filepath);

	/*
		Description: Utility function to get the extension of file
		Input: file path
		Output: file's extension
	*/
	std::string getFileExtension(const std::string& filepath);

	/*
		Description: Utility function to save composed image to file
		Input: CImg composed image, original filename
		Output: void
	*/
	void saveResultImage(const CImg<unsigned char> &grid, std::string &filename);
};
#endif