#pragma once

#include <iostream>
#include <filesystem>

#include "Image.h"
#include "SubSet.h"

using namespace std;
using namespace cv::xfeatures2d;
namespace fs = experimental::filesystem;

#define DEBUG_ON   0
#define NO_DELAY   0
#define MULTIPLIER 3
#define ARROW_SIZE 50
#define RED        Scalar(0, 0, 255)
#define WHITE      Scalar(255, 255, 255)

class Augmentation {
private:
	// Private properties
	string fileName;
	vector<SubSet> subSets;

	Mat sceneImg;
	Mat sceneDescriptors;
	vector<KeyPoint> sceneKeyPoints;
public:
	// Public property
	int debug;

	// Constructor and destructor
	Augmentation(const int debug, const string imgPath);
	~Augmentation();

	// Methods
	void start();
	void detectFeatures();
	void match();
	void localize();
	void display();
};
