#include "TelemetryWorker.h"
#include "Logger.h"
#include <QRandomGenerator>
static int counter = 0;
TelemetryWorker::TelemetryWorker(QObject *parent)
    : QObject(parent)
    , currentBattery(100)
    , failuremode(false)
    , m_isActive(true)  // New flag
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TelemetryWorker::simulate);
    Logger::instance()->log("TelemetryWorker created");
}

void TelemetryWorker::start()
{
    if (timer) {
        timer->start(500);
        Logger::instance()->log("TelemetryWorker started");
    }
}

void TelemetryWorker::simulate()
{
    // If battery is 0, drone is dead - no more updates
    if (currentBattery <= 0) {
        if (m_isActive) {
            m_isActive = false;
            timer->stop();
            Logger::instance()->error("DRONE SHUTDOWN - Battery depleted");

            // Send one final packet with zero battery
            TelemetryPacket finalPacket;
            finalPacket.lat = 12.97;
            finalPacket.lon = 77.59;
            finalPacket.alt = 0;
            finalPacket.heading = 0;
            finalPacket.speed = 0;
            finalPacket.battery = 0;
            finalPacket.gps = "No Fix";
            emit telemetryUpdated(finalPacket);
        }
        return;
    }

    TelemetryPacket packet;

    // Initialize packet with default values
    packet.lat = 12.97;
    packet.lon = 77.59;
    packet.alt = 100;
    packet.heading = 0;
    packet.speed = 0;
    packet.gps = "No Fix";

    // Update battery
    if(failuremode){
        currentBattery -=5;
    }
    else{
        if(counter % 5 == 0){
            currentBattery -=1;
        }
    }
    counter++;
    if (currentBattery < 0) currentBattery = 0;
    packet.battery = currentBattery;

    // Set GPS status (only if battery > 0)
    if (!failuremode && currentBattery > 0) {
        packet.gps = (QRandomGenerator::global()->bounded(2) == 0) ? "2D" : "3D";
    }

    // Warning at low battery
    if (currentBattery < 20 && currentBattery > 0) {
        Logger::instance()->error("Low battery: " + QString::number(currentBattery) + "%");
    }

    emit telemetryUpdated(packet);
}

void TelemetryWorker::setFailureMode(bool enabled)
{
    failuremode = enabled;
    Logger::instance()->log(QString("Failure mode: %1").arg(enabled ? "ON" : "OFF"));
}

bool TelemetryWorker::isActive() const
{
    return m_isActive;
}
