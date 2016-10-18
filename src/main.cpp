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
            30, 20, 
            100, 100, 
            20, 10, 0);

    // define a square
    int squareSize = 32;
    cv::Point2f square [squareSize];
    square[0] = cv::Point2f(0, 0);
    for (int c=1; c<squareSize/4; ++c) {
        square[c] = cv::Point2f(0, c * 100 / squareSize * 4);
        square[c + squareSize/4] = cv::Point2f(c * 100 / squareSize * 4, 100);
        square[c + 2*squareSize/4] = cv::Point2f(100, 100 - c * 100 / squareSize * 4);
        square[c + 3*squareSize/4] = cv::Point2f(100 - c * 100 / squareSize * 4, 0);
    }
    square[squareSize/4] = cv::Point2f(0, 100);
    square[2*squareSize/4] = cv::Point2f(100, 100);
    square[3*squareSize/4] = cv::Point2f(100, 0);

    // display the square
    cv::Mat firstImage(100, 100, CV_8UC3);
    for (int c=0; c<squareSize; ++c) {
        cv::circle(firstImage, square[c], 5, cv::Scalar(0, 255-c*255/squareSize, c*255/squareSize), -1, 8);
    }

    cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Window", firstImage);
    cv::waitKey(0);
    cv::destroyWindow("Display Window");

    // warp the square
    for(int c=0; c<squareSize; ++c) {
        double x, y;
        test.calcDistance(square[c].x, square[c].y, x, y);
        square[c].x = x;
        square[c].y = y;
    }

    // shift the warped square to the topleft
    int minx = square[0].x, miny = square[0].y;
    int maxx = minx, maxy = miny;
    for (int c=1; c<squareSize; ++c) {
        if (square[c].x < minx) minx = square[c].x;
        if (square[c].y < miny) miny = square[c].y;
        if (square[c].x > maxx) maxx = square[c].x;
        if (square[c].y > maxy) maxy = square[c].y;
    }
    for (int c=0; c<squareSize; ++c) {
        square[c].x -= minx;
        square[c].y -= miny;
    }
    maxx -= minx;
    maxy -= miny;
    // scale to 200x200
    for (int c=0; c<squareSize; ++c) {
        square[c].x *= 200. / maxx;
        square[c].y *= 200. / maxy;
    }
    maxx *= 200. / maxx;
    maxy *= 200. / maxy;
    for (int c=0; c<squareSize; ++c) {
        std::cout << "x: " << square[c].x << " y: " << square[c].y << std::endl;
    }
    std::cout << "maxx: " << maxx << " maxy " << maxy << std::endl;

    // make new drawing
    cv::Mat image(maxy, maxx, CV_8UC3);

    // draw circles at each vertex
    for (int c=0; c<squareSize; ++c) {
        cv::circle(image, square[c], 5, cv::Scalar(0, 255-c*255/squareSize, c*255/squareSize), -1, 8);
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
