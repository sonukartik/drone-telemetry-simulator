#include "NormalDroneSimulator.h"
#include "Logger.h"
#include "HoverStrategy.h"
#include "RandomWalkStrategy.h"
#include "CircularStrategy.h"

NormalDroneSimulator::NormalDroneSimulator(QObject* parent)
    : IDroneSimulator(parent)
    , m_strategy(nullptr)  // Initialize to nullptr
{
    Logger::instance()->log("NormalDroneSimulator created");

    workerThread = new QThread(this);
    worker = new TelemetryWorker();

    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started,
            worker, &TelemetryWorker::start);

    // Set default strategy BEFORE connecting the signal
    setStrategy("Hover");

    connect(worker, &TelemetryWorker::telemetryUpdated,
            this, [this](TelemetryPacket packet) {
                if (m_strategy && packet.battery > 0) {  // This will now be true
                    m_strategy->updatePosition(packet);
                }
                emit telemetryUpdated(packet);
            });

    connect(workerThread, &QThread::finished,
            worker, &QObject::deleteLater);
}

NormalDroneSimulator::~NormalDroneSimulator()
{
    Logger::instance()->log("NormalDroneSimulator destroyed");

    // Stop thread first
    workerThread->quit();
    workerThread->wait();

    // Then delete strategy
    if (m_strategy) {
        delete m_strategy;
        m_strategy = nullptr;
    }
}

void NormalDroneSimulator::start()
{
    Logger::instance()->log("NormalDroneSimulator started");
    workerThread->start();
}

void NormalDroneSimulator::setFailureMode(bool enabled)
{
    QMetaObject::invokeMethod(worker, [this, enabled]() {
        worker->setFailureMode(enabled);
    });
}

void NormalDroneSimulator::setStrategy(const QString& strategyName)
{
    Logger::instance()->log("Setting strategy to: " + strategyName);

    if (m_strategy) {
        delete m_strategy;
        m_strategy = nullptr;
    }

    if (strategyName == "Hover") {
        m_strategy = new HoverStrategy();
    } else if (strategyName == "Random Walk") {
        m_strategy = new RandomWalkStrategy();
    } else if (strategyName == "Circular") {
        m_strategy = new CircularStrategy();
    } else {
        Logger::instance()->error("Unknown strategy, using Hover");
        m_strategy = new HoverStrategy();
    }

    Logger::instance()->log("Strategy changed to: " + strategyName);
}
