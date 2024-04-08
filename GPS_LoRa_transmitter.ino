#include <memory>

#include <LoRa.h>
#include <TinyGPS++.h>

#include "GPSLocation.h"

static constexpr uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);

  while(!Serial);

  Serial.println("LoRa GPS transmitter");

  uint32_t LoRaFreq = 433E6;

  if(!LoRa.begin(LoRaFreq)) {
    Serial.print("Failed to initialize LoRa @ ");
    Serial.print(LoRaFreq);
    Serial.println("Hz!");
    while(true);
  }
  else {
    Serial.print("Initialized LoRa @ ");
    Serial.print(LoRaFreq);
    Serial.println("Hz");
  }

  Serial1.begin(GPSBaud);
}

GPSLocation GetLocation() {
  bool locationUpdated = false;
  bool altitudeUpdated = false;
  bool satellitesUpdated = false;

  GPSLocation location;
  while ( Serial1.available() > 0 
           && ! (    locationUpdated
                  && altitudeUpdated
                  && satellitesUpdated ) ) {
    gps.encode(Serial1.read());
    if (gps.location.isUpdated()) {
      location.latitude  = gps.location.lat();
      location.longitude = gps.location.lng();
      locationUpdated = true;
      Serial.println("  location updated");
    }
    if (gps.altitude.isUpdated()) {
      location.altitude = gps.altitude.meters();
      altitudeUpdated = true;
      Serial.println("  altitude updated");
    }
    if (gps.satellites.isUpdated()) {
      location.numSatellites = gps.satellites.value();
      satellitesUpdated = true;
      Serial.println("  satellites updated");
    }    
  }
 
  return location;
}

void loop() {
  GPSLocation location = GetLocation();

  // print GPS values
  Serial.println();
  Serial.print("Location: ");
  Serial.print(location.latitude, 7);
  Serial.print("°, ");
  Serial.print(location.longitude, 7);
  Serial.println("°");

  Serial.print("Altitude (meters): ");
  Serial.print(location.altitude, 7);
  Serial.println("m");

  Serial.print("Number of satellites: ");
  Serial.println(location.numSatellites);
  Serial.println();

  Serial.println("Transmitting GPS data via LoRa...");
  Serial.println();

  GPSLocation::NetworkView view = location.Pack();
  LoRa.beginPacket(true);
  LoRa.write((uint8_t*)&view, sizeof(view));
  LoRa.endPacket();

  Serial.println("Data transmitted");

  // wait for 10 seconds
  constexpr int delaySeconds = 15;
  Serial.print("delay ");
  Serial.print(delaySeconds);
  Serial.println(" seconds");
  Serial.println();
  delay(delaySeconds * 1000);
}
