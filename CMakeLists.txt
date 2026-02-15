cmake_minimum_required(VERSION 3.16)
project(DroneTelemetry)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Test)

# ============ LIBRARY (shared code) ============
set(LIB_SOURCES
    DroneFactory.cpp
    NormalDroneSimulator.cpp
    TelemetryWorker.cpp
    Logger.cpp
    HoverStrategy.cpp
    RandomWalkStrategy.cpp
    CircularStrategy.cpp
)

set(LIB_HEADERS
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
)

# Create a STATIC library
add_library(DroneLib STATIC ${LIB_SOURCES} ${LIB_HEADERS})
target_link_libraries(DroneLib Qt5::Core Qt5::Widgets)
target_include_directories(DroneLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ============ MAIN APPLICATION ============
add_executable(DroneTelemetry 
    main.cpp 
    TelemetryData.cpp 
    TelemetryData.h
)

target_link_libraries(DroneTelemetry DroneLib Qt5::Core Qt5::Widgets)
target_include_directories(DroneTelemetry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ============ TESTS ============
enable_testing()

set_source_files_properties(SOURCE tests/test_drone_system.cpp PROPERTY SKIP_AUTOMOC OFF)

add_executable(DroneTests tests/test_drone_system.cpp)
target_link_libraries(DroneTests DroneLib Qt5::Core Qt5::Test)
target_include_directories(DroneTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME DroneSystemTest COMMAND DroneTests)
