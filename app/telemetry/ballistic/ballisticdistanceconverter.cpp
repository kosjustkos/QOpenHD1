#include "ballisticdistanceconverter.hpp"
#include "ballisticdatastructures.hpp"

BallisticDistanceConverter::BallisticDistanceConverter(double focalLenPx, double lenCorrection)
    : m_focalLenPx(focalLenPx), m_lenCorrection(lenCorrection) {}

Point BallisticDistanceConverter::lenToPx(double objW, double objH, double altitude) const {
    int objWpx = objW * m_focalLenPx / (altitude * m_lenCorrection);
    int objHpx = objH * m_focalLenPx / (altitude * m_lenCorrection);
    return Point{objWpx, objHpx};
}