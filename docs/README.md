# Drone Fleet Telemetry System

This is a full-stack drone fleet telemetry prototype built with C++, FastAPI, React, TypeScript, and Leaflet.

This project simulates a fleet of 100 autonomous delivery drones, generates live telemetry snapshots, serves the data through a FastAPI backend, and visualizes the fleet on an interactive real-time dashboard.

## Architecture Overview

The system is divided into three main components:

### `/sim` — C++ Telemetry Simulator

The simulator models 100 autonomous drones with battery-constrained delivery behavior.

Each drone tracks:

- Position
- Altitude
- Battery percentage
- Operational state
- Base location
- Delivery destination

The drone state machine follows a delivery lifecycle:

```txt
OFF → TAKEOFF → CRUISE → DELIVERY → RETURNING → LANDED → CHARGING
```

The simulator generates telemetry every 2 seconds, including position, battery level, state, altitude, and destination data. Each telemetry snapshot is sent to the backend using HTTP POST requests.

### `/backend` — FastAPI Telemetry Server

The backend receives, processes, and serves live drone telemetry.

Core responsibilities:

- Receives telemetry from the C++ simulator through a REST ingestion endpoint
- Maintains the latest state of each drone in memory using a Python dictionary
- Handles approximately 50 telemetry updates per second
- Serves the current fleet state through a REST API
- Uses FastAPI with async I/O and uvicorn for concurrent request handling

### `/dashboard` — React + TypeScript + Leaflet Dashboard

The dashboard visualizes the fleet in real time on an interactive map.

Features:

- Polls the backend every 2 seconds for the latest drone telemetry
- Renders 100 live drone markers using Leaflet
- Displays each drone’s position, battery level, operational state, altitude, and route
- Uses color-coded markers to show fleet status:
  - Green: active
  - Yellow: low battery
  - Red: returning or critical state

## Data Flow

```txt
C++ Simulator
      ↓
POST /telemetry
      ↓
FastAPI Backend
      ↓
GET /drones
      ↓
React Dashboard
      ↓
Live Fleet Map
```

1. The C++ simulator generates telemetry for 100 drones.
2. Each drone sends telemetry snapshots to the FastAPI backend.
3. The backend updates the latest drone state in memory.
4. The React dashboard polls the backend every 2 seconds.
5. Leaflet renders updated drone positions and states on the map.

## Scale Characteristics

- Simulates 100 concurrent autonomous drones
- Generates telemetry at 2-second intervals
- Processes approximately 50 telemetry updates per second
- Maintains latest drone state in memory for fast reads
- Achieves sub-100ms backend response latency from ingestion to dashboard availability
- Streams live fleet data to an interactive browser-based dashboard

## Tech Stack

- C++
- FastAPI
- Python
- React
- TypeScript
- Leaflet
- REST APIs
- uvicorn
