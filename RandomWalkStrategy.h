#ifndef RANDOMWALKSTRATEGY_H
#define RANDOMWALKSTRATEGY_H

#include "MovementStrategy.h"
#include <QRandomGenerator>

class RandomWalkStrategy : public MovementStrategy
{
    Q_OBJECT

public:
    explicit RandomWalkStrategy(QObject* parent = nullptr);
    void updatePosition(TelemetryPacket& packet) override;

private:
    double m_targetLat;
    double m_targetLon;
};

#endif // RANDOMWALKSTRATEGY_H
