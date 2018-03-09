#pragma once

/**
 * IMPORTED FROM https://gitlab.lrz.de/move-ii/adcs_software
 */

#define MAX(x, y) ((x) > (y)) ? (x) : (y)

/**
 * ENUMS
 */
enum class SidepanelMode : uint8_t {
    SUNDATA = 0x00,
    SLEEPM  = 0x01, // Sleep is a keyword :(
    NORMAL  = 0x02,
    TESTM   = 0x03
};

struct PanelStatus {
    bool valid_data = false;
    bool valid_fw = false;
    uint32_t checksum;
} __attribute__((packed));

/**
 * ASSIST STRUCTS
 */
struct StartStruct {
    uint16_t magic = 0xCAFE;
} __attribute__((packed));

struct VerifyStruct {
    uint8_t magic; // Needs to be set to F0 to indicate valid data
    uint8_t generation;
    uint16_t checksum = 0;
    uint16_t pad = 0xFAFB;
} __attribute__((packed));

struct VectorData {
    float x;
    float y;
    float z;
} __attribute__((packed));

struct MatrixData {
    VectorData x;
    VectorData y;
    VectorData z;
} __attribute__((packed));

/**
 * PROCESSED SENSOR DATA
 */
struct GyroData {
    float x;
    float y;
    float z;
} __attribute__((packed));

struct AccelData {
    float x;
    float y;
    float z;
} __attribute__((packed));

struct MagVector {
    float x;
    float y;
    float z;
    float length;
    uint8_t coilState;
} __attribute__((packed));

struct TempData {
    uint16_t bmx;
    uint16_t t1;
    uint16_t t2;
    uint16_t t3;
    uint16_t t4; // Reserved, currently identical to t3
} __attribute__((packed));

struct SunVector {
    float x;
    float y;
    float z; // Implicitly contains length: z=1/length
    uint16_t intensity = 0;
} __attribute__((packed));

/**
 * RAW SENSOR DATA
 */
struct MagData {
    int16_t x;
    int16_t y;
    int16_t z;
    uint16_t res;
} __attribute__((packed));

struct SunData {
    uint16_t pads[4];
} __attribute__((packed));

struct CoilData {
    float current;
}__attribute__((packed));

/**
 * COMBINED SENSOR DATA
 */
struct SensorValues {
    AccelData acc;
    GyroData gyro;
    MagVector mag;
    TempData temp;
    SunVector sun;
    CoilData coil;

    MagData rawMag;
    SunData rawSun;

    PanelStatus status;
} __attribute__((packed));

/**
 * SIDEPANEL DATA
 * Transferred from Sidepanel to Mainpanel
 */
struct SidepanelStatus {
    SidepanelMode mode;
    uint32_t ts_update_start;   // Timestamp at start of loop
    uint32_t ts_sensor_start;   // Timestamp at start of sensor data gathering
    uint32_t ts_sensor_done;    // Timestamp after sensor data gathering
    uint32_t ts_before_sleep;   // Timestamp before going to sleep for fixed refresh rate
} __attribute__((packed));

struct SidepanelData {
    StartStruct start;

    SensorValues sensors;
    SidepanelStatus status;

    VerifyStruct end;
} __attribute__((packed));

/**
 * SIDEPANEL CONTROL
 * Transferred from Mainpanel to Sidepanel
 * Every element contains a generation counter that is incremented to indicate changes
 */
struct SidepanelModeData {
    SidepanelMode mode;
    uint8_t generation;
} __attribute__((packed));

enum class CoilDirection : uint8_t {
    FWD = 0x08,
    BWD = 0x0B,
};

struct CoilControl {
    CoilDirection dir;
    float current;
    uint16_t duration; // MAX 2 seconds!
    bool calibrate;
    uint8_t generation;
} __attribute__((packed));

struct LedControl {
    uint8_t state;
    uint8_t generation;
} __attribute__((packed));

struct SidepanelTranslationData {
    MatrixData mag;
    MatrixData gyro;
    uint8_t generation;
} __attribute__((packed));

struct LinearCalibration {
    float scale_x = 1;
    float scale_y = 1;
    float scale_z = 1;
    float offset_x = 0;
    float offset_y = 0;
    float offset_z = 0;
} __attribute__((packed));

struct SunSensorCalibration {
    float scale[4] = {1,1,1,1};
    int16_t offset[4] = {0,0,0,0};
    uint16_t threshold[4] = {300,300,300,300};
    uint8_t flappanelCompensation = {0};
} __attribute__((packed));

struct PanelSensorCalibration {
    LinearCalibration acc;
    LinearCalibration gyro;
    LinearCalibration mag;
    SunSensorCalibration sun;
    uint8_t generation;
} __attribute__((packed));

struct SidepanelControl {
    StartStruct start;

    SidepanelModeData mode;
    PanelSensorCalibration calibration;
    SidepanelTranslationData translation;
    CoilControl coil;
    LedControl led;

    VerifyStruct end;
} __attribute__((packed));
