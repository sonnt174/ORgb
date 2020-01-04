// Copyright (C) 2020 Sirn Nguyen Truong <sonnt174@gmail.com>

#include "stdafx.h"
#include "Utility.h"
#include "windows.h"

namespace Utility {
	CImg<unsigned char> rgbToCImg(const std::vector<unsigned char> &rgb, int width, int height) {
		CImg<unsigned char> ans(width, height, 1, 3, 0);
		for (int r = 0; r < width; r++)
			for (int c = 0; c < height; c++) {
				ans.draw_point(r, c, &rgb[r * height * 3 + c * 3]);
			}
		return ans;
	}

	std::vector<unsigned char> cimgToRgb(const CImg<unsigned char> &image) {
		int width = image.width();
		int height = image.height();
		std::vector<unsigned char> ans(width * height * 3);
		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++) {
				ans[i * height * 3 + j * 3] = image(i, j, 0, 0);
				ans[i * height * 3 + j * 3 + 1] = image(i, j, 0, 1);
				ans[i * height * 3 + j * 3 + 2] = image(i, j, 0, 2);
			}
		return ans;
	}

	bool isFileExist(const std::string& name) {
		std::ifstream f(name.c_str());
		return f.good();
	}

	std::string getFileName(const std::string& filepath) {
		int pos1 = filepath.find_last_of("\\") + 1;
		int pos2 = filepath.find_last_of(".");
		std::string fileName = filepath.substr(pos1, pos2 - pos1);
		return fileName;
	}

	std::string getFileExtension(const std::string& filepath) {
		return filepath.substr(filepath.find_last_of(".") + 1);
	}

	void saveResultImage(const CImg<unsigned char> &grid, std::string &filename) {
		if (CreateDirectoryA("Result", NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError()) {
			// CopyFile(...)
		} else {
			// Failed to create directory.
		}

		filename = "Result\\" + filename + "_adjusted" + ".jpg";
		grid.save_jpeg(filename.c_str());
		std::cout << "\n\n  ==> Saved result to: " << filename << "\n\n";
	}
}