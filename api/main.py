from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
import json

app = FastAPI()

app.add_middleware(CORSMiddleware,
                   allow_origins=["http://localhost:5173"],
                   allow_credentials=True,
                   allow_methods=["*"],
                   allow_headers=["*"]
                   )

@app.get("/")
def root():
  return {"message": "Home route, hello!"}

@app.get("/telemetry")
def get_telemetry():
    with open("../backend/telemetry.json", "r") as file:
      return json.load(file)
    
    
    
    
    # // test data 
    # "drones" : [
    #   { 
    #    "id": 1, 
    #    "position": {
    #      "lng": 10.0, 
    #      "lat": 20.0, 
    #      "alt": 0.0
    #     },
    #    "battery" : 92,
    #    "state": "IDLE"
    #    },
    #   { 
    #    "id": 2, 
    #    "position": {
    #      "lng": 20.0, 
    #      "lat": 30.0, 
    #      "alt": 70.0
    #     },
    #    "battery" : 67,
    #    "state": "DELIVERING"
    #   }
    # ]
