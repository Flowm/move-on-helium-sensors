#pragma once

// Define max temp sensors (DS18B20) for temperatures array
#define MAX_TEMP_SENSORS 8

// Define max env sensors (BME680)
#define MAX_ENV_SENSORS 2

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
    //SensorQuaternion quaternion;
    int8_t temp_accel;
    //int8_t temp_gyro;
    uint8_t resets;
} __attribute__((packed));

struct SensorTemp {
    float temp;
}__attribute__((packed));

struct SensorADCS {
    //SensorVector accel;
    SensorVector gyro;
    SensorVector mag;
    SensorVector sun;

    float temp[3];
    //int16_t raw_mag[3];
    uint16_t raw_sun[4];
} __attribute__((packed));

struct SensorTOSS {
    uint32_t timestamp;
    int16_t temp[4];
    uint16_t photodiode[9];
    uint16_t checksum;
} __attribute__((packed));

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
    SensorADCS adcs;
    SensorTOSS toss;
    SystemStatus system;
} __attribute__((packed));
