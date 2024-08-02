#include <cmath>

#include "ballisticmath.hpp"

namespace {
    constexpr double GRAVITY = 9.80665;
    constexpr int INTEGRATION_TIME_STEPS = 100;
}

BallisticMath::BallisticMath(const ProjectileParameters& projectileParams)
    : m_projectileParams(projectileParams) {}

std::pair<double, double> BallisticMath::calculateLandingPoint_m(const InertialParameters inertialParams, double altitude, const AirParameters airParams) const {
    BallisticTrajectory trajectory = calculateTrajectory(inertialParams, altitude, airParams, INTEGRATION_TIME_STEPS);
    return trajectory.landingPoint();
}

BallisticTrajectory BallisticMath::calculateTrajectory(const InertialParameters inertialParams, double altitude, const AirParameters airParams, int tdiv) const {
    auto horizonAngle = inertialParams.horizonAngle * (M_PI / 180.0);

    double vx = inertialParams.velocity * std::cos(horizonAngle);
    double vy = 0.0;
    double vz = inertialParams.velocity * std::sin(horizonAngle);

    double windAngle = airParams.windAngle * (M_PI / 180.0);
    double windVx = airParams.wind * std::cos(windAngle);
    double windVy = airParams.wind * std::sin(windAngle);

    double x = 0;
    double y = 0;
    double time = 0;
    double z = altitude;

    BallisticTrajectory trajectory;

    while (z > 0.0) {
        trajectory.addStep(BallisticStep{x, y, z, vx, vy, vz, time});

        auto wvx = vx - windVx;
        auto wvy = windVy - vy;
        auto aax = (airParams.density.getValue() * wvx * wvx / 2) / m_projectileParams.bcx / tdiv;
        vx += (windVx < vx) ? -aax : aax;
        auto aay = (airParams.density.getValue() * wvy * wvy / 2) / m_projectileParams.bcy / tdiv;
        vy += (windVy < 0) ? -aay : aay;
        auto aaz = (airParams.density.getValue() * vz * vz / 2) / m_projectileParams.bcz / tdiv;
        vz += ((vz > 0) ? -aaz : aaz) - (GRAVITY / tdiv);

        x += vx / tdiv;
        y += vy / tdiv;
        z += vz / tdiv;
        time += 1.0 / tdiv;
    }

    return trajectory;
}