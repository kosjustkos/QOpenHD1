#include <cmath>
#include <qsettings.h>
#include <qdebug.h>

#include "../models/fcmavlinksystem.h"
#include "ballisticcontroller.hpp"
#include "ballisticdatastructures.hpp"

namespace {
    constexpr double DEG_TO_RAD = M_PI / 180.0;
    constexpr int UPDATE_TIMER_INTERVAL_MS = 200;

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
    m_ballisticShifts.append(0);
    m_ballisticShifts.append(0);
    m_recalculate_timer = std::make_unique<QTimer>(this);
    QObject::connect(m_recalculate_timer.get(), &QTimer::timeout, this, &BallisticController::recalculateBallistic);
    
    QSettings settings;
    const bool ballistic_widget_is_on = settings.value("show_ballistic_widget", true).toBool();
    if (ballistic_widget_is_on) {
        qDebug()<<"Ballistic widget is ON";
        startCalculation();
    } else {
        qDebug()<<"Ballistic widget is OFF";
    }
}

BallisticController& BallisticController::instance() {
    static BallisticController instance;
    return instance;
}

QString BallisticController::getDebugInfo() const {
    return m_debugInfo;
}

QList<int> BallisticController::getBallisticShifts() const {
    return m_ballisticShifts;
}

void BallisticController::setBallisticShifts(const QList<int> shifts) {
    m_ballisticShifts = shifts;
    emit ballisticShiftsChanged(m_ballisticShifts);
}

void BallisticController::setDebugInfo(const QString info) {
    m_debugInfo = info;
    emit debugInfoChanged(m_debugInfo);
}

void BallisticController::startCalculation() {
    m_recalculate_timer->start(UPDATE_TIMER_INTERVAL_MS);
}

void BallisticController::stopCalculation() {
    m_recalculate_timer->stop();
}

void BallisticController::recalculateBallistic() {
    MavlinkData mavData = MavlinkData(&FCMavlinkSystem::instance());
    
    InertialParameters inertial(mavData.vn, mavData.ve, DEG_TO_RAD * mavData.pitchDeg, DEG_TO_RAD * mavData.rollDeg);
    AirDensity airDensity(mavData.temperature, mavData.altitude);
    AirParameters air{airDensity, mavData.wind, mavData.windAngle};

    auto shifts = m_calculator.calculateLandingCenterShift_px(inertial, air, mavData.cogDeg, mavData.azimuthDeg, mavData.altitude);

    QList<int> qt_shifts;
    qt_shifts.append(shifts.x);
    qt_shifts.append(shifts.y);
    setBallisticShifts(qt_shifts);

    setDebugInfo("vn: " + QString::number(mavData.vn, 'f', 2) +
                " ve: " + QString::number(mavData.ve, 'f', 2) +
                " pitch: " + QString::number(mavData.pitchDeg, 'f', 2) +
                " roll: " + QString::number(mavData.rollDeg, 'f', 2) +
                " azm: " + QString::number(mavData.azimuthDeg, 'f', 2) + 
                " cog: " + QString::number(mavData.cogDeg, 'f', 2) + 
                " alt: " + QString::number(mavData.altitude, 'f', 2));
}
