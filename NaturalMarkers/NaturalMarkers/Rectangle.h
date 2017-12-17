#pragma once

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Rectangle {
private:
	// Private properties
	string ID;
	Rect rect;

	// Static counter
	static int counter;
public:
	// Constructor and destructor
	Rectangle(const Rect rect);
	~Rectangle();

	// Getters
	string getID() const;
	Rect getRect() const;

	int getRectX() const;
	int getRectY() const;
	int getRectWidth() const;
	int getRectHeight() const;

	// Setters
	void setID(const string ID);
	void setRect(const Rect rect);
};
