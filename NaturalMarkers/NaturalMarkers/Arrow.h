#pragma once

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Arrow {
private:
	string ID;
	Point tip;
	Point tail;
	static int counter;
public:
	Arrow(Point tip, Point tail);
	~Arrow();

	string getID() const;
	int getTipX() const;
	int getTipY() const;
	int getTailX() const;
	int getTailY() const;

	Point getTip() const;
	Point getTail() const;

	void setTip(const Point tip);
	void setTail(const Point tail);
};
