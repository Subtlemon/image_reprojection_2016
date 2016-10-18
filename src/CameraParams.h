#pragma once

class CameraParams {
  public:
    CameraParams(double altitude, double FOV_x, double FOV_y, int width, int height, double roll, double pitch, double yaw);
    ~CameraParams();

    void calcDistance(int x, int y, double& east, double& north);

  private:
    double findXoverZ(int x, int width, double FOV, double tilt);

    double altitude, FOV_x, FOV_y, roll, pitch, yaw;
    int width, height;
};
