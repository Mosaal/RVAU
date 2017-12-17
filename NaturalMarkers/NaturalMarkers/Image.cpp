#include "Image.h"

Image::Image(Mat placeholder): placeholder(placeholder) {
	this->placeholder.copyTo(img);
}

Image::~Image() {}
