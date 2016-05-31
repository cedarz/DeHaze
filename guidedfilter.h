#ifndef GUIDED_FILTER_H
#define GUIDED_FILTER_H

#include <opencv2/opencv.hpp>

class GuidedFilterImpl;

class GuidedFilter
{
public:
	GuidedFilter(const cv::Mat &I, int r, double eps);
	~GuidedFilter();

	cv::Mat filter(const cv::Mat &p, int depth = -1) const;

private:
	GuidedFilterImpl *impl_;
};

// -guidance image : I(should be a gray - scale / single channel image) or color imgage
// -filtering input image : p(should be a gray - scale / single channel image)
// -local window radius : r
// -regularization parameter : eps
cv::Mat guidedFilter(const cv::Mat &I, const cv::Mat &p, int r, double eps, int depth = -1);

//cv::Mat guidedFilterSrc(const cv::Mat & I, const cv::Mat & p, int r, double eps, int depth = -1);

#endif