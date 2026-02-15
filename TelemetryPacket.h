#ifndef TELEMETRYPACKET_H
#define TELEMETRYPACKET_H

#include <QString>
#include <QMetaType>

struct TelemetryPacket
{
    double lat;
    double lon;
    double alt;
    int heading;
    double speed;
    int battery;
    QString gps;
};

Q_DECLARE_METATYPE(TelemetryPacket)

#endif // TELEMETRYPACKET_H#ifndef TELEMETRYPACKET_H

