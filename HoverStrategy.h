#ifndef HOVERSTRATEGY_H
#define HOVERSTRATEGY_H

#include "MovementStrategy.h"

class HoverStrategy : public MovementStrategy
{
    Q_OBJECT

public:
    explicit HoverStrategy(QObject* parent = nullptr);
    void updatePosition(TelemetryPacket& packet) override;
};

#endif // HOVERSTRATEGY_H
