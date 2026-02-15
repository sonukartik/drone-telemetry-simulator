#ifndef MOVEMENTSTRATEGY_H
#define MOVEMENTSTRATEGY_H

#include <QObject>
#include "TelemetryPacket.h"

class MovementStrategy : public QObject
{
    Q_OBJECT

public:
    explicit MovementStrategy(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~MovementStrategy() {}
    virtual void updatePosition(TelemetryPacket& packet) = 0;
};

#endif // MOVEMENTSTRATEGY_H
