# Drone Fleet Telemetry System

## Architecture Overview

### /sim (C++ Telemetry Simulator)
- Simulates 100 autonomous drones with battery-constrained delivery behavior
- State machine: takeoff → cruise → delivery → return → landing → recharge
- Generates telemetry at 2-second intervals (position, battery %, state, destination)
- Publishes via HTTP POST to backend API

### /backend (FastAPI)
- **Ingestion**: REST endpoint receives telemetry from 100 drones
- **Processing**: Updates drone state in-memory (Python dict, ~50 updates/sec)
- **Broadcasting**: Serves current state via REST API to dashboard
- Async I/O with uvicorn for concurrent request handling

### /dashboard (React + TypeScript + Leaflet)
- Polls backend every 2 seconds for latest telemetry
- Renders 100 drone markers on interactive map
- Displays real-time: position, battery level, operational state, route
- Color-coded by state (green=active, yellow=low battery, red=returning)

## Data Flow
1. C++ sim → POST /telemetry → FastAPI backend
2. Backend updates in-memory state
3. Dashboard → GET /drones → Backend → JSON response
4. React re-renders map with updated positions

## Scale Characteristics
- 100 concurrent drones
- ~50 telemetry updates/second
- <100ms latency from sim to dashboard update
