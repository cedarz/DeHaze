#pragma once 
#include <opencv/cv.hpp>
#include <iostream>
#include <algorithm>
#include <functional>
#include <queue>
#include <vector>
//#include <opencv2/core/core.hpp>
#include "guidedfilter.h"

using namespace std;

typedef struct Pixel{
	uchar intensity;
	int i, j;
	Pixel(int inten, int _i, int _j) : intensity(inten), i(_i), j(_j) {}
};
typedef struct Pixel Pixel;
bool operator> (const struct Pixel & a, const struct Pixel & b);

struct Region {
	int rmin, cmin;  //origion left-up
	int rmax, cmax;  // right-bottom
	Region(int _rmin, int _cmin, int _rmax, int _cmax) : rmin(_rmin), cmin(_cmin), rmax(_rmax), cmax(_cmax) {}
};
class DeHaze {
public:
	static const int Radius = 7;
	typedef enum { SRC, DARK, TRAN, GTRAN, DST, NUM } MatType;
private:
	cv::Mat src;
	cv::Mat dst;
	cv::Mat tran;
	cv::Mat dark;
	cv::Vec3b Alight;

	cv::Mat guided;
	cv::Mat gtran;

public:
	//DeHaze(string path);
	void loadImage(const char * path);
	void getDarkChannelPrior();
	void getTransmission();
	void softMatting(); // refine the transmission
	void gFilter();
	void recoverSceneRadiace();
	void getAtmosphericLight(); //require the dark channel prior

	///auxiliary
	string name;
	void showImage(const char * wname, MatType mt);
	void saveImage(const char * wname, MatType mt);
	void getImageName(string & fp);

};