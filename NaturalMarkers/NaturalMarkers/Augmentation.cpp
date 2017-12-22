#include "Augmentation.h"

// Constructor and destructor
Augmentation::Augmentation(const int debug, const string imgPath): debug(debug) {
	// Directory with the database, image to be augmented and results' file must have the same name
	sceneImg = imread(imgPath, CV_LOAD_IMAGE_COLOR);
	if (!sceneImg.data) {
		cout << "\nERROR: Invalid file type!" << endl;
		system("pause");
		exit(1);
	}

	// Remove file type from image file's name
	int amount = (int)(imgPath.size() - 4);
	fileName = imgPath.substr(0, amount);

	// Check if directory exists
	fs::path dir(fileName);
	if (!fs::is_directory(dir)) {
		cout << "\nERROR: No directory found with the database!" << endl;
		system("pause");
		exit(1);
	}

	// Check if results' file exists
	fs::path file(fileName + ".yml");
	if (!fs::is_regular_file(file)) {
		cout << "\nERROR: No results file found!" << endl;
		system("pause");
		exit(1);
	}

	// Print success message
	cout << "\n[AUGMENTATION] Mode started successfully!" << endl;
}

Augmentation::~Augmentation() {}

// Methods
void Augmentation::start() {
	// Load the corresponding YAML file
	FileStorage inFile(fileName + ".yml", FileStorage::READ);

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock();
		cout << "\n[AUGMENTATION] Loading data from the corresponding YAML file..." << endl;
	}

	// Read metadata
	vector<vector<string>> metadata;
	inFile["Metadata"] >> metadata;

	// Read subset images' data
	for (int i = 0; i < metadata.size(); i++) {
		for (int j = 0; j < metadata[i].size(); j++) {
			// Set ID
			string ID = "SubSet" + to_string(i) + to_string(j);

			// Load image data from file
			Mat subSetImg;
			inFile[ID] >> subSetImg;

			// Save loaded data
			SubSet subSet(metadata[i][j], subSetImg);
			subSets.push_back(subSet);
		}
	}

	// Close the input file
	inFile.release();

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock() - time;
		cout << "[AUGMENTATION] Data loaded successfully! - " << time << " ms" << endl;
	}

	// Detect the loaded images' and augmented image's features
	detectFeatures();
}

void Augmentation::detectFeatures() {
	// Using SIFT to detect the scene's features
	Ptr<SIFT> sift = SIFT::create();

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock();
		cout << "\n[AUGMENTATION] Using SIFT to detect the image's features..." << endl;
	}

	// Detect and save the scene's keypoints and descriptors
	sift->detectAndCompute(sceneImg, Mat(), sceneKeyPoints, sceneDescriptors);

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock() - time;
		cout << "[AUGMENTATION] Image's features detected successfully! - " << time << " ms" << endl;
		time = clock();
		cout << "\n[AUGMENTATION] Using SIFT to detect subset images' features..." << endl;
	}

	// Detect and save each of the subsets' keypoints and descriptors
	for (int i = 0; i < subSets.size(); i++) {
		// Detect keypoints and create descriptors
		Mat descriptors;
		vector<KeyPoint> keyPoints;

		// Detect and compute
		sift->detectAndCompute(subSets[i].getImg(), Mat(), keyPoints, descriptors);

		// Save computed data
		subSets[i].setKeyPoints(keyPoints);
		subSets[i].setDescriptors(descriptors);
	}

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock() - time;
		cout << "[AUGMENTATION] Subset images' features detected successfully! - " << time << " ms" << endl;
	}

	// Match descriptors using FLANN matcher
	match();
}

void Augmentation::match() {
	// Match descriptors using FLANN matcher
	FlannBasedMatcher matcher;

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock();
		cout << "\n[AUGMENTATION] Using FLANN based matching to match the image's subsets with itself..." << endl;
	}

	// Save all of the matches for each of the subsets
	for (int i = 0; i < subSets.size(); i++) {
		vector<DMatch> matches;
		matcher.match(subSets[i].getDescriptors(), sceneDescriptors, matches);
		subSets[i].setMatches(matches);
	}

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock() - time;
		cout << "[AUGMENTATION] All matches were found successfully! - " << time << " ms" << endl;
	}

	// Calculate the min and max distances between keypoints for each of the subsets
	for (int i = 0; i < subSets.size(); i++) {
		for (int j = 0; j < subSets[i].getDescriptors().rows; j++) {
			// Get the match's distance
			double dist = subSets[i].getMatch(j).distance;

			// Set min and max distances
			if (dist < subSets[i].getMinDist())
				subSets[i].setMinDist(dist);
			if (dist > subSets[i].getMaxDist())
				subSets[i].setMaxDist(dist);
		}
	}

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock();
		cout << "\n[AUGMENTATION] Eliminating the non-good matches..." << endl;
	}

	// We only want the good matches
	for (int i = 0; i < subSets.size(); i++) {
		for (int j = 0; j < subSets[i].getDescriptors().rows; j++) {
			// Distance determines the quality of the match
			if (subSets[i].getMatch(j).distance < subSets[i].getMinDist() * MULTIPLIER)
				subSets[i].addGoodMatch(subSets[i].getMatch(j));
		}
	}

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock() - time;
		cout << "[AUGMENTATION] Non-good matches eliminated successfully! - " << time << " ms" << endl;
	}

	// Localize the subsets
	localize();
}

void Augmentation::localize() {
	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock();
		cout << "\n[AUGMENTATION] Loading the keypoints from the good matches..." << endl;
	}

	// Get the keypoints from the good matches
	for (int i = 0; i < subSets.size(); i++) {
		// Get the keypoints
		vector<Point2f> obj;
		vector<Point2f> scene;

		// Check only the good matches
		for (int j = 0; j < subSets[i].getGoodMatches().size(); j++) {
			obj.push_back(subSets[i].getKeyPoint(subSets[i].getGoodMatch(j).queryIdx).pt);
			scene.push_back(sceneKeyPoints[subSets[i].getGoodMatch(j).trainIdx].pt);
		}

		// Save them to the corresponding subset
		subSets[i].setObj(obj);
		subSets[i].setScene(scene);
	}

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock() - time;
		cout << "[AUGMENTATION] Keypoints loaded successfully! - " << time << " ms" << endl;
		time = clock();
		cout << "\n[AUGMENTATION] Eliminating the subsets without enough keypoints..." << endl;
	}

	// Remove the subsets without enough points
	for (int i = 0; i < subSets.size(); i++) {
		// Check if either the object or the scene dont't have enough keypoints
		if (subSets[i].getObj().size() == 0 || subSets[i].getScene().size() == 0) {
			// Check if Debug mode is on
			if (debug == DEBUG_ON) {
				if (subSets[i].getType() == "A")
					cout << "[AUGMENTATION] An arrow has been eliminated!" << endl;
				else if (subSets[i].getType() == "L")
					cout << "[AUGMENTATION] A label has been eliminated!" << endl;
				else if (subSets[i].getType() == "R")
					cout << "[AUGMENTATION] A rectangle has been eliminated!" << endl;
			}

			// Eliminate subset
			subSets.erase(subSets.begin() + i--);
		}
	}

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock() - time;
		cout << "[AUGMENTATION] Subsets eliminated successfully! - " << time << " ms" << endl;
		time = clock();
		cout << "\n[AUGMENTATION] Calculating the Homography for each of the subsets..." << endl;
	}

	// Find homography for each subset
	for (int i = 0; i < subSets.size(); i++) {
		Mat homography = findHomography(subSets[i].getObj(), subSets[i].getScene(), CV_RANSAC);
		subSets[i].setHomography(homography);
	}

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock() - time;
		cout << "[AUGMENTATION] Homography found successfully! - " << time << " ms" << endl;
		time = clock();
		cout << "\n[AUGMENTATION] Eliminating the subsets without a valid Homography..." << endl;
	}

	// Remove the subsets without a valid homography
	for (int i = 0; i < subSets.size(); i++) {
		// Check whether the homography is good enough or not
		if (subSets[i].getHomography().cols == 0 || subSets[i].getHomography().rows == 0) {
			// Check if Debug mode is on
			if (debug == DEBUG_ON) {
				if (subSets[i].getType() == "A")
					cout << "[AUGMENTATION] An arrow has been eliminated!" << endl;
				else if (subSets[i].getType() == "L")
					cout << "[AUGMENTATION] A label has been eliminated!" << endl;
				else if (subSets[i].getType() == "R")
					cout << "[AUGMENTATION] A rectangle has been eliminated!" << endl;
			}

			// Eliminate subset
			subSets.erase(subSets.begin() + i--);
		}
	}

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock() - time;
		cout << "[AUGMENTATION] Subsets eliminated successfully! - " << time << " ms" << endl;
	}

	// Set the corners for each subset
	for (int i = 0; i < subSets.size(); i++) {
		subSets[i].addObjCorner(Point2f(0, 0));
		subSets[i].addObjCorner(Point2f((float)subSets[i].getImg().cols, 0));
		subSets[i].addObjCorner(Point2f((float)subSets[i].getImg().cols, (float)subSets[i].getImg().rows));
		subSets[i].addObjCorner(Point2f(0, (float)subSets[i].getImg().rows));
	}

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock();
		cout << "\n[AUGMENTATION] Setting the perspective for each subset according to its corresponding Homography..." << endl;
	}

	// Set the perspective for each subset
	for (int i = 0; i < subSets.size(); i++) {
		vector<Point2f> sceneCorners;
		perspectiveTransform(subSets[i].getObjCorners(), sceneCorners, subSets[i].getHomography());
		subSets[i].setSceneCorners(sceneCorners);
	}

	// Check if Debug mode is on
	if (debug == DEBUG_ON) {
		time = clock() - time;
		cout << "[AUGMENTATION] Perspectives set successfully! - " << time << " ms" << endl;
		cout << "\n[AUGMENTATION] Displaying results..." << endl;
	}

	// Display all of the located subsets
	display();
}

void Augmentation::display() {
	// Draw each located subset
	for (int i = 0; i < subSets.size(); i++) {
		// Draw the corresponding attribute
		if (subSets[i].getType() == "A") {
			// Get arrow's data
			int x = (int)subSets[i].getSceneCorner(0).x, y = (int)subSets[i].getSceneCorner(0).y;
			int width = (int)(subSets[i].getSceneCorner(1).x - x);
			int height = (int)(subSets[i].getSceneCorner(3).y - y);

			// Create and draw arrow
			Arrow arrow(Point(x + (width / 2), y + 25 + ARROW_SIZE), Point(x + (width / 2), y + 25));
			arrowedLine(sceneImg, arrow.getTail(), arrow.getTip(), RED, THICKNESS);
		} else if (subSets[i].getType() == "L") {
			// Get label's data
			int x = (int)subSets[i].getSceneCorner(0).x, y = (int)subSets[i].getSceneCorner(0).y;
			int width = (int)(subSets[i].getSceneCorner(1).x - x);
			int height = (int)(subSets[i].getSceneCorner(3).y - y);

			// Get the text's size
			int baseline = 2;
			Size textSize = getTextSize(subSets[i].getLabel(), FONT_HERSHEY_PLAIN, SCALE, THICKNESS, &baseline);

			// Create label
			Point center(x + (width / 2), y + (height / 2));
			Label label(subSets[i].getLabel(), center);

			// Draw label
			rectangle(sceneImg, label.getBoundingBox(), WHITE, CV_FILLED);
			putText(sceneImg, label.getText(), label.getLLCorner(), FONT_HERSHEY_PLAIN, SCALE, RED, THICKNESS);
		} else if (subSets[i].getType() == "R") {
			// Get rectangle's data
			int x = (int)subSets[i].getSceneCorner(0).x, y = (int)subSets[i].getSceneCorner(0).y;
			int width = (int)(subSets[i].getSceneCorner(1).x - x);
			int height = (int)(subSets[i].getSceneCorner(3).y - y);

			// Create and draw rectangle
			Rect rect(x, y, width, height);
			rectangle(sceneImg, rect, RED, THICKNESS);
		}
	}

	// Display the final image
	imshow("Mode: Augmentation", sceneImg);

	// Wait for keyboard press to end the program
	waitKey(NO_DELAY);
}
