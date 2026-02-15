#include "DroneFactory.h"
#include "Logger.h"
IDroneSimulator* DroneFactory::createDrone(DroneType type, QObject* parent)
{
    Logger::instance()->log("Factory creating drone...");
    switch(type) {
        case NormalDrone:
            return new NormalDroneSimulator(parent);
        default:
            return new NormalDroneSimulator(parent);
    }
}
