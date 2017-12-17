#include "Label.h"

// Static counter
int Label::counter = 0;

// Constructor and destructor
Label::Label(const string text, const Point center): text(text), center(center) {
	// Set label's ID
	ID = "L" + to_string(counter++);

	// Update label
	updateLabel();
}

Label::~Label() {}

// Methods
void Label::updateLabel() {
	// Get label text's size
	int baseline = 2;
	Size textSize = getTextSize(this->text, FONT_HERSHEY_PLAIN, SCALE, THICKNESS, &baseline);

	// Calculate the lower left corner of the text
	int llx = this->center.x - (textSize.width / 2);
	int lly = this->center.y + (textSize.height / 2);
	llCorner = Point(llx, lly);

	// Calculate the top left corner of the bounding box
	int ry = this->center.y - (textSize.height / 2) - (OFFSET / 2);
	boundingBox = Rect(llCorner.x - (OFFSET / 2), ry, textSize.width + OFFSET, textSize.height + OFFSET);
}

// Getters
string Label::getID() const { return ID; }

string Label::getText() const { return text; }

Point Label::getCenter() const { return center; }

Point Label::getLLCorner() const { return llCorner; }

Rect Label::getBoundingBox() const { return boundingBox; }

int Label::getCenterX() const { return center.x; }

int Label::getCenterY() const { return center.y; }

// Setters
void Label::setText(const string text) {
	// Set the label's new text
	this->text = text;

	// Update label
	updateLabel();
}

void Label::setCenter(const Point center) { this->center = center; }
