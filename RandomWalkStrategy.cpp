#include "RandomWalkStrategy.h"
#include "Logger.h"
#include <cmath>

RandomWalkStrategy::RandomWalkStrategy(QObject* parent)
    : MovementStrategy(parent)
{
    Logger::instance()->log("RandomWalkStrategy created");
}

void RandomWalkStrategy::updatePosition(TelemetryPacket& packet)
{
        packet.lat += (QRandomGenerator::global()->generateDouble() - 0.5) * 0.01;
        packet.lon += (QRandomGenerator::global()->generateDouble() - 0.5) * 0.01;
        packet.speed = 8.0 + (QRandomGenerator::global()->generateDouble() * 0.5);
        packet.heading = (packet.heading + 2) % 360;
}
