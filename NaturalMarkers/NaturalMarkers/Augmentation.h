#pragma once

#include <vector>
#include <sstream>
#include <iostream>
#include <filesystem>

#include "Image.h"

class Augmentation {
private:

public:
	// Constructor and destructor
	Augmentation(const string imgPath);
	~Augmentation();

	// Methods
	void start();
};
