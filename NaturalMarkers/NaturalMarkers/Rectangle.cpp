#include "Rectangle.h"

int Rectangle::counter = 0;

Rectangle::Rectangle(Rect rect): rect(rect) {
	ID = "R" + to_string(counter++);
}

Rectangle::~Rectangle() {}

string Rectangle::getID() const { return ID; }

Rect Rectangle::getRect() const { return rect; }

int Rectangle::getRectX() const { return rect.x; }

int Rectangle::getRectY() const { return rect.y; }

int Rectangle::getRectWidth() const { return rect.width; }

int Rectangle::getRectHeight() const { return rect.height; }

void Rectangle::setID(const string ID) { this->ID = ID; }

void Rectangle::setRect(const Rect rect) { this->rect = rect; }
