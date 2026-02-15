#include "mainwindow.h"
#include "TelemetryData.h"
#include "Logger.h"
#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QFormLayout>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Logger::instance()->log("=== Drone System Started ===");
    TelemetryData window(DroneFactory::NormalDrone);
    window.show();
    int result = a.exec();
    Logger::instance()->log("=== Drone System Closed ===");
    return result;
}
