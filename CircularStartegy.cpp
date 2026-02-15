#include "CircularStrategy.h"
#include "Logger.h"
#include <cmath>

CircularStrategy::CircularStrategy(QObject* parent)
    : MovementStrategy(parent)
    , m_angle(0)
{
    Logger::instance()->log("CircularStrategy created");
}

void CircularStrategy::updatePosition(TelemetryPacket& packet)
{
    static double t = 0;
    t += 0.02;

    static double centerLat = packet.lat;
    static double centerLon = packet.lon;
    static double radius = 0.005;

    static bool firstRun = true;
    if (firstRun) {
        centerLat = packet.lat;
        centerLon = packet.lon;
        firstRun = false;
    }

    m_angle += 0.03;

    packet.lat = centerLat + radius * cos(m_angle);
    packet.lon = centerLon + radius * sin(m_angle);

    packet.alt = 120 + 15 * sin(m_angle * 3);

    packet.heading = (int)((m_angle + M_PI/2) * 180 / M_PI) % 360;

    packet.speed = 10.0 + 3.0 * sin(m_angle * 2);
}
