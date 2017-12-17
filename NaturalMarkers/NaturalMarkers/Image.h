#pragma once

#include <vector>
#include <opencv2\opencv.hpp>

#include "Arrow.h"
#include "Label.h"
#include "Rectangle.h"

using namespace cv;
using namespace std;

class Image {
private:
	Mat img, placeholder;
	vector<Arrow> arrows;
	vector<Label> labels;
	vector<Rectangle> rects;
public:
	Image(Mat placeholder);
	~Image();
};

