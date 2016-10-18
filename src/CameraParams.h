#pragma once

class CameraParams {
  public:
    CameraParams(double height, double FOV_x, double FOV_y, double roll, double pitch, double yaw);
    ~CameraParams();

    void correct(int x, int y, int maxX, int maxY, int& outX, int& outY);

  private:
    double findXoverZ(int x, int maxX);
    double findYoverZ(int y, int maxY);

    double height, FOV_x, FOV_y, roll, pitch, yaw;
};
