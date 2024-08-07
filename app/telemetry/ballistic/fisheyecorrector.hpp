#pragma once

#include "ballisticdatastructures.hpp"

class FishEyeCorrector {
    public:
        explicit FishEyeCorrector(const double focalLenPx, const DistortionCoefficients distortion);
        Point applyFishEyeDistortion(const Point undistortedPoint) const;

    private:
        double m_focalLenPx;
        DistortionCoefficients m_distortion;
};