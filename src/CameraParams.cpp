#include "CameraParams.h"
#include <math>
#include <iostream>

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
    if (roll > 45) {
        roll = 45;
        std::cout << "Warning: Don't bother with roll > 45 degrees" << std::endl;
    }
    else if (roll < -45) {
        roll = -45;
        std::cout << "Warning: Don't bother with roll < -45 degrees" << std::endl;
    }
    if (pitch > 45) {
        pitch = 45;
        std::cout << "Warning: Don't bother with pitch > 45 degrees" << std::endl;
    }
    else if (pitch < -45) {
        pitch = -45;
        std::cout << "Warning: Don't bother with pitch < -45 degrees" << std::endl;
    }
    if (FOV_x > 60) {
        FOV_x = 60;
        std::cout << "Warning: Don't bother with FOV_x > 60 degrees" << std::endl;
    }
    else if (FOV_x < 1) {
        FOV_x = 1;
        std::cout << "Warning: FOV_x = " << FOV_x << " which makes no sense" << std::endl;
    }
    if (FOV_y > 60) {
        FOV_y = 60;
        std::cout << "Warning: Don't bother with FOV_y > 60 degrees" << std::endl;
    }
    else if (FOV_y < 1) {
        FOV_y = 1;
        std::cout << "Warning: FOV_y = " << FOV_y << " which makes no sense" << std::endl;
    }

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

/**
 * @brief Calculate the distance to a point on the ground.
 *
 * Calculates the distance between the camera and a location on the ground, 
 * based on its pixel location in the image.
 *
 * @param[in] x The X coordinate of the pixel (min = 0, max = maxX).
 * @param[in] y The Y coordinate of the pixel (min = 0, max = maxX).
 * @param[in] maxX The width of the image.
 * @param[in] maxY The height of the image.
 * @param[out] east The distance east of the camera location (same units as altitude).
 * @param[out] north The distance north of the camera location (same units as altitude).
 */
void CameraParams::calcDistance(int x, int y, int maxX, int maxY, double& east, double& north) {
    double deltaX = findXoverZ(x, maxX);
    double deltaY = findYoverZ(y, maxY);

    // The vector that extends to the ground from the camera can be represented as:
    // [ x(t) ]   [ deltaX ]     [   0    ]
    // [ y(t) ] = [ deltaY ] t + [   0    ]
    // [ z(t) ]   [   -1   ]     [ height ]
    // This vector intersects with the ground (plane z=0) at t = height
    // therefore:
    double distanceX = deltaX * height;
    double distanceY = deltaY * height;

    // rotate this by -yaw degrees
    double sinYaw = sin(0-yaw);
    double cosYaw = cos(0-yaw);

    east = distanceX * cosYaw - distanceY * sinYaw;
    north = distanceX * sinYaw + distanceY * cosYaw;
}
