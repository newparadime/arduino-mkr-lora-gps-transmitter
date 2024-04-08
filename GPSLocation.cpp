#include <cmath>

#include "GPSLocation.h"

#include "EndianTranslation.h"

GPSLocation::Float32::NetworkView GPSLocation::Float32::Pack() {
  GPSLocation::Float32::NetworkView view = { 0, 0, 0 };
  view.negative = n < 0 ? -1 : 1;
  float nLocal = n * view.negative;  
  
  static constexpr float UINT32_MAX_LOG10 = log10(UINT32_MAX);
  view.exponent = ceil(log10(nLocal) - UINT32_MAX_LOG10);

  nLocal *= pow(10.0, -view.exponent);
  
  view.digits = htonl(static_cast<uint32_t>(nLocal));
  view.exponent = htons(view.exponent);

  return view;
}

GPSLocation::Float32& GPSLocation::Float32::Unpack(GPSLocation::Float32::NetworkView view) {
  n = ntohl(view.digits);
  n *= view.negative;
  
  view.exponent = ntohs(view.exponent);

  n *= pow(10.0, view.exponent);
  
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
