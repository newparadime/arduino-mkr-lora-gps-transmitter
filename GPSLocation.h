#pragma once
#include <memory>

class GPSLocation {
  public:
    class Float32{
      public:
        #pragma pack(push, 1)
        struct NetworkView {
          int32_t digits;
          int16_t exponent;
          int8_t negative;
        };
        #pragma pack(pop)
        
        Float32() : n(0) {}
        Float32(const float& n) : n(n) {}
        Float32(const Float32& n) : n(n.n) {}
        Float32(const Float32::NetworkView& view) {
          Unpack(view);
        }

        Float32& operator = (const Float32& rhs) { n = rhs.n; return *this; }

        Float32& operator = (const float& rhs) { n = rhs; return *this; }

        operator float() const { return n; }

        Float32::NetworkView Pack();

        Float32& Unpack(Float32::NetworkView view);

      private:
        float n;
    };

    class Int32{
      public:
        #pragma pack(push, 1)
        struct NetworkView {
          int32_t n;
        };
        #pragma pack(pop)

        Int32() : n(0) {}
        Int32(const int32_t& n) : n(n) {}
        Int32(const Int32& n) : n(n.n) {}
        Int32(const Int32::NetworkView& view) {
          Unpack(view);
        }

        Int32& operator = (const Int32& rhs) { n = rhs.n; return *this; }

        Int32& operator = (const int32_t& rhs) { n = rhs; return *this; }

        operator int32_t() const { return n; }

        Int32::NetworkView Pack();

        Int32& Unpack(Int32::NetworkView view);

      private:
        int32_t n;
    };

    #pragma pack(push, 1)
    struct NetworkView {
      Float32::NetworkView latitude;
      Float32::NetworkView longitude;
      Float32::NetworkView altitude;
      Int32::NetworkView  numSatellites;
    };
    #pragma pack(pop)

    GPSLocation::NetworkView Pack();

    GPSLocation& Unpack(GPSLocation::NetworkView view);

    Float32 latitude;
    Float32 longitude;
    Float32 altitude;
    Int32   numSatellites;
};
