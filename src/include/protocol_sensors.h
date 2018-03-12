#pragma once

// Define max temp sensors (DS18B20) for temperatures array
#define MAX_TEMP_SENSORS 8

// Define max env sensors (BME680)
#define MAX_ENV_SENSORS 3

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
    float lat;
    float lon;
    float groundSpeed; // Knots.
    float course; // Degrees from True North.
    float magVar;
    uint32_t timestamp; //Unix timestamp(seconds)

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

struct SystemStatus {
    uint16_t log_cnt;       // Number of total logged data sets since reset
    uint16_t rtc_s;         // Time since reset in seconds
    uint16_t log_ms;        // Time for writing the last log set to storage
    uint16_t lock_wait_us;  // Delay until data structure could be locked
}__attribute__((packed));

struct SensorData {
    SensorGPS gps;
    SensorENV env[MAX_ENV_SENSORS];
    SensorIMU imu;
    SensorTemp temp[MAX_TEMP_SENSORS];
    SystemStatus system;
} __attribute__((packed));

