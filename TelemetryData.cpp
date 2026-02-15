#include "TelemetryData.h"
#include "Logger.h"
#include "NormalDroneSimulator.h"
#include <QFont>
#include <QPalette>

TelemetryData::TelemetryData(DroneFactory::DroneType droneType, QWidget* parent)
    : QWidget(parent)
    , failureMode(false)
{
    Logger::instance()->log("TelemetryData UI created");
    qRegisterMetaType<TelemetryPacket>("TelemetryPacket");

    setWindowTitle("Drone Command Center");
    setMinimumSize(500, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // ==================== HEADER ====================
    QLabel *titleLabel = new QLabel("DRONE TELEMETRY DASHBOARD");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("Arial", 16, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2c3e50; padding: 10px;");
    mainLayout->addWidget(titleLabel);

    // ==================== STATUS BAR ====================
    QHBoxLayout *statusLayout = new QHBoxLayout();

    droneStatus = new QLabel("● ACTIVE");
    droneStatus->setStyleSheet("color: green; font-weight: bold;");
    statusLayout->addWidget(droneStatus);

    statusLayout->addStretch();

    gpsStatus = new QLabel("GPS: 3D");
    gpsStatus->setStyleSheet("background-color: #27ae60; color: white; padding: 5px; border-radius: 3px;");
    statusLayout->addWidget(gpsStatus);

    mainLayout->addLayout(statusLayout);

    // ==================== TELEMETRY DATA GROUP ====================
    QGroupBox *telemetryGroup = new QGroupBox("Live Telemetry Data");
    telemetryGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 14px; }");

    QGridLayout *telemetryLayout = new QGridLayout(telemetryGroup);
    telemetryLayout->setVerticalSpacing(15);
    telemetryLayout->setHorizontalSpacing(20);

    // Row 1: Position
    QLabel *latLabel = new QLabel("Latitude:");
    latLabel->setStyleSheet("font-weight: bold; color: #34495e;");
    latitude = new QLabel("12.970000° N");
    latitude->setStyleSheet("background-color: #ecf0f1; padding: 8px; border-radius: 3px; font-family: monospace;");

    QLabel *lonLabel = new QLabel("Longitude:");
    lonLabel->setStyleSheet("font-weight: bold; color: #34495e;");
    longitude = new QLabel("77.590000° E");
    longitude->setStyleSheet("background-color: #ecf0f1; padding: 8px; border-radius: 3px; font-family: monospace;");

    telemetryLayout->addWidget(latLabel, 0, 0);
    telemetryLayout->addWidget(latitude, 0, 1);
    telemetryLayout->addWidget(lonLabel, 0, 2);
    telemetryLayout->addWidget(longitude, 0, 3);

    // Row 2: Altitude & Heading
    QLabel *altLabel = new QLabel("Altitude:");
    altLabel->setStyleSheet("font-weight: bold; color: #34495e;");
    altitude = new QLabel("100.0 m");
    altitude->setStyleSheet("background-color: #ecf0f1; padding: 8px; border-radius: 3px; font-family: monospace;");

    QLabel *headLabel = new QLabel("Heading:");
    headLabel->setStyleSheet("font-weight: bold; color: #34495e;");
    heading = new QLabel("0°");
    heading->setStyleSheet("background-color: #ecf0f1; padding: 8px; border-radius: 3px; font-family: monospace;");

    telemetryLayout->addWidget(altLabel, 1, 0);
    telemetryLayout->addWidget(altitude, 1, 1);
    telemetryLayout->addWidget(headLabel, 1, 2);
    telemetryLayout->addWidget(heading, 1, 3);

    // Row 3: Speed & GPS
    QLabel *speedLabel = new QLabel("Speed:");
    speedLabel->setStyleSheet("font-weight: bold; color: #34495e;");
    speed = new QLabel("0.0 m/s");
    speed->setStyleSheet("background-color: #ecf0f1; padding: 8px; border-radius: 3px; font-family: monospace;");

    QLabel *gpsLabel = new QLabel("GPS Fix:");
    gpsLabel->setStyleSheet("font-weight: bold; color: #34495e;");
    gps = new QLabel("3D");
    gps->setStyleSheet("background-color: #ecf0f1; padding: 8px; border-radius: 3px; font-family: monospace;");

    telemetryLayout->addWidget(speedLabel, 2, 0);
    telemetryLayout->addWidget(speed, 2, 1);
    telemetryLayout->addWidget(gpsLabel, 2, 2);
    telemetryLayout->addWidget(gps, 2, 3);

    mainLayout->addWidget(telemetryGroup);

    // ==================== BATTERY STATUS ====================
    QGroupBox *batteryGroup = new QGroupBox("Power Status");
    batteryGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 14px; }");

    QVBoxLayout *batteryLayout = new QVBoxLayout(batteryGroup);

    batteryBar = new QProgressBar();
    batteryBar->setRange(0, 100);
    batteryBar->setValue(100);
    batteryBar->setTextVisible(true);
    batteryBar->setFormat("%p% remaining");
    batteryBar->setStyleSheet(
        "QProgressBar { border: 2px solid grey; border-radius: 5px; height: 25px; }"
        "QProgressBar::chunk { background-color: #27ae60; border-radius: 3px; }"
    );

    batteryPercent = new QLabel("100%");
    batteryPercent->setAlignment(Qt::AlignCenter);
    batteryPercent->setStyleSheet("font-size: 12px; color: #2c3e50;");

    batteryLayout->addWidget(batteryBar);
    batteryLayout->addWidget(batteryPercent);

    mainLayout->addWidget(batteryGroup);

    // ==================== CONTROL PANEL ====================
    QGroupBox *controlGroup = new QGroupBox("Drone Controls");
    controlGroup->setStyleSheet("QGroupBox { font-weight: bold; font-size: 14px; }");

    QGridLayout *controlLayout = new QGridLayout(controlGroup);
    controlLayout->setVerticalSpacing(15);

    // Strategy selection
    QLabel *strategyLabel = new QLabel("Flight Pattern:");
    strategyLabel->setStyleSheet("font-weight: bold;");

    strategyCombo = new QComboBox();
    strategyCombo->addItem("Hover");
    strategyCombo->addItem("Random Walk");
    strategyCombo->addItem("Circular");
    strategyCombo->setStyleSheet(
        "QComboBox { padding: 5px; border: 1px solid #bdc3c7; border-radius: 3px; }"
    );

    controlLayout->addWidget(strategyLabel, 0, 0);
    controlLayout->addWidget(strategyCombo, 0, 1);

    // Failure mode button
    QLabel *failureLabel = new QLabel("Failure Mode:");
    failureLabel->setStyleSheet("font-weight: bold;");

    failureButton = new QPushButton("⚠ SIMULATE FAILURE");
    failureButton->setCheckable(true);
    failureButton->setStyleSheet(
        "QPushButton { background-color: #e74c3c; color: white; font-weight: bold; padding: 10px; border-radius: 5px; }"
        "QPushButton:checked { background-color: #c0392b; }"
    );

    controlLayout->addWidget(failureLabel, 1, 0);
    controlLayout->addWidget(failureButton, 1, 1);

    mainLayout->addWidget(controlGroup);

    // ==================== FOOTER ====================
    QLabel *footerLabel = new QLabel("Drone Simulation System v1.0");
    footerLabel->setAlignment(Qt::AlignCenter);
    footerLabel->setStyleSheet("color: #7f8c8d; font-size: 10px; margin-top: 10px;");
    mainLayout->addWidget(footerLabel);

    // Create drone simulator
    droneSimulator = DroneFactory::createDrone(droneType, this);

    // Connect signals
    connect(failureButton, &QPushButton::clicked,
            this, &TelemetryData::toggleFailure);

    connect(strategyCombo, QOverload<int>::of(&QComboBox::activated),
            this, &TelemetryData::changeStrategy);

    connect(droneSimulator, &IDroneSimulator::telemetryUpdated,
            this, &TelemetryData::updateUI);

    droneSimulator->start();
}

void TelemetryData::updateUI(TelemetryPacket data)
{
    // Update position
    latitude->setText(QString::number(qAbs(data.lat), 'f', 6) + "° ");
    longitude->setText(QString::number(qAbs(data.lon), 'f', 6) + "° ");
    altitude->setText(QString::number(data.alt, 'f', 1) + " m");

    // Add compass direction to heading
    heading->setText(QString::number(data.heading) + "° ");

    speed->setText(QString::number(data.speed, 'f', 1) + " m/s");
    gps->setText(data.gps);

    // Update battery
    batteryBar->setValue(data.battery);
    batteryPercent->setText(QString::number(data.battery) + "%");

    // Change battery bar color based on level
    if (data.battery <= 20) {
        batteryBar->setStyleSheet(
            "QProgressBar { border: 2px solid grey; border-radius: 5px; height: 25px; }"
            "QProgressBar::chunk { background-color: #e74c3c; border-radius: 3px; }"
        );
    } else if (data.battery <= 50) {
        batteryBar->setStyleSheet(
            "QProgressBar { border: 2px solid grey; border-radius: 5px; height: 25px; }"
            "QProgressBar::chunk { background-color: #f39c12; border-radius: 3px; }"
        );
    } else {
        batteryBar->setStyleSheet(
            "QProgressBar { border: 2px solid grey; border-radius: 5px; height: 25px; }"
            "QProgressBar::chunk { background-color: #27ae60; border-radius: 3px; }"
        );
    }

    // Update status indicators
    if (data.battery == 0) {
        droneStatus->setText("● SHUTDOWN");
        droneStatus->setStyleSheet("color: red; font-weight: bold;");
        gpsStatus->setText("GPS: LOST");
        gpsStatus->setStyleSheet("background-color: #e74c3c; color: white; padding: 5px; border-radius: 3px;");
    } else if (failureMode) {
        droneStatus->setText("● FAILURE MODE");
        droneStatus->setStyleSheet("color: orange; font-weight: bold;");
    } else {
        droneStatus->setText("● ACTIVE");
        droneStatus->setStyleSheet("color: green; font-weight: bold;");
    }

    // Update GPS status styling
    if (data.gps == "No Fix" || data.battery == 0) {
        gpsStatus->setText("GPS: LOST");
        gpsStatus->setStyleSheet("background-color: #e74c3c; color: white; padding: 5px; border-radius: 3px;");
    } else {
        gpsStatus->setText("GPS: " + data.gps);
        gpsStatus->setStyleSheet("background-color: #27ae60; color: white; padding: 5px; border-radius: 3px;");
    }
}

void TelemetryData::toggleFailure()
{
    failureMode = !failureMode;
    droneSimulator->setFailureMode(failureMode);

    if (failureMode)
    {
        failureButton->setText("⚠ FAILURE ACTIVE");
        failureButton->setStyleSheet(
            "QPushButton { background-color: #c0392b; color: white; font-weight: bold; padding: 10px; border-radius: 5px; }"
        );
    }
    else
    {
        failureButton->setText("⚠ SIMULATE FAILURE");
        failureButton->setStyleSheet(
            "QPushButton { background-color: #e74c3c; color: white; font-weight: bold; padding: 10px; border-radius: 5px; }"
        );
    }
}

void TelemetryData::changeStrategy(int index)
{
    QStringList strategies = {"Hover", "Random Walk", "Circular"};
    QString strategy = strategies[index];

    Logger::instance()->log("Changing to: " + strategy);

    NormalDroneSimulator* drone = qobject_cast<NormalDroneSimulator*>(droneSimulator);
    if (drone) {
        drone->setStrategy(strategy);
    }
}

TelemetryData::~TelemetryData()
{
    Logger::instance()->log("TelemetryData closed");
}
