#include <vector>
#include <iostream>
#include <filesystem>
#include <opencv2\opencv.hpp>

#include "Arrow.h"
#include "Label.h"
#include "Rectangle.h"

using namespace cv;
using namespace std;
namespace fs = experimental::filesystem;

enum Mode { PREPARATION, AUGMENTATION };

#define SCALE     2
#define NO_PARAM  0
#define THICKNESS 2
#define RED       Scalar(0, 0, 255)
#define WHITE     Scalar(255, 255, 255)

#define BS        8
#define ENTER     13
#define ESC       27
#define NEXT      110
#define PREVIOUS  112

int MODE = -1;
string PATH = "";

Mat img;
Point org;
Rect rect;

string text = "";
vector<string> IDs;
vector<Arrow> arrows;
vector<Label> labels;
vector<Rectangle> rects;

bool DRAWING = false;
bool WRITING = false;
int upperCaseChars[26];
string MODES[] = { "PREPARATION", "AUGMENTATION" };

bool getPath() {
	// Print out instructions for user
	if (MODE == PREPARATION) {
		cout << "Please input the path to the directory with the images to be prepared:" << endl;
	} else if (MODE == AUGMENTATION) {
		cout << "Please input the path to the image to be augmented:" << endl;
	}
	
	// Loop while input is not valid
	while (true) {
		// Get user's input
		cout << " > Path: ";
		cin >> PATH;

		// Check whether it is a file or a directory and stop if it is a valid input
		fs::path temp(PATH);
		if (fs::is_directory(temp) && MODE == PREPARATION) {
			return true;
		} else if (fs::is_regular_file(temp) && MODE == AUGMENTATION) {
			return true;
		}

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
			if (getPath()) {
				return;
			}
		}

		// Reset console reader if it isn't
		if (!cin) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

void printIDs() {
	for (int i = 0; i < IDs.size(); i++) {
		cout << IDs[i] << endl;
	}
}

void onMouse(int event, int x, int y, int flags, void* param) {
	if (DRAWING) {
		rect.x = min(x, org.x);
		rect.y = min(y, org.y);
		rect.width = abs(x - org.x);
		rect.height = abs(y - org.y);
	}

	// TODO: Replace by a switch case (maybe)
	if (event == CV_EVENT_LBUTTONUP && DRAWING) {
		DRAWING = false;
		if (rect.width > 0 && rect.height > 0) {
			Rectangle temp(rect);
			rects.push_back(temp);
			IDs.push_back(temp.getID());
		}
	} else if (event == CV_EVENT_LBUTTONDOWN && !DRAWING && !WRITING) {
		DRAWING = true;
		org = Point(x, y);
		rect = Rect(x, y, 0, 0);
	} else if (event == CV_EVENT_LBUTTONDBLCLK && !DRAWING && !WRITING) {
		Arrow temp(Point(x, y), Point(x, y - 100));
		arrows.push_back(temp);
		IDs.push_back(temp.getID());
	} else if (event == CV_EVENT_RBUTTONDBLCLK && !DRAWING && !WRITING) {
		WRITING = true;
		Label temp(text, Point(x, y));
		labels.push_back(temp);
		IDs.push_back(temp.getID());
	}
}

int indexOf(char type, string ID) {
	if (type == 'A') {
		for (int i = 0; i < arrows.size(); i++) {
			if (arrows[i].getID() == ID) {
				return i;
			}
		}
	} else if (type == 'L') {
		for (int i = 0; i < labels.size(); i++) {
			if (labels[i].getID() == ID) {
				return i;
			}
		}
	} else if (type == 'R') {
		for (int i = 0; i < rects.size(); i++) {
			if (rects[i].getID() == ID) {
				return i;
			}
		}
	}

	return -1;
}

int main(int argc, char **argv) {
	/*int it = 0;
	for (int i = 'A'; i <= 'Z'; i++) {
		upperCaseChars[it++] = i;
	}*/

	/*Mat imgs[2];
	imgs[0] = imread("./Imgs/obj.jpg", CV_LOAD_IMAGE_COLOR);
	imgs[1] = imread("./Imgs/img.jpg", CV_LOAD_IMAGE_COLOR);

	int i = 0;
	while (true) {
		imshow("Test", imgs[i]);

		int input = waitKey(1);
		if (input == NEXT) {
			if (i < 1) {
				i++;
			}
		}
		else if (input == PREVIOUS) {
			if (i > 0) {
				i--;
			}
		}
	}*/

	/*namedWindow("Test", WINDOW_AUTOSIZE);
	setMouseCallback("Test", onMouse, NO_PARAM);

	Mat temp = imread("obj.jpg", CV_LOAD_IMAGE_COLOR);

	if (!temp.data) {
		cout << "ERROR: No image found!" << endl;
		system("pause");
		return 1;
	}

	putText(temp, MODES[MODE], Point(10, 40), FONT_HERSHEY_PLAIN, 3, RED, THICKNESS);
	temp.copyTo(img);

	while (true) {
		if (DRAWING && rect.width > 0 && rect.height > 0) {
			temp.copyTo(img);
			rectangle(img, rect, Scalar(0, 0, 255), 2);
			printf("X=%d, Y=%d, W=%d, H=%d\n", rect.x, rect.y, rect.width, rect.height);
		}

		for (int i = 0; i < rects.size(); i++) {
			rectangle(img, rects[i].getRect(), RED, THICKNESS);
		}

		for (int i = 0; i < arrows.size(); i++) {
			arrowedLine(img, arrows[i].getTail(), arrows[i].getTip(), RED, THICKNESS);
		}

		for (int i = 0; i < labels.size(); i++) {
			rectangle(img, labels[i].getBoundingBox(), WHITE, CV_FILLED);
			putText(img, labels[i].getText(), labels[i].getLLCorner(), FONT_HERSHEY_PLAIN, 2, RED, THICKNESS);
		}

		imshow("Test", img);

		int input = waitKey(10);
		switch (input) {
			case BS: {
				if (WRITING) {
					if (text.size() > 0) {
						text.erase(text.begin() + text.size() - 1);
						labels[labels.size() - 1].setText(text);
					}
				} else if (IDs.size() > 0) {
					printIDs();
					cout << "-----" << endl;

					string lastID = IDs[IDs.size() - 1];
					IDs.erase(IDs.begin() + IDs.size() - 1);
					char front = lastID.front();

					if (front == 'A') {
						arrows.erase(arrows.begin() + indexOf(front, lastID));
					} else if (front == 'L') {
						labels.erase(labels.begin() + indexOf(front, lastID));
					} else if (front == 'R') {
						rects.erase(rects.begin() + indexOf(front, lastID));
					}

					temp.copyTo(img);

					printIDs();
					cout << "-----" << endl;
				}
				break;
			}
			case NEXT: {
				cout << "NEXT" << endl;
				break;
			}
			case PREVIOUS: {
				cout << "PREVIOUS" << endl;
				break;
			}
			case ENTER: {
				if (WRITING && text.size() > 0) {
					WRITING = false;
					text = "";
				}
				break;
			}
			case ESC: {
				exit(0);
			}
			default: {
				if (WRITING) {
					for (int i = 0; i < 26; i++) {
						if (tolower(upperCaseChars[i]) == input) {
							text += upperCaseChars[i];
							labels[labels.size() - 1].setText(text);
							break;
						}
					}
				}
			}
		}
	}*/

	return 0;
}