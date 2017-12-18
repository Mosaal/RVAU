#include "Augmentation.h"

// Constructor and destructor
Augmentation::Augmentation(const string imgPath) {
	// Directory with the database, image to be augmented and results' file must have the same name
	Mat img = imread(imgPath, CV_LOAD_IMAGE_COLOR);
	if (!img.data) {
		cout << "ERROR: Invalid file type!" << endl;
		exit(1);
	}

	// Remove file type from image file's name
	int amount = (int)(imgPath.size() - 4);
	string fileName = imgPath.substr(0, amount);

	// Check if directory exists
	fs::path dir(fileName);
	if (!fs::is_directory(dir)) {
		cout << "ERROR: No directory found with the database!" << endl;
		exit(1);
	}

	// Check if results' file exists
	fs::path file(fileName);
	if (!fs::is_regular_file(file)) {
		cout << "ERROR: No results file found!" << endl;
		exit(1);
	}
}

Augmentation::~Augmentation() {}

// Methods
void Augmentation::start() {}
