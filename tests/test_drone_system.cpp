#include <QTest>
#include <QDebug>
#include <QThread>
#include <QSignalSpy>
#include <QtGlobal>
#include <cmath>
#include "../TelemetryPacket.h"
#include "../Logger.h"
#include "../MovementStrategy.h"
#include "../HoverStrategy.h"
#include "../RandomWalkStrategy.h"
#include "../CircularStrategy.h"
#include "../TelemetryWorker.h"
#include "../IDroneSimulator.h"
#include "../DroneFactory.h"
#include "../NormalDroneSimulator.h"

class DroneSystemTest : public QObject
{
    Q_OBJECT

private:
    TelemetryPacket createTestPacket() {
        TelemetryPacket p;
        p.lat = 12.97 + (rand() % 100) / 1000.0;
        p.lon = 77.59 + (rand() % 100) / 1000.0;
        p.alt = 100 + rand() % 50;
        p.heading = rand() % 360;
        p.speed = rand() % 20;
        p.battery = rand() % 100;
        p.gps = (rand() % 2) ? "3D" : "2D";
        return p;
    }

private slots:
    void testPacketUnit() {
        qDebug() << "\nTesting TelemetryPacket";
        
        TelemetryPacket p;
        p.lat = 12.9716;
        p.lon = 77.5946;
        p.alt = 150.5;
        p.heading = 90;
        p.speed = 12.3;
        p.battery = 75;
        p.gps = "3D";
        
        QCOMPARE(p.lat, 12.9716);
        QCOMPARE(p.lon, 77.5946);
        QCOMPARE(p.alt, 150.5);
        QCOMPARE(p.heading, 90);
        QCOMPARE(p.speed, 12.3);
        QCOMPARE(p.battery, 75);
        QCOMPARE(p.gps, QString("3D"));
        
        qDebug() << " Packet creation and access works";
    }
    
    void testLoggerUnit() {
        qDebug() << "\nTesting Logger Singleton";
        
        Logger* log1 = Logger::instance();
        Logger* log2 = Logger::instance();
        
        QVERIFY(log1 != nullptr);
        QVERIFY(log1 == log2);
        
        log1->log("Unit test log message");
        log1->error("Unit test error message");
        
        qDebug() << "Singleton pattern works";
    }
    
    void testHoverUnit() {
        qDebug() << "\nTesting HoverStrategy";
        
        HoverStrategy hover;
        TelemetryPacket p = createTestPacket();
        
        double oldLat = p.lat;
        double oldLon = p.lon;
        double oldAlt = p.alt;
        
        hover.updatePosition(p);
        
        double latDiff = qAbs(p.lat - oldLat);
        double lonDiff = qAbs(p.lon - oldLon);
        double altDiff = qAbs(p.alt - oldAlt);
        
        QVERIFY(latDiff < 0.001);
        QVERIFY(lonDiff < 0.001);
        QVERIFY(altDiff < 5.0);
        
        qDebug() << "\nHover strategy produces minimal movement";
    }
    
    void testRandomWalkUnit() {
        qDebug() << "\nTesting RandomWalkStrategy";
        
        RandomWalkStrategy random;
        TelemetryPacket p = createTestPacket();
        
        double startLat = p.lat;
        double startLon = p.lon;
        
        bool moved = false;
        for(int i = 0; i < 10; i++) {
            random.updatePosition(p);
            if(p.lat != startLat || p.lon != startLon) {
                moved = true;
                break;
            }
        }
        
        QVERIFY(moved);
        QVERIFY(p.heading >= 0 && p.heading < 360);
        
        qDebug() << "Random walk strategy changes position";
    }
    
    void testCircularUnit() {
        qDebug() << "\nTesting CircularStrategy";
        
        CircularStrategy circle;
        TelemetryPacket p = createTestPacket();
        
        circle.updatePosition(p);
        
        QVERIFY(p.speed > 5.0 && p.speed < 20.0);
        QVERIFY(p.heading >= 0 && p.heading < 360);
        
        qDebug() << "Circular strategy produces valid output";
    }
    
    void testWorkerUnit() {
        qDebug() << "\nTesting TelemetryWorker";
        
        TelemetryWorker worker;
        
        QSignalSpy spy(&worker, &TelemetryWorker::telemetryUpdated);
        
        worker.simulate();
        
        QCOMPARE(spy.count(), 1);
        
        TelemetryPacket p = spy.last().at(0).value<TelemetryPacket>();
        
        QVERIFY(p.battery >= 0 && p.battery <= 100);
        QVERIFY(p.gps == "2D" || p.gps == "3D" || p.gps == "No Fix");
        
        worker.setFailureMode(true);
        worker.simulate();
        
        QCOMPARE(spy.count(), 2);
        p = spy.last().at(0).value<TelemetryPacket>();
        
        if(p.gps == "No Fix") {
            qDebug() << "Failure mode affects GPS";
        }
        
        qDebug() << "Worker generates valid telemetry";
    }
    
    void testFactoryUnit() {
        qDebug() << "\nTesting DroneFactory";
        
        IDroneSimulator* drone = DroneFactory::createDrone(DroneFactory::NormalDrone);
        
        QVERIFY(drone != nullptr);
        
        NormalDroneSimulator* normalDrone = qobject_cast<NormalDroneSimulator*>(drone);
        QVERIFY(normalDrone != nullptr);
        
        delete drone;
        qDebug() << "Factory creates correct drone type";
    }
    
    void testFactoryDroneWorkerIntegration() {
        qDebug() << "\nFactory → Drone → Worker";
        
        IDroneSimulator* drone = DroneFactory::createDrone(DroneFactory::NormalDrone);
        QVERIFY(drone != nullptr);
        
        bool signalReceived = false;
        QObject::connect(drone, &IDroneSimulator::telemetryUpdated,
            [&](TelemetryPacket p) {
                signalReceived = true;
                qDebug() << "Received telemetry - Battery:" << p.battery << "%";
            });
        
        drone->start();
        
        QTest::qWait(1500);
        
        QVERIFY(signalReceived);
        qDebug() << "Factory, Drone, and Worker all work together";
        
        delete drone;
    }
    
    void testStrategyTelemetryIntegration() {
        qDebug() << "\nStrategy → Telemetry";
        
        TelemetryWorker worker;
        HoverStrategy strategy;
        
        TelemetryPacket lastPacket;
        int updateCount = 0;
        
        QObject::connect(&worker, &TelemetryWorker::telemetryUpdated,
            [&](TelemetryPacket packet) {
                strategy.updatePosition(packet);
                lastPacket = packet;
                updateCount++;
            });
        
        for(int i = 0; i < 5; i++) {
            worker.simulate();
            QTest::qWait(50);
        }
        
        QVERIFY(updateCount > 0);
        QVERIFY(lastPacket.heading >= 0 && lastPacket.heading < 360);
        
        qDebug() << "Strategy successfully modifies telemetry data";
    }
    
    void testFailurePropagationIntegration() {
        qDebug() << "\nUI → Drone → Worker (Failure Mode)";
        
        NormalDroneSimulator drone;
        
        bool failureDetected = false;
        
        QObject::connect(&drone, &IDroneSimulator::telemetryUpdated,
            [&](TelemetryPacket p) {
                if(p.gps == "No Fix") {
                    failureDetected = true;
                }
            });
        
        drone.start();
        QTest::qWait(500);
        
        qDebug() << "Enabling failure mode...";
        drone.setFailureMode(true);
        
        QTest::qWait(1500);
        
        QVERIFY(failureDetected);
        qDebug() << "Failure mode propagates through entire system";
    }

    void testStrategyChangeIntegration() {
        qDebug() << "\nChanging strategy mid-flight";
        
        NormalDroneSimulator drone;
        
        QList<TelemetryPacket> packets;
        
        QObject::connect(&drone, &IDroneSimulator::telemetryUpdated,
            [&](TelemetryPacket p) {
                packets.append(p);
            });
        
        drone.setStrategy("Hover");
        drone.start();
        QTest::qWait(1000);
        
        qDebug() << "Switching to Random Walk...";
        drone.setStrategy("Random Walk");
        QTest::qWait(1000);
        
        qDebug() << "Switching to Circular...";
        drone.setStrategy("Circular");
        QTest::qWait(1000);
        
        QVERIFY(packets.count() > 4);
        qDebug() << "Strategy changes without crashing, received" << packets.count() << "updates";
    }
};

QTEST_MAIN(DroneSystemTest)
#include "test_drone_system.moc"
