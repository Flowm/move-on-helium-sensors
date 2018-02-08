#pragma once

// Define max temp sensors for temperatures array.
#define MAX_TEMP_SENSORS 8

// Generic
struct SensorVector {
    union {
        float v[3];
        struct {
            float x;
            float y;
            float z;
        };
    };
} __attribute__((packed));

struct SensorQuaternion{
    union {
        float q[4];
        struct {
            float w;
            float x;
            float y;
            float z;
        };
    };
} __attribute__((packed));

// Sensor data
struct SensorGPS {
    uint32_t time;
} __attribute__((packed));

struct SensorENV {
    float temperature;
    float humidity;
    float pressure;
    float gasresistance;
} __attribute__((packed));

struct SensorIMU {
    SensorVector accel;
    SensorVector gyro;
    SensorVector mag;
    SensorVector orientation;
    SensorQuaternion quaternion;
    int8_t temp_accel;
    int8_t temp_gyro;
    uint8_t resets_temps;
    uint8_t resets_zeroes;
} __attribute__((packed));

struct SensorTemp
{
    float temp;
//    char rom[8];
}__attribute__((packed));

struct SensorData {
    SensorGPS gps;
    SensorENV env;
    SensorIMU imu;
    SensorTemp temp[MAX_TEMP_SENSORS];
} __attribute__((packed));

// CDH protocol format
struct OutProtocolHeader {
    uint8_t opcode;
} __attribute__((packed));

struct InProtocolHeader {
    uint8_t start; // Value: 0x01
    uint8_t status;
    uint16_t numBytes; // Value: sizeof(SensorData)
} __attribute__((packed));

struct InProtocolFooter {
    uint16_t checksum;
    uint8_t end; // Value: 0x04
} __attribute__((packed));

struct CDHPacket {
    InProtocolHeader header;
    SensorData data;
    InProtocolFooter footer;
} __attribute__((packed));

#ifdef __cplusplus
#if __GNUG__ && __GNUC__ < 5
static_assert(__has_trivial_copy(CDHPacket), "CDHPacket must be trivially copyable.");
#else
static_assert(std::is_trivially_copyable<CDHPacket>::value, "CDHPacket must be trivially copyable.");
static_assert(std::is_pod<SensorData>::value, "SensorData must be a POD type.");
static_assert(std::is_pod<CDHPacket>::value, "CDHPacket must be a POD type.");
#endif
#endif
