#pragma once
#include <opencv2/core.hpp>
#include <string>

class ProjectionWindow{
  public:
    ProjectionWindow();
    ~ProjectionWindow();

    cv::Mat correctImage(const cv::Mat& image);

    friend void cb(int event, int x, int y, int flags, void* userdata);
  private:
    cv::Point2f points[4];
    int currPoint;
    int nextPoint;

    void leftClick(int x, int y);
    void rightClick();

    static int max2abs(int a, int b);
    static int max3abs(int a, int b, int c);
};
