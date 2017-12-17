#pragma once

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Arrow {
private:
	// Private properties
	string ID;
	Point tip;
	Point tail;

	// Static counter
	static int counter;
public:
	// Constructor and destructor
	Arrow(const Point tip, const Point tail);
	~Arrow();

	// Getters
	string getID() const;
	int getTipX() const;
	int getTipY() const;
	int getTailX() const;
	int getTailY() const;

	Point getTip() const;
	Point getTail() const;

	// Setters
	void setTip(const Point tip);
	void setTail(const Point tail);
};
