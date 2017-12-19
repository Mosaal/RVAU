#include "SubSet.h"

// Constructor and destuctor
SubSet::SubSet(const string type, const Mat img): type(type), img(img), minDist(100), maxDist(0) {
	// Split the string on the first character
	string temp = type.substr(0, 1);

	// Check the type of subset
	if (temp == "L") {
		this->type = temp;
		label = type.substr(2, type.size() - 2);
	}
}

SubSet::~SubSet() {}

// Methods
void SubSet::addGoodMatch(const DMatch goodMatch) { goodMatches.push_back(goodMatch); }

void SubSet::addObjCorner(const Point2f objCorner) { objCorners.push_back(objCorner); }

// Getters
Mat SubSet::getImg() const { return img; }

string SubSet::getType() const { return type; }

string SubSet::getLabel() const { return label; }

double SubSet::getMinDist() const { return minDist; }

double SubSet::getMaxDist() const { return maxDist; }

Mat SubSet::getDescriptors() const { return descriptors; }

DMatch SubSet::getMatch(const int index) const { return matches[index]; }

vector<DMatch> SubSet::getMatches() const { return matches; }

DMatch SubSet::getGoodMatch(const int index) const { return goodMatches[index]; }

vector<DMatch> SubSet::getGoodMatches() const { return goodMatches; }

KeyPoint SubSet::getKeyPoint(const int index) const { return keyPoints[index]; }

vector<KeyPoint> SubSet::getKeyPoints() const { return keyPoints; }

Mat SubSet::getHomography() const { return homography; }

vector<Point2f> SubSet::getObj() const { return obj; }

vector<Point2f> SubSet::getScene() const { return scene; }

Point2f SubSet::getObjCorner(const int index) const { return objCorners[index]; }

vector<Point2f> SubSet::getObjCorners() const { return objCorners; }

Point2f SubSet::getSceneCorner(const int index) const { return sceneCorners[index]; }

vector<Point2f> SubSet::getSceneCorners() const { return sceneCorners; }

// Setters
void SubSet::setImg(const Mat img) { this->img = img; }

void SubSet::setType(const string type) { this->type = type; }

void SubSet::setLabel(const string label) { this->label = label; }

void SubSet::setMinDist(const double minDist) { this->minDist = minDist; }

void SubSet::setMaxDist(const double maxDist) { this->maxDist = maxDist; }

void SubSet::setDescriptors(const Mat descriptors) { this->descriptors = descriptors; }

void SubSet::setKeyPoints(const vector<KeyPoint> keyPoints) { this->keyPoints = keyPoints; }

void SubSet::setMatches(const vector<DMatch> matches) { this->matches = matches; }

void SubSet::setGoodMatches(const vector<DMatch> goodMatches) { this->goodMatches = goodMatches; }

void SubSet::setHomography(const Mat homography) { this->homography = homography; }

void SubSet::setObj(const vector<Point2f> obj) { this->obj = obj; }

void SubSet::setScene(const vector<Point2f> scene) { this->scene = scene; }

void SubSet::setSceneCorners(const vector<Point2f> sceneCorners) { this->sceneCorners = sceneCorners; }
