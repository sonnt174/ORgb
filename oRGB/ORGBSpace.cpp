// Copyright (C) 2020 Sirn Nguyen Truong <sonnt174@gmail.com>

#include "ORGBSpace.h"

Eigen::Matrix3f ORGBSpace::_transMat;

ORGBSpace::ORGBSpace() {
	fillTransMat();
}

ORGBSpace::~ORGBSpace() {

}

ORGBSpace::ORGBSpace(const RGBDataUChar &rgbdata) {
	fillTransMat();
	fromRGB(rgbdata);
}

ORGBSpace::ORGBSpace(const ORGBDataFloat &rgbdata) {
	fillTransMat();
	fromRGB(rgbdata);
}

ORGBSpace::ORGBSpace(const ORGBSpace &other) {
	this->_oRgbData = other._oRgbData;
}

void ORGBSpace::fillTransMat() {
	_transMat(0, 0) = 0.299f;
	_transMat(0, 1) = 0.587f;
	_transMat(0, 2) = 0.114f;
	_transMat(1, 0) = 0.500f;
	_transMat(1, 1) = 0.500f;
	_transMat(1, 2) = -1.000f;
	_transMat(2, 0) = 0.866f;
	_transMat(2, 1) = -0.866f;
	_transMat(2, 2) = 0.000f;
}

int ORGBSpace::fromRGB(const RGBDataUChar &rgb) {
	size_t sz = rgb.size();
	_oRgbData.resize(sz);
	for (size_t i = 0; i < sz; i++) {
		_oRgbData[i] = (float)rgb[i];
	}
	rgbToLcc(_oRgbData);
	lccToOrgb(_oRgbData);

	return 1;
}

int ORGBSpace::fromRGB(const ORGBDataFloat &rgb) {
	_oRgbData = rgb; // copy rgb data to process
	rgbToLcc(_oRgbData);
	lccToOrgb(_oRgbData);

	return 1;
}

RGBDataUChar ORGBSpace::toRGB_uchar() {
	ORGBDataFloat rgb = _oRgbData;
	orgbToLcc(rgb);
	lccToRgb(rgb);
	size_t sz = rgb.size();
	RGBDataUChar ans(sz);
	for (size_t i = 0; i < sz; i++) {
		ans[i] = (unsigned char)rgb[i];
	}
	return ans;
}

ORGBDataFloat ORGBSpace::toRGB_float() {
	ORGBDataFloat rgb = _oRgbData;
	orgbToLcc(rgb);
	lccToRgb(rgb);
	return rgb;
}

int ORGBSpace::rgbToLcc(ORGBDataFloat &rgbData) {
	size_t sz = rgbData.size();
	for (size_t i = 0; i < sz; i += 3) {
		Eigen::Vector3f v = _transMat * Eigen::Vector3f(rgbData[i], rgbData[i + 1], rgbData[i + 2]);
		(rgbData)[i] = v[0];
		(rgbData)[i + 1] = v[1];
		(rgbData)[i + 2] = v[2];
	}

	return 1;
}

int ORGBSpace::lccToOrgb(ORGBDataFloat &lcc) {
	size_t sz = lcc.size();
	for (size_t i = 0; i < sz; i += 3) {
		float theta = atan2f(lcc[i + 2], lcc[i + 1]);
		bool belowYBAxis = false;
		if (theta < 0.0) {
			theta = -theta;
			belowYBAxis = true;
		}
		if (theta < M_PI / 3.f) {
			theta = theta * .5f; // theta - (theta / 2) = 0.5 * theta
		} else {
			theta = M_PI * .5f + 0.75f * (theta - M_PI / 3.0f) - theta;
		}

		if (belowYBAxis) {
			theta = -theta;
		}
		Eigen::Rotation2Df rot(theta);
		
		Eigen::Vector2f v2d(lcc[i + 1], lcc[i + 2]);
		v2d = rot * v2d;
		lcc[i] = lcc[i];
		lcc[i + 1] = v2d[0];
		lcc[i + 2] = v2d[1];
	}

	return 1;
}

int ORGBSpace::orgbToLcc(ORGBDataFloat &orgb) {
	size_t sz = orgb.size();
	for (size_t i = 0; i < sz; i += 3) {
		bool belowYBAxis = false;
		float theta = atan2f(orgb[i + 2], orgb[i + 1]);
		if (theta < 0.f) {
			theta = -theta;
			belowYBAxis = true;
		}
		if (theta < M_PI / 2.f - 1e-6) {
			theta = theta * (2.0f / 3.0f) - theta;
		} else {
			theta = M_PI / 3.0f + 4.0f * (theta - M_PI / 2.0f) / 3.0f - theta;
		}

		if (belowYBAxis) {
			theta = -theta;
		}
		Eigen::Rotation2Df rot(theta);
		
		Eigen::Vector2f v2d(orgb[i + 1], orgb[i + 2]);
		v2d = rot * v2d;
		//orgb[i] = orgb[i];
		orgb[i + 1] = v2d[0];
		orgb[i + 2] = v2d[1];
	}

	return 1;
}

int ORGBSpace::lccToRgb(ORGBDataFloat &lcc) {
	Eigen::Matrix3f inv = _transMat.inverse();
	for (size_t i = 0; i < lcc.size(); i += 3) {
		Eigen::Vector3f v = inv * Eigen::Vector3f(lcc[i], lcc[i + 1], lcc[i + 2]);
		v[0] = std::max(0.f, v[0]);
		v[0] = std::min(255.f, v[0]);
		v[1] = std::max(0.f, v[1]);
		v[1] = std::min(255.f, v[1]);
		v[2] = std::max(0.f, v[2]);
		v[2] = std::min(255.f, v[2]);

		lcc[i] = v[0];
		lcc[i + 1] = v[1];
		lcc[i + 2] = v[2];
	}

	return 1;
}

ORGBDataFloat ORGBSpace::adjustOverallByMean(int adjustAxes[2]) {
	ORGBDataFloat orgb = _oRgbData;
	size_t sz = orgb.size();
	float meanYB = 0.0f, meanRG = 0.0f;
	for (size_t i = 0; i < sz; i += 3) {
		meanYB += orgb[i + 1];
		meanRG += orgb[i + 2];
	}
	meanYB /= (sz / 3.0f);
	meanRG /= (sz / 3.0f);
	for (size_t i = 0; i < sz; i += 3) {
		orgb[i + 1] = orgb[i + 1] + meanYB * adjustAxes[0];
		orgb[i + 2] = orgb[i + 2] + meanRG * adjustAxes[1];

		// make sure that yb and rg axes value in range [-255, 255]
		orgb[i + 1] = std::min(255.f, orgb[i + 1]);
		orgb[i + 1] = std::max(-255.f, orgb[i + 1]);
		orgb[i + 2] = std::min(255.f, orgb[i + 2]);
		orgb[i + 2] = std::max(-255.f, orgb[i + 2]);
	}

	return orgb;
}

RGBDataUChar ORGBSpace::toRGB_uchar(const ORGBDataFloat &orgb) {
	ORGBDataFloat rgb = orgb;
	orgbToLcc(rgb);
	lccToRgb(rgb);
	size_t sz = orgb.size();
	RGBDataUChar ans(orgb.size());
	for (size_t i = 0; i < sz; i++) {
		ans[i] = (unsigned char)rgb[i];
	}

	return ans;
}

ORGBDataFloat ORGBSpace::toRGB_float(const ORGBDataFloat &orgb) {
	ORGBDataFloat rgb = orgb;
	orgbToLcc(rgb);
	lccToRgb(rgb);
	return rgb;
}

int ORGBSpace::calcMeanValues(float val[2]) {
	size_t sz = _oRgbData.size();
	float meanYB = 0.0f, meanRG = 0.0f;
	for (size_t i = 0; i < sz; i += 3) {
		meanYB += _oRgbData[i + 1];
		meanRG += _oRgbData[i + 2];
	}
	meanYB /= (sz / 3.0f);
	meanRG /= (sz / 3.0f);

	val[0] = meanYB;
	val[1] = meanRG;

	return 1;
}

float ORGBSpace::calcOverallMeanValue() {
	size_t sz = _oRgbData.size();
	float ans = 0.0f;
	for (size_t i = 0; i < sz; i+= 3) {
		ans += _oRgbData[i];
		ans += _oRgbData[i + 1];
		ans += _oRgbData[i + 2];
	}
	ans /= sz;

	return ans;
}

ORGBDataFloat ORGBSpace::adjustYBColor(const ORGBDataFloat &orgb, float offset) {
	ORGBDataFloat nworgb = orgb;
	size_t sz = nworgb.size();
	for (size_t i = 1; i < sz; i += 3) {
		nworgb[i] += offset;
		// make sure that the value is in range [-255, 255]
		nworgb[i] = std::min(255.f, nworgb[i]);
		nworgb[i] = std::max(-255.f, nworgb[i]);
	}

	return nworgb;
}

ORGBDataFloat ORGBSpace::adjustRGColor(const ORGBDataFloat &orgb, float offset) {
	ORGBDataFloat nworgb = orgb;
	size_t sz = nworgb.size();
	for (size_t i = 2; i < sz; i += 3) {
		nworgb[i] += offset;
		// make sure that the value is in range [-255, 255]
		nworgb[i] = std::min(255.f, nworgb[i]);
		nworgb[i] = std::max(-255.f, nworgb[i]);
	}

	return nworgb;
}

ORGBDataFloat ORGBSpace::adjustOverall(float offsets[2]) {
	return adjustOverall(_oRgbData, offsets);
}

RGBDataUChar ORGBSpace::adjustOverall_toRGB(float offsets[2]) {
	return toRGB_uchar(adjustOverall(offsets));
}

ORGBDataFloat ORGBSpace::adjustOverall(const ORGBDataFloat &orgb, float offsets[2]) {
	ORGBDataFloat nworgb = orgb;
	size_t sz = nworgb.size();
	for (size_t i = 1; i < sz; i += 3) {
		nworgb[i] += offsets[0];
		nworgb[i + 1] += offsets[1];

		// make sure that yb and rg axes value in range [-255, 255]
		nworgb[i] = std::min(255.f, nworgb[i]);
		nworgb[i] = std::max(-255.f, nworgb[i]);
		nworgb[i + 1] = std::min(255.f, nworgb[i + 1]);
		nworgb[i + 1] = std::max(-255.f, nworgb[i + 1]);
	}

	return nworgb;
}
ORGBDataFloat ORGBSpace::adjustOverall(const ORGBDataFloat &orgb, 
									   float ybOffet, float rgOffset) {
	ORGBDataFloat nworgb = orgb;
	size_t sz = nworgb.size();
	for (size_t i = 1; i < sz; i += 3) {
		nworgb[i] += ybOffet;
		nworgb[i + 1] += rgOffset;

		// make sure that yb and rg axes value in range [-255, 255]
		nworgb[i] = std::min(255.f, nworgb[i]);
		nworgb[i] = std::max(-255.f, nworgb[i]);
		nworgb[i + 1] = std::min(255.f, nworgb[i + 1]);
		nworgb[i + 1] = std::max(-255.f, nworgb[i + 1]);
	}

	return nworgb;
}

RGBDataUChar ORGBSpace::adjustOverallByMean_toRGB(int adjustAxes[2]) {
	ORGBDataFloat nwOrgb = adjustOverallByMean(adjustAxes);
	return toRGB_uchar(nwOrgb);
}

ORGBDataFloat ORGBSpace::adjustOverallByMean_toRGBfloat(int adjustAxes[2]) {
	ORGBDataFloat nwOrgb = adjustOverallByMean(adjustAxes);
	return toRGB_float(nwOrgb);
}