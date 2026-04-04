import './App.css'

import { useEffect, useState } from "react";

type Drone = {
  id: number;
  position: {
    lng: number;
    lat: number;
    alt: number;
  };
  battery: number;
  state: string;
};

export default function App() {
  const [drones, setDrones] = useState<Drone[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState("");

  useEffect(() => {
    const interval = setInterval(() => {
      fetch("http://127.0.0.1:8000/telemetry")
      .then((response) => response.json())
      .then((data) => {
        setDrones(data.drones);
        setLoading(false);
      })
      .catch((err) => {
        console.error("Failed to fetch telemetry:", err);
        setError("Failed to fetch telemetry");
        setLoading(false);
      });
    }, 1000)
    
    return () => clearInterval(interval);
  }, []);

  if (loading) { return <p>Loading telemetry...</p>; }
  if (error) { return <p>{error}</p>; }

  return (
    <div>
      <h1>Fleet Telemetry Dashboard</h1>

      {drones.map((drone) => (
        <div key={drone.id}>
          <h2>Drone #{drone.id}</h2>
          <p>Longitude: {drone.position.lng}</p>
          <p>Latitude: {drone.position.lat}</p>
          <p>Altitude: {drone.position.alt}</p>
          <p>Battery: {drone.battery}%</p>
          <p>State: {drone.state}</p>
        </div>
      ))}
    </div>
  );
}