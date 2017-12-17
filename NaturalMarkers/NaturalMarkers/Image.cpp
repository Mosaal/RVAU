#include "Image.h"

// Constructor and destructor
Image::Image(const Mat placeholder): placeholder(placeholder) { this->placeholder.copyTo(img); }

Image::~Image() {}

// Methods
void Image::updateImage() { placeholder.copyTo(img); }

void Image::updateLabel(const int index, const string text) { labels[index].setText(text); }

void Image::removeID(const int index) { IDs.erase(IDs.begin() + index); }

void Image::removeArrow(const int index) { arrows.erase(arrows.begin() + index); }

void Image::removeLabel(const int index) { labels.erase(labels.begin() + index); }

void Image::removeRect(const int index) { rects.erase(rects.begin() + index); }

// Getters
Mat Image::getPlaceholder() const { return placeholder; }

vector<string> Image::getIDs() const { return IDs; }

vector<Arrow> Image::getArrows() const { return arrows; }

vector<Label> Image::getLabels() const { return labels; }

vector<Rectangle> Image::getRects() const { return rects; }

string Image::getID(const int index) const { return IDs[index]; }

Arrow Image::getArrow(const int index) const { return arrows[index]; }

Label Image::getLabel(const int index) const { return labels[index]; }

Rectangle Image::getRect(const int index) const { return rects[index]; }

// Setters
void Image::setPlaceholder(const Mat placeholder) { this->placeholder = placeholder; }

void Image::addID(const string ID) { IDs.push_back(ID); }

void Image::addArrow(const Arrow arrow) { arrows.push_back(arrow); }

void Image::addLabel(const Label label) { labels.push_back(label); }

void Image::addRect(const Rectangle rect) { rects.push_back(rect); }

void Image::setIDs(const vector<string> IDs) { this->IDs = IDs; }

void Image::setArrows(const vector<Arrow> arrows) { this->arrows = arrows; }

void Image::setLabels(const vector<Label> labels) { this->labels = labels; }

void Image::setRects(const vector<Rectangle> rects) { this->rects = rects; }
