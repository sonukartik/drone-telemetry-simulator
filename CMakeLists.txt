cmake_minimum_required(VERSION 3.16)
project(DroneTelemetry)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# List all source files - NOTE the closing parenthesis!
set(SOURCES
    main.cpp
    TelemetryData.cpp
    DroneFactory.cpp
    NormalDroneSimulator.cpp
    TelemetryWorker.cpp
    Logger.cpp
    HoverStrategy.cpp
    RandomWalkStrategy.cpp
    CircularStrategy.cpp
)  # <-- THIS WAS MISSING!

# List all header files - NOTE the closing parenthesis!
set(HEADERS
    TelemetryData.h
    IDroneSimulator.h
    DroneFactory.h
    NormalDroneSimulator.h
    TelemetryWorker.h
    TelemetryPacket.h
    Logger.h
    MovementStrategy.h
    HoverStrategy.h
    RandomWalkStrategy.h
    CircularStrategy.h
)  # <-- THIS WAS MISSING!

add_executable(DroneTelemetry ${SOURCES} ${HEADERS})
target_link_libraries(DroneTelemetry Qt5::Core Qt5::Widgets)
