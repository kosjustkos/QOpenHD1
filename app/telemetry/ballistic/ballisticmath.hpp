#pragma once

#include <utility>

#include "ballisticdatastructures.hpp"

class BallisticMath {
public:
    explicit BallisticMath(const ProjectileParameters& projectileParams);

    std::pair<double, double> calculateLandingPoint_m(const InertialParameters inertialParams, double altitude, const AirParameters airParams) const;    

private:
    BallisticTrajectory calculateTrajectory(const InertialParameters inertialParams, double altitude, const AirParameters airParams, int tdiv) const;
    const ProjectileParameters m_projectileParams;
};
