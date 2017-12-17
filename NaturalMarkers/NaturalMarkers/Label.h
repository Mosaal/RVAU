#pragma once

#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

#define SCALE     2
#define THICKNESS 2
#define OFFSET    20

class Label {
private:
	// Private properties
	string ID;
	string text;
	Point center;
	Point llCorner;
	Rect boundingBox;

	// Static counter
	static int counter;
public:
	// Constructor and destructor
	Label(const string text, const Point center);
	~Label();

	// Methods
	void updateLabel();

	// Getters
	string getID() const;
	string getText() const;
	Point getCenter() const;
	Point getLLCorner() const;
	Rect getBoundingBox() const;

	int getCenterX() const;
	int getCenterY() const;

	// Setters
	void setText(const string text);
	void setCenter(const Point center);
};
