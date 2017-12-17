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
	// Private properties
	Mat placeholder;
	vector<string> IDs;
	vector<Arrow> arrows;
	vector<Label> labels;
	vector<Rectangle> rects;
public:
	// Public property
	Mat img;

	// Constructor and destructor
	Image(const Mat placeholder);
	~Image();

	// Methods
	void updateImage();
	void updateLabel(const int index, const string text);

	void removeID(const int index);
	void removeArrow(const int index);
	void removeLabel(const int index);
	void removeRect(const int index);

	// Getters
	Mat getPlaceholder() const;

	string getID(const int index) const;
	Arrow getArrow(const int index) const;
	Label getLabel(const int index) const;
	Rectangle getRect(const int index) const;

	vector<string> getIDs() const;
	vector<Arrow> getArrows() const;
	vector<Label> getLabels() const;
	vector<Rectangle> getRects() const;

	// Setters
	void setPlaceholder(const Mat placeholder);

	void addID(const string ID);
	void addArrow(const Arrow arrow);
	void addLabel(const Label label);
	void addRect(const Rectangle rect);

	void setIDs(const vector<string> IDs);
	void setArrows(const vector<Arrow> arrows);
	void setLabels(const vector<Label> labels);
	void setRects(const vector<Rectangle> rects);
};

