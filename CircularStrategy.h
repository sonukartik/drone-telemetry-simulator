#ifndef CIRCULARSTRATEGY_H
#define CIRCULARSTRATEGY_H

#include "MovementStrategy.h"

class CircularStrategy : public MovementStrategy
{
    Q_OBJECT

public:
    explicit CircularStrategy(QObject* parent = nullptr);
    void updatePosition(TelemetryPacket& packet) override;

private:
    double m_angle;
    double m_centerLat;
    double m_centerLon;
};

#endif // CIRCULARSTRATEGY_H
