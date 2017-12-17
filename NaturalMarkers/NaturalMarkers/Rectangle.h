#pragma once

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Rectangle {
private:
	string ID;
	Rect rect;
	static int counter;
public:
	Rectangle(Rect rect);
	~Rectangle();

	string getID() const;
	Rect getRect() const;

	int getRectX() const;
	int getRectY() const;
	int getRectWidth() const;
	int getRectHeight() const;

	void setID(const string ID);
	void setRect(const Rect rect);
};
