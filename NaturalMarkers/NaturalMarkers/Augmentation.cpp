#include "Augmentation.h"

// Constructor and destructor
Augmentation::Augmentation(const string imgPath) {
	// Directory with the database, image to be augmented and results' file must have the same name
	sceneImg = imread(imgPath, CV_LOAD_IMAGE_COLOR);
	if (!sceneImg.data) {
		cout << "\nERROR: Invalid file type!" << endl;
		exit(1);
	}

	// Remove file type from image file's name
	int amount = (int)(imgPath.size() - 4);
	fileName = imgPath.substr(0, amount);

	// Check if directory exists
	fs::path dir(fileName);
	if (!fs::is_directory(dir)) {
		cout << "\nERROR: No directory found with the database!" << endl;
		exit(1);
	}

	// Check if results' file exists
	fs::path file(fileName + ".yml");
	if (!fs::is_regular_file(file)) {
		cout << "\nERROR: No results file found!" << endl;
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

	// Close the file
	inFile.release();

	// Detect the loaded images' and augmented image's features
	detectFeatures();
}

void Augmentation::detectFeatures() {
	// Using SIFT to detect the scene's features
	Ptr<SIFT> sift = SIFT::create();

	// Detect and save the scene's keypoints and descriptors
	sift->detectAndCompute(sceneImg, Mat(), sceneKeyPoints, sceneDescriptors);

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

	// Match descriptors using FLANN matcher
	match();
}

void Augmentation::match() {
	// Match descriptors using FLANN matcher
	FlannBasedMatcher matcher;

	// Save all of the matches for each of the subsets
	for (int i = 0; i < subSets.size(); i++) {
		vector<DMatch> matches;
		matcher.match(subSets[i].getDescriptors(), sceneDescriptors, matches);
		subSets[i].setMatches(matches);
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

	// We only want the good matches
	for (int i = 0; i < subSets.size(); i++) {
		for (int j = 0; j < subSets[i].getDescriptors().rows; j++) {
			// Distance determines the quality of the match
			if (subSets[i].getMatch(j).distance < subSets[i].getMinDist() * MULTIPLIER)
				subSets[i].addGoodMatch(subSets[i].getMatch(j));
		}
	}

	// Localize the subsets
	localize();
}

void Augmentation::localize() {
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

	// Find homography for each subset
	for (int i = 0; i < subSets.size(); i++) {
		Mat homography = findHomography(subSets[i].getObj(), subSets[i].getScene(), CV_RANSAC);
		subSets[i].setHomography(homography);
	}

	// Set the corners for each subset
	for (int i = 0; i < subSets.size(); i++) {
		subSets[i].addObjCorner(Point2f(0, 0));
		subSets[i].addObjCorner(Point2f((float)subSets[i].getImg().cols, 0));
		subSets[i].addObjCorner(Point2f((float)subSets[i].getImg().cols, (float)subSets[i].getImg().rows));
		subSets[i].addObjCorner(Point2f(0, (float)subSets[i].getImg().rows));
	}

	// Set the perspective for each subset
	for (int i = 0; i < subSets.size(); i++) {
		vector<Point2f> sceneCorners;
		perspectiveTransform(subSets[i].getObjCorners(), sceneCorners, subSets[i].getHomography());
		subSets[i].setSceneCorners(sceneCorners);
	}

	// Display all of the located subsets
	display();
}

void Augmentation::display() {
	// Draw each located subset
	for (int i = 0; i < subSets.size(); i++) {
		// Draw the corresponding attribute
		if (subSets[i].getType() == "A") {
			// TODO: Draw arrowed lines

			//// Get rectangle data
			//int x = (int)subSets[i].getSceneCorner(0).x, y = (int)subSets[i].getSceneCorner(0).y;
			//int width = (int)(subSets[i].getSceneCorner(1).x - x);
			//int height = (int)(subSets[i].getSceneCorner(3).y - y);

			//// Create and draw rectangle
			//Rect rect(x, y, width, height);
			//rectangle(sceneImg, rect, Scalar(255, 0, 0), THICKNESS);
		} else if (subSets[i].getType() == "L") {
			// TODO: Write and draw labels

			//// Get rectangle data
			//int x = (int)subSets[i].getSceneCorner(0).x, y = (int)subSets[i].getSceneCorner(0).y;
			//int width = (int)(subSets[i].getSceneCorner(1).x - x);
			//int height = (int)(subSets[i].getSceneCorner(3).y - y);

			//// Create and draw rectangle
			//Rect rect(x, y, width, height);
			//rectangle(sceneImg, rect, WHITE, CV_FILLED);
		} else if (subSets[i].getType() == "R") {
			// Get rectangle data
			int x = (int)subSets[i].getSceneCorner(0).x, y = (int)subSets[i].getSceneCorner(0).y;
			int width = (int)(subSets[i].getSceneCorner(1).x - x);
			int height = (int)(subSets[i].getSceneCorner(3).y - y);

			// Create and draw rectangle
			Rect rect(x, y, width, height);
			rectangle(sceneImg, rect, RED, THICKNESS);
		}

		// Draw the lines between the corners
		/*line(sceneImg, subSets[i].getSceneCorner(0), subSets[i].getSceneCorner(1), RED, THICKNESS);
		line(sceneImg, subSets[i].getSceneCorner(1), subSets[i].getSceneCorner(2), RED, THICKNESS);
		line(sceneImg, subSets[i].getSceneCorner(2), subSets[i].getSceneCorner(3), RED, THICKNESS);
		line(sceneImg, subSets[i].getSceneCorner(3), subSets[i].getSceneCorner(0), RED, THICKNESS);*/
	}

	// Display the final image
	imshow("Final Image", sceneImg);

	// Wait for keyboard press to end the program
	waitKey(0);
}
