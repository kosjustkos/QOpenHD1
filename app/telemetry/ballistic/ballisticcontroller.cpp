#include <cmath>

#include "../models/fcmavlinksystem.h"
#include "ballisticcontroller.hpp"
#include "ballisticdatastructures.hpp"

namespace {
    constexpr double DEG_TO_RAD = M_PI / 180.0;
}

BallisticController::BallisticController()
    : m_calculator(ProjectileParameters(0.5, CrossSectionalAreas{0.0113, 0.0113, 0.0216}, DragCoefficients{1.82, 1.82, 1.52}), 902.0f, 1.0f) {    
    // focal len and len correction are hardcoded for now. Will be passed from some external source (air unit?)
}

BallisticController& BallisticController::instance() {
    static BallisticController instance;
    return instance;
}

QList<int> BallisticController::getBallisticShifts() const {
    FCMavlinkSystem &mavlink = FCMavlinkSystem::instance();

    auto vn = mavlink.vx();
    auto ve = mavlink.vy();
    auto pitchRad = DEG_TO_RAD * mavlink.pitch();
    auto rollRad = DEG_TO_RAD * mavlink.roll();
    auto azimuthDeg = mavlink.hdg();
    auto cogDeg = mavlink.cog();
    auto altitude = mavlink.altitude_rel_m();    
    auto wind = mavlink.wind_speed();
    auto windAngle = mavlink.wind_direction();
    std::optional<double> temperature;           // no temperature source for now

    InertialParameters inertial(vn, ve, pitchRad, rollRad);
    AirDensity airDensity(temperature, altitude);
    AirParameters air{airDensity, wind, windAngle};

    auto shifts = m_calculator.calculateLandingCenterShift_px(inertial, air, cogDeg, azimuthDeg, altitude);

    QList<int> shifts_qt;
    
    shifts_qt.append(shifts.x);
    shifts_qt.append(shifts.y);
    
    return shifts_qt;
}