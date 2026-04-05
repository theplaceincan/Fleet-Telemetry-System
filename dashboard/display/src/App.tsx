import './App.css'
import { useEffect, useState } from "react";
import { MapContainer, TileLayer, CircleMarker, Popup } from "react-leaflet";

type Location = { lat: number; lng: number; address: string; };
type Drone = {
  id: number;
  position: { lng: number; lat: number; alt: number; };
  battery: number;
  state: string;
  base: Location;
  destination: Location;
  timestamp: number;
};

const STATE_COLORS: Record<string, string> = {
  TAKEOFF:   "#ffffff",
  CRUISE:    "#e0e0e0",
  DELIVERY:  "#cc0000",
  APPROACH:  "#ff4444",
  RETURNING: "#888888",
  LANDED:    "#555555",
  CHARGING:  "#ff6600",
  OFF:       "#333333",
};

const stateColor = (s: string) => STATE_COLORS[s] ?? "#fff";

const S = {
  root: {
    display: "flex", flexDirection: "column" as const,
    height: "100vh", width: "100vw",
    background: "#000", color: "#fff",
    fontFamily: "'Courier New', monospace"
  },
  header: {
    display: "flex", alignItems: "center", justifyContent: "space-between",
    padding: "0 24px", height: 48,
    borderBottom: "1px solid #222", background: "#000", flexShrink: 0
  },
  logo: {
    fontSize: 13, fontWeight: 700, letterSpacing: 6,
    color: "#fff", textTransform: "uppercase" as const
  },
  sub: {
    fontSize: 10, color: "#888", letterSpacing: 3,
    textTransform: "uppercase" as const
  },
  statsBar: { display: "flex", borderBottom: "1px solid #222", flexShrink: 0 },
  statCell: {
    flex: 1, padding: "10px 0",
    textAlign: "center" as const, borderRight: "1px solid #222"
  },
  statVal: { fontSize: 20, fontWeight: 700, letterSpacing: 2 },
  statLabel: {
    fontSize: 9, color: "#888", letterSpacing: 3,
    marginTop: 2, textTransform: "uppercase" as const
  },
  filterBar: {
    display: "flex", alignItems: "center",
    padding: "0 16px", height: 36,
    borderBottom: "1px solid #222", gap: 2,
    flexShrink: 0, background: "#000"
  },
  filterBtn: (active: boolean, state: string) => ({
    padding: "4px 14px",
    border: active ? `1px solid ${stateColor(state)}` : "1px solid #333",
    background: active ? stateColor(state) : "transparent",
    color: active ? "#000" : "#aaa",
    fontSize: 9, letterSpacing: 2, cursor: "pointer",
    textTransform: "uppercase" as const,
    fontFamily: "'Courier New', monospace", fontWeight: 700
  }),
  mapWrap: { flex: 1, position: "relative" as const },
  legend: {
    display: "flex", gap: 20, padding: "8px 24px",
    borderTop: "1px solid #222", background: "#000", flexShrink: 0
  },
  legendItem: { display: "flex", alignItems: "center", gap: 6 },
  dot: (color: string) => ({
    width: 8, height: 8, borderRadius: "50%", background: color
  }),
  legendLabel: {
    fontSize: 9, color: "#888", letterSpacing: 2,
    textTransform: "uppercase" as const
  },
};

export default function App() {
  const [drones, setDrones] = useState<Drone[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState("");
  const [filter, setFilter] = useState("ALL");
  const [tick, setTick] = useState(0);

  useEffect(() => {
    const fetchData = () => {
      fetch("http://127.0.0.1:8000/telemetry")
        .then(r => { if (!r.ok) throw new Error(`${r.status}`); return r.json(); })
        .then(data => {
          setDrones(data.drones);
          setLoading(false);
          setError("");
          setTick(t => t + 1);
        })
        .catch(() => {
          setError("SIGNAL LOST");
          setLoading(false);
        });
    };
    fetchData();
    const iv = setInterval(fetchData, 2000);
    return () => clearInterval(iv);
  }, []);

  const counts = drones.reduce((a, d) => {
    a[d.state] = (a[d.state] ?? 0) + 1;
    return a;
  }, {} as Record<string, number>);

  const lowBat = drones.filter(d => d.battery <= 25 && d.state !== "CHARGING").length;
  const filtered = filter === "ALL" ? drones : drones.filter(d => d.state === filter);

  if (loading) return (
    <div style={{ ...S.root, alignItems: "center", justifyContent: "center" }}>
      <div style={{ fontSize: 11, letterSpacing: 6, color: "#666" }}>
        INITIALIZING FLEET NETWORK...
      </div>
    </div>
  );

  return (
    <div style={S.root}>

      {/* Header */}
      <div style={S.header}>
        <div style={{ display: "flex", alignItems: "center", gap: 20 }}>
          <span style={S.logo}>FLEETOPS</span>
          <span style={S.sub}>Las Vegas Autonomous Network</span>
        </div>
        <div style={{ display: "flex", gap: 24, alignItems: "center" }}>
          {error && (
            <span style={{ fontSize: 9, color: "#cc0000", letterSpacing: 3 }}>
              ⚠ {error}
            </span>
          )}
          {lowBat > 0 && (
            <span style={{ fontSize: 9, color: "#ff6600", letterSpacing: 3 }}>
              ⚠ {lowBat} LOW BATTERY
            </span>
          )}
          <span style={{ fontSize: 9, color: "#888", letterSpacing: 2 }}>
            TICK {tick.toString().padStart(4, "0")} · {new Date().toLocaleTimeString()}
          </span>
        </div>
      </div>

      {/* Stats */}
      <div style={S.statsBar}>
        {[
          { label: "Fleet",     val: drones.length,              color: "#fff"    },
          { label: "Cruise",    val: counts["CRUISE"] ?? 0,      color: "#e0e0e0" },
          { label: "Delivery",  val: counts["DELIVERY"] ?? 0,    color: "#cc0000" },
          { label: "Returning", val: counts["RETURNING"] ?? 0,   color: "#888"    },
          { label: "Charging",  val: counts["CHARGING"] ?? 0,    color: "#ff6600" },
          { label: "Takeoff",   val: counts["TAKEOFF"] ?? 0,     color: "#fff"    },
          { label: "Landed",    val: counts["LANDED"] ?? 0,      color: "#666"    },
        ].map(({ label, val, color }) => (
          <div key={label} style={S.statCell}>
            <div style={{ ...S.statVal, color }}>{val}</div>
            <div style={S.statLabel}>{label}</div>
          </div>
        ))}
      </div>

      {/* Filter */}
      <div style={S.filterBar}>
        {["ALL", "CRUISE", "DELIVERY", "RETURNING", "CHARGING", "TAKEOFF", "LANDED"].map(s => (
          <button key={s} style={S.filterBtn(filter === s, s)} onClick={() => setFilter(s)}>
            {s}
          </button>
        ))}
        <span style={{ marginLeft: "auto", fontSize: 9, color: "#888", letterSpacing: 2 }}>
          {filtered.length} UNITS
        </span>
      </div>

      {/* Map */}
      <div style={S.mapWrap}>
        <MapContainer
          center={[36.1699, -115.1398] as [number, number]}
          zoom={13}
          style={{ height: "100%", width: "100%" }}
          zoomControl={false}
        >
          <TileLayer
            url="https://{s}.basemaps.cartocdn.com/dark_all/{z}/{x}/{y}{r}.png"
            attribution="CartoDB"
          />
          {filtered.map(drone => (
            <CircleMarker
              key={drone.id}
              center={[drone.position.lat, drone.position.lng] as [number, number]}
              radius={drone.state === "DELIVERY" ? 6 : 4}
              color={stateColor(drone.state)}
              fillColor={stateColor(drone.state)}
              fillOpacity={drone.state === "CHARGING" || drone.state === "LANDED" ? 0.4 : 0.9}
              weight={1}
            >
              <Popup>
                <div style={{
                  fontFamily: "'Courier New', monospace", fontSize: 11,
                  background: "#000", color: "#fff", padding: 12,
                  minWidth: 220, border: "1px solid #222"
                }}>
                  <div style={{
                    fontSize: 13, fontWeight: 700, letterSpacing: 3,
                    marginBottom: 8, borderBottom: "1px solid #222", paddingBottom: 6
                  }}>
                    DRONE #{drone.id}
                  </div>
                  <div style={{ display: "grid", gridTemplateColumns: "auto 1fr", gap: "4px 12px" }}>
                    <span style={{ color: "#888" }}>STATE</span>
                    <span style={{ color: stateColor(drone.state), letterSpacing: 2 }}>{drone.state}</span>

                    <span style={{ color: "#888" }}>BATTERY</span>
                    <span style={{ color: drone.battery <= 25 ? "#cc0000" : "#fff" }}>{drone.battery}%</span>

                    <span style={{ color: "#888" }}>ALT</span>
                    <span>{drone.position.alt.toFixed(1)}m</span>

                    <span style={{ color: "#888" }}>BASE</span>
                    <span style={{ color: "#aaa", fontSize: 10 }}>{drone.base.address}</span>

                    <span style={{ color: "#888" }}>DEST</span>
                    <span style={{ color: "#aaa", fontSize: 10 }}>{drone.destination.address}</span>

                    <span style={{ color: "#888" }}>SYNC</span>
                    <span style={{ color: "#666" }}>{new Date(drone.timestamp).toLocaleTimeString()}</span>
                  </div>
                </div>
              </Popup>
            </CircleMarker>
          ))}
        </MapContainer>
      </div>

      {/* Legend */}
      <div style={S.legend}>
        {Object.entries(STATE_COLORS).map(([state, color]) => (
          <div key={state} style={S.legendItem}>
            <div style={S.dot(color)} />
            <span style={S.legendLabel}>{state}</span>
          </div>
        ))}
      </div>

    </div>
  );
}