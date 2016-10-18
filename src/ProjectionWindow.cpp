#include "ProjectionWindow.h"
#include <iostream>
#include <opencv2/opencv.hpp>

void cb(int event, int x, int y, int flags, void* classPtr) {
    ProjectionWindow* ptr = (ProjectionWindow*) classPtr;
    if (event == cv::EVENT_LBUTTONDOWN) {
        ptr->leftClick(x, y);
    }
    else if (event == cv::EVENT_RBUTTONUP) {
        ptr->rightClick();
    }
}

ProjectionWindow::ProjectionWindow() {
}

ProjectionWindow::~ProjectionWindow() {
    // nothing to do here
}

cv::Mat ProjectionWindow::correctImage(const cv::Mat& image) {

    std::string windowNames[4] = {"Click topleft", "Click topright", "Click bottomright", "Click bottomleft"};
    currPoint = 0;
    nextPoint = 0;

    // get points
    do {
        cv::namedWindow(windowNames[currPoint], cv::WINDOW_NORMAL);
        cv::setMouseCallback(windowNames[currPoint], cb, this);
        // cv::moveWindow or cv::resizeWindow here as necessary

        // draw previous points on displayImage
        cv::Mat dispImage = image.clone();
        for(int c=0; c<currPoint; ++c) {
            cv::circle(dispImage, points[c], 5, cv::Scalar(0, 0, 255), -1,  8);
        }

        cv::imshow(windowNames[currPoint], dispImage);
        do {
            cv::waitKey(1);
        } while(currPoint == nextPoint);

        cv::destroyWindow(windowNames[currPoint]);
        currPoint = nextPoint;
    } while(currPoint < 4);

    // create destination matrix
    cv::Size newSize;
    newSize.width = max2abs(points[1].x - points[0].x, points[2].x - points[3].x);
    newSize.height = max2abs(points[3].y - points[0].y, points[2].y - points[1].y);

    newSize.width = max2abs(newSize.width, newSize.height);
    newSize.height = newSize.width;

    newSize.width *= 2;
    newSize.height *= 2;

    cv::Point2f destPoints[4];
    destPoints[0] = cv::Point2f(newSize.width*0.35, newSize.height*0.35);
    destPoints[1] = cv::Point2f(newSize.width*0.65, newSize.height*0.35);
    destPoints[2] = cv::Point2f(newSize.width*0.65, newSize.height*0.65);
    destPoints[3] = cv::Point2f(newSize.width*0.35, newSize.height*0.65);

    cv::Mat dst(newSize, image.type());

    // do transform
    cv::Mat transform = cv::getPerspectiveTransform(points, destPoints);
    cv::warpPerspective(image, dst, transform, newSize);

    return dst;
}

void ProjectionWindow::leftClick(int x, int y) {
    points[currPoint] = cv::Point2f(x, y);
    nextPoint = currPoint + 1;
}

void ProjectionWindow::rightClick() {
    if (currPoint > 0)
        nextPoint = currPoint - 1;
}

int ProjectionWindow::max2abs(int a, int b) {
    if (a < 0) a = 0-a;
    if (b < 0) b = 0-b;
    return a > b ? a : b;
}

int ProjectionWindow::max3abs(int a, int b, int c) {
    if (a < 0) a = 0-a;
    if (b < 0) b = 0-b;
    if (c < 0) c = 0-c;
    int temp = a > b ? a : b;
    return temp > c ? temp : c;
}
