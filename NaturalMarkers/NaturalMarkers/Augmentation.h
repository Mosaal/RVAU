#pragma once

#include <iostream>
#include <filesystem>

#include "Image.h"

using namespace std;
namespace fs = experimental::filesystem;

class Augmentation {
private:
public:
	// Constructor and destructor
	Augmentation(const string imgPath);
	~Augmentation();

	// Methods
	void start();
};
