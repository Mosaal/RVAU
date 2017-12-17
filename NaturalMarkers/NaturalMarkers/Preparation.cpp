#include "Preparation.h"

// Constructor and destructor
Preparation::Preparation(string dirPath): mode("") {
	// Iterate through given directory
	for (auto &file: fs::directory_iterator(dirPath)) {
		// Get the file's path
		stringstream ss; ss << file;
		string filePath = ss.str();

		// Check if the file is an image
		Mat temp = imread(filePath, CV_LOAD_IMAGE_COLOR);
		if (!temp.data) {
			cout << "\nERROR: Directory contains invalid file types!" << endl;
			system("pause");
			exit(1);
		}

		// Add it to the list
		Image img(temp);
		imgs.push_back(img);
	}

	// Check if directory contained at least one image
	if (imgs.size() == 0) {
		cout << "\nERROR: No images found in directory!" << endl;
		system("pause");
		exit(1);
	}

	// Success message
	cout << "\n[PREPARATION] Mode started successfully!" << endl;
}

Preparation::~Preparation() {}

// Methods
void onMouse(int event, int x, int y, int flags, void* param) {
	// Cast the parameter with the user data
	Preparation* prep = (Preparation*)param;

	// Check if user is currently drawing
	if (prep->DRAWING && !prep->WRITING) {
		// Update the rectangle's settings
		prep->rect.x = min(x, prep->org.x);
		prep->rect.y = min(y, prep->org.y);
		prep->rect.width = abs(x - prep->org.x);
		prep->rect.height = abs(y - prep->org.y);
	}

	// Check the event and act accordingly
	switch (event) {
		case CV_EVENT_LBUTTONUP: {
			// Lift the left mouse button to stop drawing the rectangle
			if (prep->DRAWING && !prep->WRITING) {
				// Set drawing mode off
				prep->DRAWING = false;

				// Add the rectangle to the corresponding image
				if (prep->rect.width > 0 && prep->rect.height > 0) {
					// Only if the rectangle's dimensions are greater than zero
					Rectangle rectangle(prep->rect);
					prep->imgs[prep->currImg].addRect(rectangle);
					prep->imgs[prep->currImg].addID(rectangle.getID());
				}

				// Refresh image
				prep->imgs[prep->currImg].updateImage();
			}
			break;
		}
		case CV_EVENT_LBUTTONDOWN: {
			// Click and drag the left mouse button to start drawing the rectangle
			if (!prep->DRAWING && !prep->WRITING) {
				// Set drawing mode on
				prep->DRAWING = true;

				// Set the currently drawing rectangle's settings
				prep->org = Point(x, y);
				prep->rect = Rect(x, y, 0, 0);
			}
			break;
		}
		case CV_EVENT_LBUTTONDBLCLK: {
			// Double left click for a new arrow
			if (!prep->DRAWING && !prep->WRITING) {
				// Add the arrow to the corresponding image
				Arrow arrow(Point(x, y), Point(x, y - ARROW_SIZE));
				prep->imgs[prep->currImg].addArrow(arrow);
				prep->imgs[prep->currImg].addID(arrow.getID());
			}
			break;
		}
		case CV_EVENT_RBUTTONDBLCLK: {
			// Double right click for a new label
			if (!prep->DRAWING && !prep->WRITING) {
				// Set writing mode on
				prep->WRITING = true;

				// Add the label to the corresponfing image
				Label label(prep->labelText, Point(x, y));
				prep->imgs[prep->currImg].addLabel(label);
				prep->imgs[prep->currImg].addID(label.getID());
			}
			break;
		}
	}
}

void Preparation::start() {
	// Set the characters allowed for the labels' text
	for (int d = '0'; d <= '9'; d++)
		possibleChars.push_back(d);
	for (int c = 'A'; c <= 'Z'; c++)
		possibleChars.push_back(c);

	// Initialize window
	window = "Mode: Preparation | Number of Images: " + to_string(imgs.size());
	namedWindow(window, WINDOW_AUTOSIZE);

	// Override the mouse's callback
	setMouseCallback(window, onMouse, this);

	// Display first image on the list
	imshow(window, imgs[currImg].img);

	// Refresh the images on the screen
	display();
}

void Preparation::updateMode() {
	// Get mode's text dimensions
	int baseline = 2;
	Size size = getTextSize(mode, FONT_HERSHEY_PLAIN, SCALE, THICKNESS, &baseline);

	// Get mode's text position
	llx = imgs[currImg].img.size().width - size.width - (OFFSET / 2);
}

void Preparation::update() {
	bool refresh = false;

	// Refresh the screen if the user is currently drawing
	if (DRAWING && !WRITING && rect.width > 0 && rect.height > 0) {
		// Set mode
		refresh = true;
		mode = "DRAWING";

		// Refresh image
		imgs[currImg].updateImage();

		// Update mode
		updateMode();

		// Draw current rectangle
		rectangle(imgs[currImg].img, rect, RED, THICKNESS);
	} else if (!DRAWING && WRITING) {
		// Set mode
		mode = "WRITING";

		// Refresh image
		imgs[currImg].updateImage();

		// Update mode
		updateMode();
	} else {
		// Set mode
		mode = "";
	}
}

void Preparation::display() {
	// Loop until user is done preparing the images
	while (true) {
		// Update data before dislaying it
		update();

		// Display the current image's rectangles
		for (int i = 0; i < imgs[currImg].getRects().size(); i++)
			rectangle(imgs[currImg].img, imgs[currImg].getRect(i).getRect(), RED, THICKNESS);

		// Display the current image's arrows
		for (int i = 0; i < imgs[currImg].getArrows().size(); i++)
			arrowedLine(imgs[currImg].img, imgs[currImg].getArrow(i).getTail(), imgs[currImg].getArrow(i).getTip(), RED, THICKNESS);

		// Display the current image's labels
		for (int i = 0; i < imgs[currImg].getLabels().size(); i++) {
			rectangle(imgs[currImg].img, imgs[currImg].getLabel(i).getBoundingBox(), WHITE, CV_FILLED);
			putText(imgs[currImg].img, imgs[currImg].getLabel(i).getText(), imgs[currImg].getLabel(i).getLLCorner(), FONT_HERSHEY_PLAIN, SCALE, RED, THICKNESS);
		}

		// Display the current mode (drawing or writing)
		putText(imgs[currImg].img, mode, Point(llx, 35), FONT_HERSHEY_PLAIN, SCALE, RED, THICKNESS);

		// Display the currently selected image
		imshow(window, imgs[currImg].img);

		// Handle the keyboard's input
		handleInput(waitKey(DELAY));
	}
}

int Preparation::indexOfChar(const int input) const {
	// Check if the input is valid
	for (int i = 0; i < possibleChars.size(); i++)
		if (tolower(possibleChars[i]) == input)
			return i;
	return -1;
}

int Preparation::indexOfArrow(const string ID) const {
	// Check if the given ID is valid
	for (int i = 0; i < imgs[currImg].getArrows().size(); i++)
		if (imgs[currImg].getArrow(i).getID() == ID)
			return i;
	return -1;
}

int Preparation::indexOfLabel(const string ID) const {
	// Check if the given ID is valid
	for (int i = 0; i < imgs[currImg].getLabels().size(); i++)
		if (imgs[currImg].getLabel(i).getID() == ID)
			return i;
	return -1;
}

int Preparation::indexOfRectangle(const string ID) const {
	// Check if the given ID is valid
	for (int i = 0; i < imgs[currImg].getRects().size(); i++)
		if (imgs[currImg].getRect(i).getID() == ID)
			return i;
	return -1;
}

void Preparation::removeAttribute(const string ID) {
	// Parse the ID ("{ID}{COUNTER}")
	char front = ID.front();

	// Check whether the ID belongs to an arrow, a label or a rectangle
	if (front == 'A')
		imgs[currImg].removeArrow(indexOfArrow(ID));
	else if (front == 'L')
		imgs[currImg].removeLabel(indexOfLabel(ID));
	else if (front == 'R')
		imgs[currImg].removeRect(indexOfRectangle(ID));
}

void Preparation::handleInput(int input) {
	// Check user's input and act accordingly
	switch (input) {
		case ESC: {
			// Quit the program on Escape
			exit(0);
			break;
		}
		case BACKSPACE: {
			// Check current mode and act accordingly
			if (!DRAWING && WRITING && labelText.size() > 0) {
				// Delete the latest character from the label and update it
				labelText.erase(labelText.begin() + labelText.size() - 1);
				imgs[currImg].updateLabel((int)(imgs[currImg].getLabels().size() - 1), labelText);

				// Refresh image
				imgs[currImg].updateImage();
			} else if (!DRAWING && !WRITING && imgs[currImg].getIDs().size() > 0) {
				// Delete the last added ID
				string lastID = imgs[currImg].getID((int)(imgs[currImg].getIDs().size() - 1));
				imgs[currImg].removeID((int)(imgs[currImg].getIDs().size() - 1));

				// Delete the last added attribute
				removeAttribute(lastID);

				// Refresh image
				imgs[currImg].updateImage();
			}
			break;
		}
		case ENTER: {
			// Stop writing mode and thus finish writing the current label
			if (!DRAWING && WRITING && labelText.size() > 0) {
				// Reset label's text
				labelText = "";

				// Set writing mode off
				WRITING = false;

				// Refresh image
				imgs[currImg].updateImage();
			}
			break;
		}
		case NEXT: {
			// Advance torwards the next image on the list
			if (!DRAWING && !WRITING && currImg < imgs.size() - 1)
				currImg++;
			break;
		}
		case PREVIOUS: {
			// Go back to the previous image
			if (!DRAWING && !WRITING && currImg > 0)
				currImg--;
			break;
		}
		default: {
			// Check if it is in writing mode
			if (!DRAWING && WRITING) {
				// Check if the input is valid
				int index = indexOfChar(input);

				// Add it to the current label if it is
				if (index != -1) {
					labelText += possibleChars[index];
					imgs[currImg].updateLabel((int)(imgs[currImg].getLabels().size() - 1), labelText);
				}
			}
		}
	}
}
