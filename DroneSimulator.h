#ifndef DRONESIMULATOR_H
#define DRONESIMULATOR_H
#include<QObject>
#include "IDroneSimulator.h"
class NormalDroneSimulator : public IDroneSimulator
{
        Q_OBJECT
    public:
        void start();
        void setFailureMode(bool enabled);
        explicit NormalDroneSimulator(QObject* parent = nullptr);
    signals:
        void telemetryUpdated(TelemetryPacket data);
};
#endif // DRONESIMULATOR_H
