#include "HoverStrategy.h"
#include "Logger.h"
#include <cmath>

HoverStrategy::HoverStrategy(QObject* parent)
    : MovementStrategy(parent)
{
    Logger::instance()->log("HoverStrategy created");
}

void HoverStrategy::updatePosition(TelemetryPacket& packet)
{
    static double t = 0;
    t += 0.05;

    static double hoverLat = packet.lat;
    static double hoverLon = packet.lon;
    static double hoverAlt = packet.alt;

    static bool firstRun = true;
    if (firstRun) {
        hoverLat = packet.lat;
        hoverLon = packet.lon;
        hoverAlt = packet.alt;
        firstRun = false;
    }

    packet.lat = hoverLat + 0.00002 * sin(t);
    packet.lon = hoverLon + 0.00002 * cos(t * 1.3);
    packet.alt = hoverAlt + 0.5 * sin(t * 2);

    packet.heading = (packet.heading + 2) % 360;
    packet.speed = 0.2;
}
