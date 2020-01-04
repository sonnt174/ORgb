// Copyright (C) 2020 Sirn Nguyen Truong <sonnt174@gmail.com>

#ifndef __ORGB_SPACE_H__
#define __ORGB_SPACE_H__

#include <memory>
#include <vector>
#include <Eigen\Dense>
#include <iostream>

const float M_PI = 3.1415926535f;

typedef std::vector<unsigned char> RGBDataUChar;
typedef std::vector<float> ORGBDataFloat;

class ORGBSpace {
public:
	/*
		Description: ORGBSpace constructor
		Input:
		Output:
	*/
	ORGBSpace();

	/*
		Description: ORGBSpace destroctor
		Input:
		Output:
	*/
	~ORGBSpace();

	/*
		Description: ORGBSpace constructor with param
		Input:
		Output:
	*/
	ORGBSpace(const RGBDataUChar &);

	/*
		Description: ORGBSpace constructor with param
		Input:
		Output:
	*/
	ORGBSpace(const ORGBDataFloat &);

	/*
		Description: copy constructor
		Input:
		Output:
	*/
	ORGBSpace(const ORGBSpace &other);

	/* 
		Description: Create ORGB space from RGB data of type unsigned char
		Input: RGBDataUChar
		Output: int
	*/
	int fromRGB(const RGBDataUChar &rgb);

	/*
		Description: Create ORGB space from RGB data of type float
		Input: ORGBDataFloat
		Output: int
	*/
	int fromRGB(const ORGBDataFloat &rgb);

	/*
		Description: Convert self ORGB data to RGB data
		Input:
		Output: RGB data in unsigned char
	*/
	RGBDataUChar toRGB_uchar(); // return unsigned char data

	/*
		Description: Convert self ORGB data to RGB data
		Input:
		Output: RGB data in float
	*/
	ORGBDataFloat toRGB_float(); // return float data

	/*
		Description: Convert external ORGB data to RGB data
		Input: ORGBDataFloat
		Output: RGB data in unsigned char
	*/
	RGBDataUChar toRGB_uchar(const ORGBDataFloat &orgb);

	/*
		Description: Convert external ORGB data to RGB data
		Input: ORGBDataFloat
		Output: RGB data in float
	*/
	ORGBDataFloat toRGB_float(const ORGBDataFloat &orgb);

	/*
		Description: Calculate mean values of yb axis and rg axis
		Input: 
		Output: mean value of yb in val[0], rg in val[1]
	*/
	int calcMeanValues(float val[2]);

	/*
		Description: Calculate mean values of overall orgb data
		Input:
		Output: overall mean value
	*/
	float calcOverallMeanValue();

	/*
		Description: Adjust color of YB(yellow-blue) axis
		Input: ORGB data to be changed, offset value
		Output: Adjusted ORGB
	*/
	static ORGBDataFloat adjustYBColor(const ORGBDataFloat &orgb, float offset);

	/*
		Description: Adjust color of RG(red-green) axis
		Input: ORGB data to be changed, offset value
		Output: Adjusted ORGB
	*/
	static ORGBDataFloat adjustRGColor(const ORGBDataFloat &orgb, float offset);

	/*
		Description: Adjust overall in both YB and RG axis
		Input: ORGB data to be changed, offsets value
		Output: Adjusted ORGB
	*/
	static ORGBDataFloat adjustOverall(const ORGBDataFloat &orgb, float offests[2]);

	/*
		Description: Adjust overall in both YB and RG axis
		Input: ORGB data to be changed, yb offset value, rg offset value
		Output: Adjusted ORGB
	*/
	static ORGBDataFloat adjustOverall(const ORGBDataFloat &orgb, float ybOffet, float rgOffset);

	/*
		Description: Adjust overall in both YB and RG axis of self orgb
		Input: offests value
		Output: Adjusted ORGB
	*/
	ORGBDataFloat adjustOverall(float offests[2]);

	/*
		Description: Adjust overall in both YB and RG axis of self orgb
		Input: offests value
		Output: Adjusted ORGB
	*/
	RGBDataUChar adjustOverall_toRGB(float offests[2]);

	/*
		Description: Adjust overall in both YB and RG axis of self orgb by mean value
		Input: sign of axes to be adjusted
		Output: Adjusted ORGB
	*/
	ORGBDataFloat adjustOverallByMean(int adjustAxes[2]);

	/*
		Description: Adjust overall in both YB and RG axis of self orgb by mean value
		Input: sign of axes to be adjusted
		Output: RGB data in unsigned char after adjusting
	*/
	RGBDataUChar adjustOverallByMean_toRGB(int adjustAxes[2]);
	/*
	Description: Adjust overall in both YB and RG axis of self orgb by mean value
	Input: sign of axes to be adjusted
	Output: RGB data in float after adjusting
	*/
	ORGBDataFloat adjustOverallByMean_toRGBfloat(int adjustAxes[2]);

protected:
	/*
		Description: Fill transform matrix
		Input: 
		Output:
	*/
	void fillTransMat();

	/*
		Description: convert RGB data to LCC
		Input: 
		Output: lcc data
	*/
	int rgbToLcc(ORGBDataFloat &rgbData);

	/*
		Description: convert lcc data to orgb data
		Input:
		Output: ORGB data
	*/
	int lccToOrgb(ORGBDataFloat &lcc);

	/*
	Description: convert orgb data to lcc data, 
				 this is intermediate step to inverse orgb to rbg
	Input:
	Output: lcc data
	*/
	int orgbToLcc(ORGBDataFloat &lcc);

	/*
		Description: convert lcc data to rgb data,
		this is final step to inverse orgb to rbg
		Input:
		Output: rgb data
	*/
	int lccToRgb(ORGBDataFloat &rgbData);

private:
	static Eigen::Matrix3f _transMat;
	ORGBDataFloat _oRgbData;
};
#endif