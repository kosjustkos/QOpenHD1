#pragma once

#include <utility>

#include "ballisticmath.hpp"
#include "ballisticdistanceconverter.hpp"
#include "fisheyecorrector.hpp"

struct Point;
class ProjectileParameters;
class InertialParameters;
class AirParameters;

class BallisticCalculator {
public:
    BallisticCalculator(const ProjectileParameters projectileParams, const DistortionCoefficients distortionCoeff, float focalLenPx, float lenCorrection);

    Point calculateLandingCenterShift_px(const InertialParameters inertial, const AirParameters air,
                                                                            const double cogDeg, const double azimuthDeg, const double altitude) const;
private:
    const BallisticMath m_ballisticMath;
    const BallisticDistanceConverter m_distanceConverter;
    const FishEyeCorrector m_fisheyeCorrector;
};
