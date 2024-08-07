#pragma once

#include <utility>
#include <QObject>
#include <QList>
#include <QString>
#include <QTimer>
#include <memory>

#include "ballisticcalculator.hpp"

class BallisticCalculator;

class BallisticController: public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<int> ballisticShifts READ getBallisticShifts WRITE setBallisticShifts NOTIFY ballisticShiftsChanged)
    Q_PROPERTY(QString debugInfo READ getDebugInfo WRITE setDebugInfo NOTIFY debugInfoChanged)

public:
    static BallisticController& instance();
    BallisticController();
    
    Q_INVOKABLE QList<int> getBallisticShifts() const;
    Q_INVOKABLE QString getDebugInfo() const;
    Q_INVOKABLE void startCalculation();
    Q_INVOKABLE void stopCalculation();

signals:
    void ballisticShiftsChanged(const QList<int>& shifts);
    void debugInfoChanged(const QString info);

private:    
    const BallisticCalculator m_calculator;
    std::unique_ptr<QTimer> m_recalculate_timer = nullptr;
    QList<int> m_ballisticShifts;
    QString m_debugInfo;

    void setBallisticShifts(const QList<int> shifts);
    void setDebugInfo(const QString info);
    void recalculateBallistic();
};
