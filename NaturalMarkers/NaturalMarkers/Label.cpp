#include "Label.h"

int Label::counter = 0;

Label::Label(string text, Point center): text(text), center(center) {
	int baseline = 2;
    Size textSize = getTextSize(this->text, FONT_HERSHEY_PLAIN, 2, 2, &baseline);
    int x = this->center.x - (textSize.width / 2);
    int y = this->center.y + (textSize.height / 2);
    llCorner = Point(x, y);
    int ry = this->center.y - (textSize.height / 2) - 10;
    boundingBox = Rect(llCorner.x - 10, ry, textSize.width + 20, textSize.height + 20);
	ID = "L" + to_string(counter++);
}

Label::~Label() {}

string Label::getID() const { return ID; }

string Label::getText() const { return text; }

Point Label::getCenter() const { return center; }

Point Label::getLLCorner() const { return llCorner; }

Rect Label::getBoundingBox() const { return boundingBox; }

int Label::getCenterX() const { return center.x; }

int Label::getCenterY() const { return center.y; }

void Label::setText(const string text) {
	this->text = text;
	int baseline = 2;
	Size textSize = getTextSize(this->text, FONT_HERSHEY_PLAIN, 2, 2, &baseline);
	int x = this->center.x - (textSize.width / 2);
	int y = this->center.y + (textSize.height / 2);
	llCorner = Point(x, y);
	int ry = this->center.y - (textSize.height / 2) - 10;
	boundingBox = Rect(llCorner.x - 10, ry, textSize.width + 20, textSize.height + 20);
}

void Label::setCenter(const Point center) { this->center = center; }
