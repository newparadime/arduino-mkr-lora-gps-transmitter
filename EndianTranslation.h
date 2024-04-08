#pragma once

// The host order of the Arduino platform is little endian.
// Sometimes it is desired to convert to big endian (or
// network order)

#ifndef htons
// Host to Network short
#define htons(x) ( (((x)&0xFF)<<8) | (((x)>>8)&0xFF) )

// Network to Host short
#define ntohs(x) htons(x)
#endif // !defined(htons)

#ifndef htonl
// Host to Network long
#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )

// Network to Host long
#define ntohl(x) htonl(x)

#endif // !defined(htonl)