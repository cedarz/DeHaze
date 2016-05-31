#include "deHaze.h"
#include <algorithm>


bool operator> (const struct Pixel & a, const struct Pixel & b) {
	return a.intensity > b.intensity;
}


void DeHaze::loadImage(const char * path) {
	src.release();
	src = cv::imread(path);
	assert(src.data);
	cout << "Imgage successfully loaded from " << path << endl;
	cout << "image size : " << src.rows << " " << src.cols << " " << src.depth() << endl;
	//cout << src.type() << endl;
	//cout << src.channels() << endl;
	tran.release();
	dark.release();
	dst.release();
	//dst.create(src.rows, src.cols, src.type());
	src.copyTo(dst);
	//cv::imshow("dst", dst);
	//cv::waitKey(0);
	tran.create(src.rows, src.cols, CV_32FC1);
	dark.create(src.rows, src.cols, CV_8UC1);
	gtran.create(src.rows, src.cols, CV_32FC1);
	//cout << src << endl;

}


///Single Image Haze Removal Using Dark Channel Prior
void DeHaze::getDarkChannelPrior() {
	//cout << src.channels() << " " << dark.channels() << endl;
	//cout << src.rows << " " << src.cols << endl;
	//cout << dark.rows << " " << dark.cols << endl;
	cout << "Starting to retrieve the dark channel prior of the image..." << endl;
	for (int i = 0; i < src.rows; ++i) {
		for (int j = 0; j < src.cols; ++j) {
			uchar pixel = 255;
			Region roi(max(0, i - Radius), max(0, j - Radius), min(i + Radius, src.rows - 1), min(j + Radius, src.cols - 1));
			for (int h = roi.rmin; h <= roi.rmax; ++h) {
				for (int w = roi.cmin; w <= roi.cmax; ++w) {
					cv::Vec3b vbc = src.at<cv::Vec3b>(h, w);
					pixel = min(pixel, vbc[0]);
					pixel = min(pixel, vbc[1]);
					pixel = min(pixel, vbc[2]);
				}
			}
			dark.at<uchar>(i, j) = pixel;
		}
	}
	//showImage("dark", dark);
}
void DeHaze::getTransmission() {
	cout << "Starting to compute the transmission matrix..." << endl;
	for (int i = 0; i < src.rows; ++i) {
		for (int j = 0; j < src.cols; ++j) {
			float pixel = 255.0;
			Region roi(max(0, i - Radius), max(0, j - Radius), min(i + Radius, src.rows - 1), min(j + Radius, src.cols - 1));
			for (int h = roi.rmin; h <= roi.rmax; ++h) {
				for (int w = roi.cmin; w <= roi.cmax; ++w) {
					cv::Vec3b vbc = src.at<cv::Vec3b>(h, w);
					pixel = min(pixel, float(int(vbc[0])) / Alight[0]);
					pixel = min(pixel, float(int(vbc[1])) / Alight[1]);
					pixel = min(pixel, float(int(vbc[2])) / Alight[2]);
				}
			}
			//cout << pixel << endl;
			tran.at<float>(i, j) = 1.0 - 0.95 * pixel;
			//cout << (1.0 - 0.95 * pixel) << " : " << tran.at<float>(i, j) << endl;
		}
	}
	//showImage("tran", tran);
}

void DeHaze::softMatting() {
	
}

void DeHaze::gFilter() {
	//cv::cvtColor(src, guided, cv::COLOR_BGR2GRAY);
	///
	//cv::Mat res;
	//res.create(tran.rows, tran.cols, CV_8UC1);
	//for (int i = 0; i < tran.rows; ++i) {
	//	for (int j = 0; j < tran.cols; ++j) {
	//		uchar tmp = uchar((tran.at<float>(i, j) > 1.0 ? 1.0 : tran.at<float>(i, j)) *255 + 0.5);
	//		//cout << "t : " << tran.at<float>(i, j) << " tmp : " << (int)tmp << endl;
	//		res.at<uchar>(i, j) = tmp;
	//	}
	//}
	//guidedFilter(res, src, 30, 0.001);
	gtran = guidedFilter(src, tran, 20, 0.001);
	//std::cout << gtran << std::endl;
	//for (int i = 0; i < res.rows; ++i) {
	//	for (int j = 0; j < res.cols; ++j) {
	//		float tmp = float(gtran.at<uchar>(i, j)) / 255.0;
	//		//cout << "t : " << tran.at<float>(i, j) << " tmp : " << (int)tmp << endl;
	//		gtran.at<float>(i, j) = tmp;
	//	}
	//}
	cv::imshow("gtran", gtran);
	cv::waitKey(0);
}

//这个地方因为数据类型的原因浪费时间
void DeHaze::recoverSceneRadiace(){
	cout << "g channel : " << tran.channels() << endl;
	cout << "Startint to dehaze the image..." << endl;
	for (int i = 0; i < src.rows; ++i) {
		for (int j = 0; j < src.cols; ++j) {
			cv::Vec3b & dv = dst.at<cv::Vec3b>(i, j);
			//cv::Vec3b & sv = src.at<cv::Vec3b>(i, j);
			float t = std::max(gtran.at<float>(i, j), 0.1f);
			cv::Vec3f tmp;// = (dv - Alight) / t;
			for (int k = 0; k < 3; k++) {
				tmp[k] = (float(dv[k]) - float(Alight[k])) / t;
			}
			//dst.at<cv::Vec3b>(i, j) = (src.at<cv::Vec3b>(i, j) - Alight) / std::max(tran.at<float>(i, j), 0.1f) + Alight;

			cv::Vec3b res(uchar(tmp[0]) + Alight[0], uchar(tmp[1]) + Alight[1], uchar(tmp[2]) + Alight[2]);
			dv = res;
			//cout << dv << endl;
		}
	}
	//cout << dst << endl;
}
void DeHaze::getAtmosphericLight(){
	cout << "Starting to compute the atmospheric light intensity..." << endl;
	std::priority_queue<Pixel, std::vector<Pixel>, std::greater<Pixel>> pq;
	int num = src.rows * src.cols * 0.001;
	for (int i = 0; i < dark.rows; ++i) {
		for (int j = 0; j < dark.cols; ++j) {
			Pixel p(dark.at<uchar>(i, j), i, j);
			pq.push(p);
			if (pq.size() > num) {
				pq.pop();
			}
		}
	}
	unsigned int A[3] = {};
	//uchar A[3] = {0, 0, 0};

	///maximum
	//while (!pq.empty()) {
	//	Pixel tmp = pq.top();
	//	//printf("%d (%d %d)\n", tmp.intensity, tmp.i, tmp.j);
	//	cv::Vec3b vcb = src.at<cv::Vec3b>(tmp.i, tmp.j);
	//	//A[0] += vcb[0], A[1] += vcb[1], A[2] += vcb[2];
	//	A[0] = max(A[0], vcb[0]);
	//	A[1] = max(A[1], vcb[1]);
	//	A[2] = max(A[2], vcb[2]);
	//	//printf("%d %d %d\n", vcb[0], vcb[1], vcb[2]);
	//	pq.pop();
	//}
	////cout << int(A[0]) << " " << int(A[1]) << " " << int(A[2]) << endl;
	//Alight[0] = A[0];// / num;
	//Alight[1] = A[1];// / num;
	//Alight[2] = A[2];// / num;

	/// mean
	while (!pq.empty()) {
		Pixel tmp = pq.top();
		//printf("%d (%d %d)\n", tmp.intensity, tmp.i, tmp.j);
		cv::Vec3b vcb = src.at<cv::Vec3b>(tmp.i, tmp.j);
		A[0] += vcb[0], A[1] += vcb[1], A[2] += vcb[2];
		printf("%d %d %d\n", vcb[0], vcb[1], vcb[2]);
		pq.pop();
	}
	cout << int(A[0]) << " " << int(A[1]) << " " << int(A[2]) << endl;
	Alight[0] = A[0] / num;
	Alight[1] = A[1] / num;
	Alight[2] = A[2] / num;

	//cout << num << " " << pq.size() << endl;
}


////
void DeHaze::showImage(const char * wname, MatType mt) {
	switch (mt) {
	case DeHaze::SRC:
		cv::imshow(wname, src);
		break;
	case DeHaze::DST:
		cv::imshow(wname, dst);
		break;
	case DeHaze::DARK:
		cv::imshow(wname, dark);
		break;
	case DeHaze::TRAN:
		cv::imshow(wname, tran);
		break;
	case DeHaze::GTRAN:
		cv::imshow(wname, gtran);
		break;
	}
	if (cv::waitKey() == 27) {
		cv::destroyAllWindows();
	}
}


void DeHaze::saveImage(const char * wname, MatType mt) {

}
void DeHaze::getImageName(string & fp) {
	
}