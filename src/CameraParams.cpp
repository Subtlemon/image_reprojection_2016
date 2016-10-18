#include "CameraParams.h"
#include <math>

/**
 * @brief Simple Constructor.
 *
 * @param[in] height Altitude of plane (in any unit).
 * @param[in] FOV_x Field of view along x-axis of camera (degrees).
 * @param[in] FOV_y Field of view along y-axis of camera (degrees).
 * @param[in] roll Roll angle of plane (degrees) (rotation around y-axis of camera).
 * @param[in] pitch Pitch angle of plane (degrees) (rotation around x-axis of camera).
 * @param[in] yaw Yaw angle of plane (degrees) (rotation around z-axis of camera)
 */
CameraParams::CameraParams(double height, double FOV_x, double FOV_y, double roll, double pitch, double yaw) {
    this->height = height;
    this->FOV_x = FOV_x * PI / 180.;
    this->FOV_y = FOV_y * PI / 180.;
    this->roll = roll / 180.;
    this->pitch = pitch / 180.;
    this->yaw = yaw * PI / 180.;
}

/**
 * @brief Empty Destructor.
 */
CameraParams::~CameraParams(){
    // nothing to destruct
}


