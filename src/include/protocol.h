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
} __attribute__((packed));

struct SensorTemp
{
    float temp;
    char rom[8];
};

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
    uint8_t start = 0x01;
    uint8_t status;
    uint16_t numBytes;
} __attribute__((packed));

struct InProtocolFooter {
    uint8_t checksum;
    uint8_t end = 0x04;
} __attribute__((packed));

struct CDHPacket {
    InProtocolHeader header;
    SensorData data;
    InProtocolFooter footer;
} __attribute__((packed));
