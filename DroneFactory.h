#ifndef DRONEFACTORY_H
#define DRONEFACTORY_H

#include "IDroneSimulator.h"
#include "NormalDroneSimulator.h"

class DroneFactory
{
public:
    enum DroneType {
        NormalDrone,
        // For adding more Drones
    };

    static IDroneSimulator* createDrone(DroneType type = NormalDrone, QObject* parent = nullptr);
};

#endif // DRONEFACTORY_H
