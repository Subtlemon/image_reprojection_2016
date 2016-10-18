#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "ProjectionWindow.h"
#include "CameraParams.h"

int main1(int argc, char**argv);

int main(int argc, char** argv) {
    CameraParams test(200, 
            40, 30, 
            100, 100, 
            40, 10, 0);

    // define a square
    cv::Point2f square [4];
    square[0] = cv::Point2f(0, 0);
    square[1] = cv::Point2f(0, 100);
    square[2] = cv::Point2f(100, 100);
    square[3] = cv::Point2f(100, 0);

    // warp the square
    for(int c=0; c<4; ++c) {
        double x, y;
        test.calcDistance(square[c].x, square[c].y, x, y);
        square[c].x = x;
        square[c].y = y;
    }

    // shift the warped square to the topleft
    int minx = square[0].x, miny = square[0].y;
    int maxx = minx, maxy = miny;
    for (int c=1; c<4; ++c) {
        if (square[c].x < minx) minx = square[c].x;
        if (square[c].y < miny) miny = square[c].y;
        if (square[c].x > maxx) maxx = square[c].x;
        if (square[c].y > maxy) maxy = square[c].y;
    }
    for (int c=0; c<4; ++c) {
        square[c].x -= minx;
        square[c].y -= miny;
    }
    maxx -= minx;
    maxy -= miny;
    for (int c=0; c<4; ++c) {
        std::cout << "x: " << square[c].x << " y: " << square[c].y << std::endl;
    }
    std::cout << "maxx: " << maxx << " maxy " << maxy << std::endl;

    // make new drawing
    cv::Mat image(maxy, maxx, CV_8UC3);

    // draw circles at each vertex
    for (int c=0; c<4; ++c) {
        cv::circle(image, square[c], 5, cv::Scalar(0, 0, 255), -1, 8);
    }

    cv::imshow("Display Window", image);
    cv::waitKey(0);

    return 0;
}

int main1(int argc, char** argv) {
    cv::Mat image;
    image = cv::imread("/home/harry/Downloads/test1.jpg", CV_LOAD_IMAGE_COLOR);
    if (!image.data) {
        std::cout << "Couldn't load image" << std::endl;
        return 0;
    }

    std::cout << "Width: " << image.size().width << " Height: " << image.size().height << std::endl;

    ProjectionWindow win;
    cv::Mat correctedImage = win.correctImage(image);

    cv::imshow("Corrected Image (s to save)", correctedImage);
    int key = cv::waitKey(0);
    if ((char)key == 's') {
        cv::imwrite("/home/harry/Downloads/testOut.jpg", correctedImage);
    }
}
