# System Design

## Overview

### /sim
- Sends fake data simulating drones

### /backend
- Receives data from `/sim`
- Broadcasts data to `/dashboard`
- Updates state of drones (loop)

### /dashboard
- React/TS app