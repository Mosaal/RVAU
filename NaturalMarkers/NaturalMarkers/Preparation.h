#pragma once

#include <vector>
#include <sstream>
#include <iostream>
#include <filesystem>

#include "Image.h"

using namespace std;
namespace fs = experimental::filesystem;

#define BACKSPACE  8
#define PREVIOUS   9
#define ENTER      13
#define ESC        27
#define NEXT       32

#define NO_PARAM   0
#define DELAY      1
#define SCALE      2
#define THICKNESS  2
#define ARROW_SIZE 50
#define RED        Scalar(0, 0, 255)
#define WHITE      Scalar(255, 255, 255)

class Preparation {
private:
	// Private properties
	int llx;
	string mode;
	string window;
	vector<char> possibleChars;
public:
	// Public properties
	Point org;
	Rect rect;

	vector<Image> imgs;

	int currImg = 0;
	bool DRAWING = false;
	bool WRITING = false;
	string labelText = "";

	// Constructor and destructor
	Preparation(const string dirPath);
	~Preparation();

	// Methods
	void start();
	void update();
	void display();
	void updateMode();
	void handleInput(const int input);

	void removeAttribute(const string ID);
	int indexOfChar(const int input) const;
	int indexOfArrow(const string ID) const;
	int indexOfLabel(const string ID) const;
	int indexOfRectangle(const string ID) const;
};
