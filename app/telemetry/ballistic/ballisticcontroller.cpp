#include <cmath>

#include "../models/fcmavlinksystem.h"
#include "ballisticcontroller.hpp"
#include "ballisticdatastructures.hpp"

namespace {
    constexpr double DEG_TO_RAD = M_PI / 180.0;

    struct MavlinkData {
        double vn;
        double ve;
        double pitchDeg;
        double rollDeg;
        double azimuthDeg;
        double cogDeg;
        double altitude;
        double wind;
        double windAngle;
        std::optional<double> temperature;

        MavlinkData(FCMavlinkSystem *mavlink)
        : vn(mavlink->vx()), ve(mavlink->vy()), pitchDeg(mavlink->pitch()), rollDeg(mavlink->roll()), azimuthDeg(mavlink->hdg()),
          cogDeg(mavlink->cog()), altitude(mavlink->altitude_rel_m()), wind(mavlink->wind_speed()), windAngle(mavlink->wind_direction()), temperature(std::nullopt) {
            // no temperature source for now
        }
    };
}

BallisticController::BallisticController()
    : m_calculator(ProjectileParameters(0.5, CrossSectionalAreas{0.0113, 0.0113, 0.0216}, DragCoefficients{1.82, 1.82, 1.52}), DistortionCoefficients{0.04326691, -0.04997936,  0.00245744,  0.00115149, -0.01736539}, 902.0f, 1.0f) {    
    // camera distortion coefficients, focal len and len correction are hardcoded for now. Will be passed from some external source (air unit?)
}

BallisticController& BallisticController::instance() {
    static BallisticController instance;
    return instance;
}

QList<int> BallisticController::getBallisticShifts() const {
    MavlinkData mavData = MavlinkData(&FCMavlinkSystem::instance());

    InertialParameters inertial(mavData.vn, mavData.ve, DEG_TO_RAD * mavData.pitchDeg, DEG_TO_RAD * mavData.rollDeg);
    AirDensity airDensity(mavData.temperature, mavData.altitude);
    AirParameters air{airDensity, mavData.wind, mavData.windAngle};

    auto shifts = m_calculator.calculateLandingCenterShift_px(inertial, air, mavData.cogDeg, mavData.azimuthDeg, mavData.altitude);

    QList<int> shifts_qt;
    
    shifts_qt.append(shifts.x);
    shifts_qt.append(shifts.y);
    
    return shifts_qt;
}

QString BallisticController::getDebugInfo() {
    MavlinkData mavData = MavlinkData(&FCMavlinkSystem::instance());
    return "vn: " + QString::number(mavData.vn, 'f', 2) +
            " ve: " + QString::number(mavData.ve, 'f', 2) +
            " pitch: " + QString::number(mavData.pitchDeg, 'f', 2) +
            " roll: " + QString::number(mavData.rollDeg, 'f', 2) +
            " azm: " + QString::number(mavData.azimuthDeg, 'f', 2) + 
            " cog: " + QString::number(mavData.cogDeg, 'f', 2) + 
            " alt: " + QString::number(mavData.altitude, 'f', 2);
}