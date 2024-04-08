#include <LoRa.h>

#include <Arduino_MKRGPS.h>

#include "GPSLocation.h"

#include <memory>

void setup() {
  Serial.begin(9600);

  while(!Serial);

  Serial.println("LoRa GPS transmitter");

  uint32_t LoRaFreq = 915E6;

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

  if(!GPS.begin()) {
    Serial.println("Failed to initialize GPS!");
    while(true);
  }
  else {
    Serial.println("Initialized GPS");
    Serial.println("Placing GPS module in standby mode...");
    GPS.standby();
    Serial.println("GPS asleep");
    Serial.println();
  }

}

GPSLocation GetLocationStandby() {
  Serial.println("  Wake GPS module from standby...");
  GPS.wakeup();
  while (!GPS.available());
  Serial.println("  GPS awoken");
  
  GPSLocation location;
  Serial.println("  Retrieve GPS data...");
  location.latitude      = GPS.latitude();
  location.longitude     = GPS.longitude();
  location.altitude      = GPS.altitude();
  location.numSatellites = GPS.satellites();
  Serial.println("  GPS data retrieved");

  Serial.println("  Placing GPS module back in standby mode...");
  GPS.standby();
  Serial.println("  GPS asleep");
}

int send_location_lora(GPSLocation location) {

}

void loop() {
  GPSLocation location = GetLocationStandby();

  // print GPS values
  Serial.println();
  Serial.print("Location: ");
  Serial.print(location.latitude, 7);
  Serial.print("°, ");
  Serial.print(location.longitude, 7);
  Serial.println("°");

  Serial.print("Altitude: ");
  Serial.print(location.altitude);
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
