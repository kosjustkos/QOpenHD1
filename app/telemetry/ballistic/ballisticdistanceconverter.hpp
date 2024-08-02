#pragma once

#include <utility>

struct Point;

class BallisticDistanceConverter {
public:
    BallisticDistanceConverter(double focalLenPx, double lenCorrection);

    Point lenToPx(double objW, double objH, double altitude) const;

private:
    const double m_focalLenPx;
    const double m_lenCorrection;
};
