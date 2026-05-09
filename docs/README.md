# Drone Fleet Telemetry System

A full-stack drone fleet telemetry prototype built with C++, FastAPI, React, TypeScript, and Leaflet.

This project simulates 100 autonomous delivery drones, generates telemetry snapshots, serves that telemetry through a FastAPI backend, and visualizes the fleet on an interactive live dashboard.

## Overview

The system is split into three parts:

### C++ Simulation Engine

The simulator models a fleet of autonomous drones with battery-constrained delivery behavior.

Each drone tracks:

- Position
- Altitude
- Battery level
- Current state
- Base location
- Delivery destination

The drone state machine includes:

```txt
OFF → TAKEOFF → CRUISE → DELIVERY → RETURNING → LANDED → CHARGING
