#ifndef NORMALDRONESIMULATOR_H
#define NORMALDRONESIMULATOR_H

#include <QObject>
#include <QThread>
#include "IDroneSimulator.h"
#include "TelemetryWorker.h"
#include "MovementStrategy.h"
class NormalDroneSimulator : public IDroneSimulator
{
    Q_OBJECT
public:
    explicit NormalDroneSimulator(QObject* parent = nullptr);
    ~NormalDroneSimulator();

    void start() override;
    void setFailureMode(bool enabled) override;
    void setStrategy(const QString& strategyName);

private:
    QThread* workerThread;
    TelemetryWorker* worker;
    MovementStrategy* m_strategy;
};

#endif // NORMALDRONESIMULATOR_H
