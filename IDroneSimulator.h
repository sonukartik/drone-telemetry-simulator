#ifndef IDRONESIMULATOR_H
#define IDRONESIMULATOR_H
#include <QObject>
#include "TelemetryPacket.h"
class IDroneSimulator : public QObject
{
    Q_OBJECT

public:
    explicit IDroneSimulator(QObject* parent = nullptr)
        : QObject(parent) {}

    virtual void start() = 0;
    virtual void setFailureMode(bool enabled) = 0;
signals:
    void telemetryUpdated(TelemetryPacket data);
};
#endif // IDRONESIMULATOR_H
