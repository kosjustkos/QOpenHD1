#pragma once

#include <vector>
#include <utility>
#include <optional>

struct Point {
    int x;
    int y;
};

// Cross-sectional areas in the flight direction
// https://energyeducation.ca/encyclopedia/Cross-sectional_area
struct CrossSectionalAreas {
    double aX;
    double aY;
    double aZ;
};

// Drag coefficients
// https://en.wikipedia.org/wiki/Drag_coefficient
struct DragCoefficients {
    double dX;
    double dY;
    double dZ;
};

struct BallisticStep {
    double x;
    double y;
    double z;
    double vx;
    double vy;
    double vz;
    double time;
};

class BallisticTrajectory {
public:
    void addStep(BallisticStep step);
    std::pair<double, double> landingPoint() const;

private:
    double getLastX() const;
    double getLastY() const;
    std::vector<BallisticStep> m_steps;
};

class ProjectileParameters {
public:
    ProjectileParameters(double weight, const CrossSectionalAreas cs_areas, const DragCoefficients drag_coeffs);

    const double bcx;
    const double bcy;
    const double bcz;
};

class AirDensity {
public:
    AirDensity(std::optional<double> temperature, double altitude);
    double getValue() const;

private:
    double value;
};

struct AirParameters {
    AirDensity density;
    double wind;
    double windAngle;
};

class InertialParameters {
public:
    InertialParameters(double vn, double ve, double pitchRad, double rollRad);

    const double velocity;
    const double horizonAngle;
};
