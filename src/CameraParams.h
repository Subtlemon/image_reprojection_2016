#pragma once

class CameraParams {
  public:
    CameraParams(double altitude, double FOV_x, double FOV_y, int width, int height, double roll, double pitch, double yaw);
    ~CameraParams();

    void calcDistance(int x, int y, double& east, double& north);

  private:
    void rotateX(double u[3], double theta);
    void rotateY(double u[3], double theta);
    void rotateZ(double u[3], double theta);
    double findAngle(int pixelPos, int maxPixelPos, double FOV);
    // no longer in commission
    double findXoverZ(int x, int width, double FOV, double tilt);

    double altitude, FOV_x, FOV_y, roll, pitch, yaw;
    int width, height;

    static void printVector(const double u[3]);
};
