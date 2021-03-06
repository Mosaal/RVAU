#include "Arrow.h"

// Static counter
int Arrow::counter = 0;

// Constructor and destructor
Arrow::Arrow(const Point tip, const Point tail): tip(tip), tail(tail) { ID = "A" + to_string(counter++); }

Arrow::~Arrow() {}

// Getters
string Arrow::getID() const { return ID; }

int Arrow::getTipX() const { return tip.x; }

int Arrow::getTipY() const { return tip.y; }

int Arrow::getTailX() const { return tail.x; }

int Arrow::getTailY() const { return tail.y; }

Point Arrow::getTip() const { return tip; }

Point Arrow::getTail() const { return tail; }

// Setters
void Arrow::setTip(const Point tip) { this->tip = tip; }

void Arrow::setTail(const Point tail) { this->tail = tail; }
