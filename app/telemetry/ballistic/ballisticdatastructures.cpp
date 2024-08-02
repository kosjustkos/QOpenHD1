#include <cmath>

#include "ballisticdatastructures.hpp"

namespace {
    constexpr double RAD_TO_DEG = 180.0 / M_PI;

    constexpr double SEA_LEVEL_PRESSURE_P = 101325.0;   // Atmospheric pressure at sea level in Pascals
    constexpr double G_ACCEL = 9.80665;                 // Gravity acceleration in m/s^2
    constexpr double TEMP_LAPSE_RATE = -0.0065;         // Temperature lapse rate in K/m (rate at which atmospheric temperature decreases with an increase in altitude)
    constexpr double IDEAL_GAS_CONSTANT = 8.31447;      // Ideal gas constant in J/(mol·K)
    constexpr double AIR_MOLAR_MASS = 0.0289644;        // Molar mass of dry air in kg/mol
    constexpr double SEA_LEVEL_TEMP_K = 288.15;         // Standard temperature at sea level in K
    constexpr double DEFAULT_AIR_DENSITY = 1.225;       // Default air density at sea level, standard conditions
}

void BallisticTrajectory::addStep(BallisticStep step) {
    m_steps.push_back(step);
}

double BallisticTrajectory::getLastX() const {
    if (m_steps.empty()) {
        return 0;
    }
    return m_steps.back().x;
}

double BallisticTrajectory::getLastY() const {
    if (m_steps.empty()) {
        return 0;
    }
    return m_steps.back().y;
}

std::pair<double, double> BallisticTrajectory::landingPoint() const {
    return std::make_pair(getLastX(), getLastY());
}

ProjectileParameters::ProjectileParameters(double weight, const CrossSectionalAreas cs_areas, const DragCoefficients drag_coeffs)
    : bcx(weight / (drag_coeffs.dX * cs_areas.aX)),
      bcy(weight / (drag_coeffs.dY * cs_areas.aY)),
      bcz(weight / (drag_coeffs.dZ * cs_areas.aZ)) {
}

AirDensity::AirDensity(std::optional<double> temperature, double altitude) {
    if (!temperature.has_value() || altitude < 0) {
        value = DEFAULT_AIR_DENSITY;
    } else {
        // Calculate density p at the given height h using the barometric formula
        double p = SEA_LEVEL_PRESSURE_P * std::pow(1 + TEMP_LAPSE_RATE * altitude / SEA_LEVEL_TEMP_K, -G_ACCEL * AIR_MOLAR_MASS / (IDEAL_GAS_CONSTANT * TEMP_LAPSE_RATE));

        // Convert temperature from Celsius to Kelvin
        double temp_kelvin = temperature.value() + 273.15;

        // Calculate the air density
        value = p * AIR_MOLAR_MASS / (IDEAL_GAS_CONSTANT * temp_kelvin);
    }
}

double AirDensity::getValue() const {
    return value;
}

InertialParameters::InertialParameters(double vn, double ve, double pitchRad, double rollRad)
    : velocity(std::sqrt(vn * vn + ve * ve)),
      horizonAngle(RAD_TO_DEG * std::atan(std::sqrt(std::pow(std::tan(pitchRad), 2) + std::pow(std::tan(rollRad), 2)))) {
}
