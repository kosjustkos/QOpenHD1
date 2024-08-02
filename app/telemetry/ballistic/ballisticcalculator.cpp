#include "ballisticcalculator.hpp"
#include "ballisticdatastructures.hpp"

#include <cmath>

namespace {
    constexpr double DEG_TO_RAD = M_PI / 180.0;
}

BallisticCalculator::BallisticCalculator(const ProjectileParameters projectileParams, float focalLenPx, float lenCorrection)
    : m_ballisticMath(projectileParams), m_distanceConverter(focalLenPx, lenCorrection) {}

Point BallisticCalculator::calculateLandingCenterShift_px(const InertialParameters inertial, const AirParameters air,
                                                          const double cogDeg, const double azimuthDeg, const double altitude) const {
    auto shifts = m_ballisticMath.calculateLandingPoint_m(inertial, altitude, air);
    auto shiftX = shifts.first;
    auto shiftY = shifts.second;

    // Calculate angle to rotate visually landing point
    auto anglesDelta = (cogDeg - azimuthDeg) * DEG_TO_RAD; // Convert degrees to radians

    // Calculate rotated coordinates relatively to course over ground
    auto rotatedX = shiftX * std::cos(anglesDelta) - shiftY * std::sin(anglesDelta);
    auto rotatedY = shiftX * std::sin(anglesDelta) + shiftY * std::cos(anglesDelta);

    // Convert length to pixels
    return m_distanceConverter.lenToPx(-rotatedX, rotatedY, altitude);
}
