#ifndef TELEMETRYDATA_H
#define TELEMETRYDATA_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include "IDroneSimulator.h"
#include "DroneFactory.h"

class TelemetryData : public QWidget {
    Q_OBJECT

public:
    explicit TelemetryData(DroneFactory::DroneType droneType = DroneFactory::NormalDrone,
                          QWidget* parent = nullptr);
    ~TelemetryData();

private slots:
    void updateUI(TelemetryPacket data);
    void toggleFailure();
    void changeStrategy(int index);

private:
    IDroneSimulator* droneSimulator;

    // Telemetry displays
    QLabel *latitude;
    QLabel *longitude;
    QLabel *altitude;
    QLabel *heading;
    QLabel *speed;
    QLabel *batteryPercent;
    QLabel *gps;

    // Battery progress bar
    QProgressBar *batteryBar;

    // Controls
    QPushButton *failureButton;
    QComboBox *strategyCombo;

    // Status indicators
    QLabel *droneStatus;
    QLabel *gpsStatus;

    bool failureMode;
};

#endif // TELEMETRYDATA_H
