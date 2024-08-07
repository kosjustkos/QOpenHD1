#include <cmath>

#include "fisheyecorrector.hpp"

FishEyeCorrector::FishEyeCorrector(const double focalLenPx, const DistortionCoefficients distortion)
    : m_focalLenPx(focalLenPx), m_distortion(distortion) {}

// inspired by https://github.com/opencv/opencv/blob/333054e05ee331a668692fc0b07abd313d894219/modules/calib3d/src/fisheye.cpp#L255
Point FishEyeCorrector::applyFishEyeDistortion(const Point undistortedPoint) const {
    // Normalize point using focal length
    double x = undistortedPoint.x / m_focalLenPx;
    double y = undistortedPoint.y / m_focalLenPx;
    double r2 = x * x + y * y;
    double r = std::sqrt(r2);

    // Apply distortion model
    double theta = std::atan(r);
    double theta2 = theta * theta;
    double theta3 = theta2 * theta;
    double theta4 = theta2 * theta2;
    double theta5 = theta4 * theta;
    double theta6 = theta3 * theta3;
    double theta7 = theta6 * theta;
    double theta8 = theta4 * theta4;
    double theta9 = theta8 * theta;
    double theta10 = theta9 * theta;
    double theta11 = theta10 * theta;

    double theta_d = theta + m_distortion.k1 * theta3 + m_distortion.k2 * theta5 + m_distortion.k3 * theta7 + m_distortion.k4 * theta9 + m_distortion.k5 * theta11;

    double inv_r = (r > 1e-8) ? 1.0 / r : 1;
    double cdist = (r > 1e-8) ? theta_d * inv_r : 1;

    // Distorted normalized coordinates
    double xd = x * cdist;
    double yd = y * cdist;  // tangential distortion is skipped since we don't have enough info

    // Convert back to image coordinates
    // No additional adjustment for the center of the image is performed here, because passed point is already shift from the center
    int x_distorted = static_cast<int>(m_focalLenPx * xd);
    int y_distorted = static_cast<int>(m_focalLenPx * yd);

    return Point{x_distorted, y_distorted};
}