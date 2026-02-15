# Drone Telemetry Simulation System 🚁

A Qt-based drone telemetry simulation system demonstrating design patterns including Factory, Strategy, and Singleton patterns.

## Features

- **Real-time Telemetry Display**: Live updates of drone position, altitude, heading, speed, battery, and GPS status
- **Multiple Flight Patterns**: Switch between different movement behaviors using Strategy pattern
  - 🔄 Hover - Stable position keeping
  - 🎲 Random Walk - Exploration behavior
  - ⭕ Circular - Orbital flight pattern
- **Failure Simulation**: Toggle failure mode to simulate GPS loss and accelerated battery drain
- **Thread-safe Architecture**: Worker thread for telemetry generation
- **Centralized Logging**: Singleton logger for system events
- **Clean UI**: Professional dashboard with progress bars and status indicators

## Design Patterns Used

- **Factory Pattern**: `DroneFactory` creates different drone simulator instances
- **Strategy Pattern**: `MovementStrategy` hierarchy for interchangeable flight behaviors
- **Singleton Pattern**: `Logger` for centralized logging
- **Observer Pattern**: Signals/slots for UI updates

## Requirements

- Qt 5.12 or higher
- C++11 or higher
- MinGW or MSVC compiler

## Building the Project

### Clone the repository:

```bash 
git clone https://github.com/sonukartik/drone-telemetry-simulator.git
cd drone-telemetry-simulator
```

### Build with CMake

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt5
make -j4
./DroneTelemetry
```

### Usage
Select Flight Pattern: Use dropdown to choose between Hover, Random Walk, or Circular

Simulate Failure: Toggle failure mode to see GPS loss and faster battery drain

Monitor Telemetry: Watch real-time updates of all drone parameters

Battery Warning: Visual indicators when battery is low (20%) or depleted (0%)


### Project Structure

drone-telemetry-simulator/
├── 📄 drone.pro                  # qmake project file
├── 📄 CMakeLists.txt             # CMake build file
├── 📄 README.md                  # This file
├── 📄 main.cpp                   # Application entry
├── 📄 IDroneSimulator.h          # Drone interface
├── 📄 DroneFactory.h/cpp         # Factory pattern
├── 📄 NormalDroneSimulator.h/cpp # Concrete drone
├── 📄 TelemetryWorker.h/cpp      # Worker thread
├── 📄 MovementStrategy.h          # Strategy interface
├── 📄 HoverStrategy.h/cpp         # Hover behavior
├── 📄 RandomWalkStrategy.h/cpp    # Random walk
├── 📄 CircularStrategy.h/cpp      # Circular behavior
├── 📄 Logger.h/cpp                # Singleton logger
└── 📄 TelemetryData.h/cpp         # UI widget  
