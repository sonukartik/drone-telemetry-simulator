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

**Factory Pattern** - `DroneFactory`:
Creates different drone simulator instances without exposing creation logic to the client. Makes it easy to add new drone types in the future.
```cpp
// Client doesn't need to know concrete classes
IDroneSimulator* drone = DroneFactory::createDrone(NormalDrone);
```

**Strategy Pattern** - MovementStrategy Hierarchy : 
Encapsulates different movement algorithms so they can be interchanged at runtime. Users can switch behaviors dynamically.
```cpp
// Strategies can be changed on the fly
drone->setStrategy(new HoverStrategy());      // Stable hover
drone->setStrategy(new RandomWalkStrategy()); // Random exploration
drone->setStrategy(new CircularStrategy());   // Orbital flight
```

**Singleton Pattern** - Logger
Ensures only one instance of the logger exists throughout the application, providing centralized logging from all components.
```cpp
// Access from anywhere in the code
Logger::instance()->log("Drone created");
Logger::instance()->error("Low battery!");
```

**Observer Pattern**: Signals/slots for UI updates
Built into Qt framework. Telemetry data is observed by UI components which update automatically when data changes.
```cpp
// Worker emits signal when data is ready
emit telemetryUpdated(packet);

// UI automatically updates when signal is received
connect(worker, &TelemetryWorker::telemetryUpdated, 
        ui, &TelemetryData::updateUI);
```

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
```
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
```
