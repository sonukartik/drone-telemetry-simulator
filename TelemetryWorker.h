#ifndef TELEMETRYWORKER_H
#define TELEMETRYWORKER_H

#include <QTimer>
#include "TelemetryPacket.h"

class TelemetryWorker : public QObject
{
    Q_OBJECT
public:
    explicit TelemetryWorker(QObject *parent = nullptr);
    void setFailureMode(bool enabled);
    bool isActive() const;  // New method

signals:
    void telemetryUpdated(TelemetryPacket data);

public slots:
    void start();
    void simulate();

private:
    QTimer *timer;
    int currentBattery = 100;
    bool failuremode = false;
    bool m_isActive;  // New flag
};

#endif // TELEMETRYWORKER_H
