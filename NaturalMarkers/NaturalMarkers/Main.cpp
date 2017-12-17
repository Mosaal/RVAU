#include <iostream>
#include <filesystem>

#include "Preparation.h"
#include "Augmentation.h"

using namespace std;
namespace fs = experimental::filesystem;

// Available execution modes
enum Mode { PREPARATION, AUGMENTATION };

// Global settings
int MODE = -1;
string PATH = "";

void getPath() {
	// Print out instructions for user
	if (MODE == PREPARATION)
		cout << "\nPlease input the path to the directory with the images to be prepared:" << endl;
	else if (MODE == AUGMENTATION)
		cout << "\nPlease input the path to the image to be augmented:" << endl;

	// Loop while input is not valid
	while (true) {
		// Get user's input
		cout << " > Path: ";
		cin >> PATH;

		// Check whether it is a file or a directory and stop if it is a valid input
		fs::path temp(PATH);
		if (fs::is_directory(temp) && MODE == PREPARATION)
			return;
		else if (fs::is_regular_file(temp) && MODE == AUGMENTATION)
			return;

		// Reset console reader if it isn't
		if (!cin) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

void getMode() {
	// Print out options for user input
	cout << "Please choose one of the following options:" << endl;
	cout << "\t0 - Execute in preparation mode" << endl;
	cout << "\t1 - Execute in augmentation mode" << endl;

	// Loop while input is not valid
	while (true) {
		// Get user's input
		cout << " > Mode: ";
		cin >> MODE;

		// Stop if it is a valid input
		if (MODE == PREPARATION || MODE == AUGMENTATION) {
			getPath();
			return;
		}

		// Reset console reader if it isn't
		if (!cin) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

int main(int argc, char **argv) {
	// Get user's chosen mode
	getMode();

	// Initialize corresponding mode
	switch (MODE) {
		case PREPARATION: {
			// Mode that allows to create a database of images
			Preparation mode(PATH);
			mode.start();
			break;
		}
		case AUGMENTATION: {
			// Mode that augments an image with a given database
			Augmentation mode(PATH);
			mode.start();
			break;
		}
		default: {
			// Terminate if mode is invalid
			cout << "ERROR: Invalid execution mode!" << endl;
			system("pause");
			return 1;
		}
	}

	return 0;
}