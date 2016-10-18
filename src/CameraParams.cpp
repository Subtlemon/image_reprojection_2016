#include "CameraParams.h"
#include <math.h>
#include <iostream>

#ifndef PI
#define PI 3.14159265358979323846
#endif

/**
 * @brief Simple Constructor.
 *
 * @param[in] altitude Altitude of plane (in any unit).
 * @param[in] FOV_x Field of view along x-axis of camera (degrees).
 * @param[in] FOV_y Field of view along y-axis of camera (degrees).
 * @param[in] width Width of camera (pixels).
 * @param[in] height Height of camera (pixels).
 * @param[in] roll Roll angle of plane (degrees) (rotation around y-axis of camera).
 * @param[in] pitch Pitch angle of plane (degrees) (rotation around x-axis of camera).
 * @param[in] yaw Yaw angle of plane (degrees) (rotation around z-axis of camera)
 */
CameraParams::CameraParams(double altitude, double FOV_x, double FOV_y, int width, int height, double roll, double pitch, double yaw) {
    if (roll > 45) {
        std::cout << "Warning: Don't bother with roll > 45 degrees" << std::endl;
        roll = 45;
    }
    else if (roll < -45) {
        std::cout << "Warning: Don't bother with roll < -45 degrees" << std::endl;
        roll = -45;
    }
    if (pitch > 45) {
        std::cout << "Warning: Don't bother with pitch > 45 degrees" << std::endl;
        pitch = 45;
    }
    else if (pitch < -45) {
        pitch = -45;
        std::cout << "Warning: Don't bother with pitch < -45 degrees" << std::endl;
    }
    if (FOV_x > 60) {
        std::cout << "Warning: Don't bother with FOV_x > 60 degrees" << std::endl;
        FOV_x = 60;
    }
    else if (FOV_x < 1) {
        std::cout << "Warning: FOV_x = " << FOV_x << " which makes no sense" << std::endl;
        FOV_x = 1;
    }
    if (FOV_y > 60) {
        std::cout << "Warning: Don't bother with FOV_y > 60 degrees" << std::endl;
        FOV_y = 60;
    }
    else if (FOV_y < 1) {
        std::cout << "Warning: FOV_y = " << FOV_y << " which makes no sense" << std::endl;
        FOV_y = 1;
    }
    if (width < 1) {
        std::cout << "Warning: width " << width << " makes no sense" << std::endl;
        width = 1920;
    }
    if (height < 1) {
        std::cout << "Warning: height " << height << " makes no sense" << std::endl;
        height = 1080;
    }
    if (altitude <= 0) {
        std::cout << "Warning: altitude " << altitude << " makes no sense" << std::endl;
        altitude = 1;
    }

    this->altitude = altitude;
    this->FOV_x = FOV_x * PI / 180.;
    this->FOV_y = FOV_y * PI / 180.;
    this->width = width;
    this->height = height;
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
 * @param[out] east The distance east of the camera location (same units as altitude).
 * @param[out] north The distance north of the camera location (same units as altitude).
 */
void CameraParams::calcDistance(int x, int y, double& east, double& north) {
    double deltaX = findXoverZ(x, width, FOV_x, roll);
    double deltaY = findXoverZ(y, height, FOV_y, pitch);

    // The vector that extends to the ground from the camera can be represented as:
    //
    // [ x(t) ]   [ deltaX ]     [   0    ]
    // [ y(t) ] = [ deltaY ] t + [   0    ]
    // [ z(t) ]   [   -1   ]     [ height ]
    //
    // (Assuming that the camera lens is tiny compared to height)
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

/**
 * @brief Calculate the change in X per unit Z.
 *
 * This function can be reused for Y, if you use the right FOV and the pitch 
 * instead of roll for tilt.
 *
 * @param[in] x Pixel location of X.
 * @param[in] maxX Image width.
 * @param[in] FOV Field of view along x.
 * @param[in] tilt Tilt along x-axis.
 */
double CameraParams::findXoverZ(int x, int width, double FOV, double tilt) {
    
    // Given the FOV, the furthest ray and middle ray (and by approximation, 
    // all the rays) converge at distance d behind the lens, where d is
    // measured in pixels.
    double d = width / 2 / tan(FOV);

    // If we put an imaginary stick of length d behind the lens at the
    // midpoint, the line passing through pixel x makes an angle alpha with
    // the imaginary line d.
    double alpha = atan((x-width) / d);

    // Since the plane is tilted at angle tilt, the ray passing through the tip
    // of d and the pixel x makes angle theta with the normal to the ground (Z).
    double theta = alpha + tilt;

    // tan(theta) = deltaX / 1;
    return tan(theta);
}
