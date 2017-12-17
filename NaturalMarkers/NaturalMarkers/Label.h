#pragma once

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Label {
private:
	string ID;
	string text;
	Point center;
	Point llCorner;
	Rect boundingBox;
	static int counter;
public:
	Label(string text, Point center);
	~Label();

	string getID() const;
	string getText() const;
	Point getCenter() const;
	Point getLLCorner() const;
	Rect getBoundingBox() const;

	int getCenterX() const;
	int getCenterY() const;

	void setText(const string text);
	void setCenter(const Point center);
};
