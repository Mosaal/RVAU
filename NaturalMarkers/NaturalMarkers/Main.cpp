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

//#include <iostream>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//
//#include <stdio.h>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/calib3d.hpp>
//#include <opencv2/xfeatures2d.hpp>
//
//using namespace cv;
//using namespace std;
//using namespace cv::xfeatures2d;
//
//#define PREPARATION  0
//#define AUGMENTATION 1
//
//int T_MODE = -1;
//
//void printUsage(char *arg0) {
//	cout << "Usage: " << arg0 << " [mode]" << endl;
//	cout << "Where [mode] can be:" << endl;
//	cout << "\t-p | --preparation - preparation mode" << endl;
//	cout << "\t-a | --augmentation - augmentation mode" << endl;
//	system("pause");
//}
//
//bool validArgs(char **argv) {
//	return false;
//}
//
//void testSURF() {
//	Mat img_object = imread("img/img (5).jpg", CV_LOAD_IMAGE_GRAYSCALE);
//	Mat img_scene = imread("img.jpg", CV_LOAD_IMAGE_GRAYSCALE);
//
//	if (!img_object.data || !img_scene.data)
//	{
//		std::cout << " --(!) Error reading images " << std::endl; return;
//	}
//
//	//-- Step 1: Detect the keypoints using SURF Detector
//	int minHessian = 400;
//
//	Ptr<SURF> detector = SURF::create();
//	detector->setHessianThreshold(minHessian);
//
//	std::vector<KeyPoint> keypoints_object, keypoints_scene;
//
//	detector->detect(img_object, keypoints_object);
//	detector->detect(img_scene, keypoints_scene);
//
//	//-- Step 2: Calculate descriptors (feature vectors)
//	Ptr<SURF> extractor = SURF::create();
//
//	Mat descriptors_object, descriptors_scene;
//
//	extractor->compute(img_object, keypoints_object, descriptors_object);
//	extractor->compute(img_scene, keypoints_scene, descriptors_scene);
//
//	//-- Step 3: Matching descriptor vectors using FLANN matcher
//	FlannBasedMatcher matcher;
//	std::vector< DMatch > matches;
//	matcher.match(descriptors_object, descriptors_scene, matches);
//
//	double max_dist = 0; double min_dist = 100;
//
//	//-- Quick calculation of max and min distances between keypoints
//	cout << "M: " << matches.size() << endl;
//	cout << "D: " << descriptors_object.rows << endl;
//	for (int i = 0; i < descriptors_object.rows; i++)
//	{
//		double dist = matches[i].distance;
//		if (dist < min_dist) min_dist = dist;
//		if (dist > max_dist) max_dist = dist;
//	}
//
//	printf("-- Max dist : %f \n", max_dist);
//	printf("-- Min dist : %f \n", min_dist);
//
//	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
//	std::vector< DMatch > good_matches;
//
//	for (int i = 0; i < descriptors_object.rows; i++)
//	{
//		if (matches[i].distance < 3 * min_dist)
//		{
//			good_matches.push_back(matches[i]);
//		}
//	}
//
//	Mat img_matches;
//	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
//		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
//		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//
//	//-- Localize the object
//	std::vector<Point2f> obj;
//	std::vector<Point2f> scene;
//
//	for (int i = 0; i < good_matches.size(); i++)
//	{
//		//-- Get the keypoints from the good matches
//		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
//		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
//	}
//
//	Mat H = findHomography(obj, scene, CV_RANSAC);
//
//	//-- Get the corners from the image_1 ( the object to be "detected" )
//	std::vector<Point2f> obj_corners(4);
//	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_object.cols, 0);
//	obj_corners[2] = cvPoint(img_object.cols, img_object.rows); obj_corners[3] = cvPoint(0, img_object.rows);
//	std::vector<Point2f> scene_corners(4);
//
//	perspectiveTransform(obj_corners, scene_corners, H);
//
//	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
//	line(img_matches, scene_corners[0] + Point2f((float)img_object.cols, 0), scene_corners[1] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
//	line(img_matches, scene_corners[1] + Point2f((float)img_object.cols, 0), scene_corners[2] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
//	line(img_matches, scene_corners[2] + Point2f((float)img_object.cols, 0), scene_corners[3] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
//	line(img_matches, scene_corners[3] + Point2f((float)img_object.cols, 0), scene_corners[0] + Point2f((float)img_object.cols, 0), Scalar(0, 255, 0), 4);
//
//	//-- Show detected matches
//	imshow("Good Matches & Object detection", img_matches);
//
//	waitKey(0);
//}
//
//int main() {
//	testSURF();
//	return 0;
//}