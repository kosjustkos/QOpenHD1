#pragma once

#include <utility>
#include <QObject>
#include <QList>

#include "ballisticcalculator.hpp"

class BallisticCalculator;

class BallisticController: public QObject {
    Q_OBJECT

public:
    static BallisticController& instance();
    BallisticController();
    Q_INVOKABLE QList<int> getBallisticShifts() const;
    Q_INVOKABLE QString getDebugInfo() const;

private:    
    const BallisticCalculator m_calculator;
    std::pair<int, int> getMockedShifts();
};
