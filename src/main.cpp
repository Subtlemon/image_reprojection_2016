#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "ProjectionWindow.h"

int main(int argc, char** argv) {
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
