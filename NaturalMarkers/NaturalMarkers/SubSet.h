#pragma once

#include <vector>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\xfeatures2d.hpp>

using namespace cv;
using namespace std;

class SubSet {
private:
	// Private properties
	Mat img;

	string type;
	string label;

	double minDist;
	double maxDist;

	Mat descriptors;
	vector<KeyPoint> keyPoints;

	vector<DMatch> matches;
	vector<DMatch> goodMatches;

	Mat homography;
	vector<Point2f> obj;
	vector<Point2f> scene;
	vector<Point2f> objCorners;
	vector<Point2f> sceneCorners;
public:
	// Constructor and destructor
	SubSet(const string type, const Mat img);
	~SubSet();

	// Methods
	void addGoodMatch(const DMatch goodMatch);
	void addObjCorner(const Point2f objCorner);

	// Getters
	Mat getImg() const;
	string getType() const;
	string getLabel() const;

	double getMinDist() const;
	double getMaxDist() const;

	Mat getDescriptors() const;

	DMatch getMatch(const int index) const;
	vector<DMatch> getMatches() const;
	DMatch getGoodMatch(const int index) const;
	vector<DMatch> getGoodMatches() const;

	KeyPoint getKeyPoint(const int index) const;
	vector<KeyPoint> getKeyPoints() const;

	Mat getHomography() const;
	vector<Point2f> getObj() const;
	vector<Point2f> getScene() const;

	Point2f getObjCorner(const int index) const;
	vector<Point2f> getObjCorners() const;

	Point2f getSceneCorner(const int index) const;
	vector<Point2f> getSceneCorners() const;

	// Setters
	void setImg(const Mat img);
	void setType(const string type);
	void setLabel(const string label);

	void setMinDist(const double minDist);
	void setMaxDist(const double maxDist);

	void setDescriptors(const Mat descriptors);
	void setKeyPoints(const vector<KeyPoint> keyPoints);

	void setMatches(const vector<DMatch> matches);
	void setGoodMatches(const vector<DMatch> goodMatches);

	void setHomography(const Mat homography);
	void setObj(const vector<Point2f> obj);
	void setScene(const vector<Point2f> scene);
	void setSceneCorners(const vector<Point2f> sceneCorners);
};

