#include <cstdio>
#include <cstdint>
#include "../../../../src/include/protocol_sensors.h"

int main() {
    int c;
    int s = 0;
    char buf[1000];

    // Read binary data from stdin
    while((c = getchar()) != EOF) {
        buf[s++] = c;
    }
    printf("DBG SIZE=%d\n", s);

    // Cast to datastructure and print
    SensorDataGps* data = (SensorDataGps*) buf;

    printf("GPS "
           "LAT=%.6f,LON=%.6f,ALT=%.4f,"
           "TIME=%u,SPEED=%.4f,TRUETRK=%.4f"
           "\r\n",
           data->gps.lat, data->gps.lon, data->gps.altitude,
           data->gps.timestamp, data->gps.groundSpeed, data->gps.trueTrack);


    return 0;
}
