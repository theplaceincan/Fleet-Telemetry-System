import './App.css'

import { useEffect, useState } from "react";
import { MapContainer, TileLayer, CircleMarker, Popup } from "react-leaflet";

type Location = {
  lat: number;
  lng: number;
  address: string;
};

type Drone = {
  id: number;
  position: {
    lng: number;
    lat: number;
    alt: number;
  };
  battery: number;
  state: string;
  base: Location;
  destination: Location;
  timestamp: number;
};

const stateColor = (state: string) => {
  switch (state) {
    case "CRUISE":
    case "TAKEOFF": return "blue";
    case "DELIVERY": return "green";
    case "APPROACH": return "orange";
    case "RETURNING": return "purple";
    case "OFF": return "gray";
    default: return "black";
  }
};

export default function App() {
  const [drones, setDrones] = useState<Drone[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState("");

  useEffect(() => {
    const fetchData = () => {
      const controller = new AbortController();
      fetch("http://127.0.0.1:8000/telemetry")
        .then((response) => {
          if (!response.ok) throw new Error(`HTTP ${response.status}`);
          return response.json();
        })
        .then((data) => {
          setDrones(data.drones);
          setLoading(false);
          setError("");
        })
        .catch((err) => {
          console.error("Failed to fetch telemetry:", err);
          setError("Failed to fetch telemetry");
          setLoading(false);
        });
      return controller;
    }

    fetchData();
    const interval = setInterval(fetchData, 1000)
    return () => clearInterval(interval);
  }, []);


  if (loading) { return <p>Loading telemetry...</p>; }
  // if (error) { return <p>{error}</p>; }

  return (
    <div>
      <h1>Fleet Telemetry Dashboard</h1>
      {error && <p style={{ color: "red" }}>{error}</p>}

      <MapContainer center={[36.1699, -115.1398] as [number, number]} zoom={13} style={{ height: "100vh", width: "100%" }}>
        <TileLayer url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png" />

        {drones.map((drone) => (
          <CircleMarker
            key={drone.id}
            center={[drone.position.lat, drone.position.lng] as [number, number]}
            radius={5}
            color={stateColor(drone.state)}
          >
            <Popup>
              <b>Drone #{drone.id}</b><br />
              Battery: {drone.battery}%<br />
              State: {drone.state}<br />
              Base: {drone.base.address}<br />
              Destination: {drone.destination.address}<br />
            </Popup>
          </CircleMarker>
        ))}
      </MapContainer>

      {/* <h1>Fleet Telemetry Dashboard</h1>
      {error && <p style={{ color: "red" }}>{error}</p>}
      {drones.map((drone) => (
        <div key={drone.id}>
          <h2>Drone #{drone.id}</h2>
          <p>Longitude: {drone.position.lng}</p>
          <p>Latitude: {drone.position.lat}</p>
          <p>Altitude: {drone.position.alt}</p>
          <p>Battery: {drone.battery}%</p>
          <p>State: {drone.state}</p>
          <p>Last Updated: {new Date(drone.timestamp).toLocaleTimeString()}</p>
        </div>
      ))} */}
    </div>
  );
}