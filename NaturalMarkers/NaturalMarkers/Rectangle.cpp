#include "Rectangle.h"

// Static counter
int Rectangle::counter = 0;

// Constructor and destructor
Rectangle::Rectangle(Rect rect): rect(rect) { ID = "R" + to_string(counter++); }

Rectangle::~Rectangle() {}

// Getters
string Rectangle::getID() const { return ID; }

Rect Rectangle::getRect() const { return rect; }

int Rectangle::getRectX() const { return rect.x; }

int Rectangle::getRectY() const { return rect.y; }

int Rectangle::getRectWidth() const { return rect.width; }

int Rectangle::getRectHeight() const { return rect.height; }

// Setters
void Rectangle::setID(const string ID) { this->ID = ID; }

void Rectangle::setRect(const Rect rect) { this->rect = rect; }
