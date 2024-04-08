#include "GPSLocation.h"

#include "EndianTranslation.h"

GPSLocation::Float32::NetworkView GPSLocation::Float32::Pack() {
  float nLocal = n;
  GPSLocation::Float32::NetworkView view = { 0, 0 };
  bool negative = false;
  
  if(n < 0) {
    negative = true;
    nLocal *= -1;
  }

  while(nLocal >= 10) {
    nLocal /= 10;
    view.E++;
  }
  
  while(nLocal < 1) {
    nLocal *= 10;
    view.E--;
  }
  
  if(negative) {
    nLocal *= -1;
  }
  
  view.n = htonl(static_cast<int32_t>(nLocal));
  view.E = htonl(view.E);

  return view;
}

GPSLocation::Float32& GPSLocation::Float32::Unpack(GPSLocation::Float32::NetworkView view) {
  n = ntohl(view.n);
  int32_t eLocal = ntohl(view.E);

  while (eLocal > 0) {
    n *= 10;
    eLocal--;
  }

  while (eLocal < 0) {
    n /= 10;
    eLocal++;
  }
  
  return *this;
}

GPSLocation::Int32::NetworkView GPSLocation::Int32::Pack() {
  GPSLocation::Int32::NetworkView view;
  
  view.n = htonl(n);
  
  return view;
}

GPSLocation::Int32& GPSLocation::Int32::Unpack(GPSLocation::Int32::NetworkView view) {
  n = ntohl(view.n);

  return *this;
}

GPSLocation::NetworkView GPSLocation::Pack() {
  GPSLocation::NetworkView view;
  
  view.latitude = latitude.Pack();
  view.longitude = longitude.Pack();
  view.altitude = altitude.Pack();
  view.numSatellites = numSatellites.Pack();
  
  return view;
}

GPSLocation& GPSLocation::Unpack(GPSLocation::NetworkView view) {
  latitude.Unpack(view.latitude);
  longitude.Unpack(view.longitude);
  altitude.Unpack(view.altitude);
  numSatellites.Unpack(view.numSatellites);
  
  return *this;
}
