/** @file
 * Common Data Structures
 */

#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>

#ifndef FW_GIT_COMMIT_HASH
#define FW_GIT_COMMIT_HASH ""
#endif
#define DEG2RAD (M_PI / 180.0)
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

enum class MainpanelMode : uint8_t {
    //! [modes]
    NONE     = 0x00, // hex for 0000 0000 // Not used adcs internally
    SLEEPM   = 0x01, // hex for 0000 0001
    ATTDET   = 0x02, // hex for 0000 0010
    DETUMB   = 0x03, // hex for 0000 0011
    NADIR    = 0x04, // hex for 0000 0100
    SUN      = 0x05, // hex for 0000 0101
    FLASH    = 0x06, // hex for 0000 0110
    EXP      = 0x07, // hex for 0000 0111
    TESTM    = 0x08, // hex for 0000 1000
    POWEROFF = 0x0F  // hex for 0000 1111 // Not used adcs internally
    //! [modes]
};

// MainpanelFlags
// Prefix indicates the mode in which the flag is valid
enum class MainpanelFlags : uint8_t {
    //! [flags]
    NONE          = 0x00,
    SUN_EKF       = 0x01, // Use EKF attitude Estimation for sunpointing?
    SUN_ECLIPSE   = 0x02, // Enable coils when in eclipse or when No valid sunvector is present.
    COIL_FCVT     = 0x04, // Fixed current, variable time coil actuation (if disabled variable current, fixed time)
    SUN_3AXES     = 0x10, // 3 axis sunpointing(also try to set the attitude of the z axis)
    EKF_INIT      = 0x20, // Init the EKF. Should be set X mins after changing the mode to SUN.
    TEST_COILDIR  = 0x40, // Set coil direction in testmode.
    COIL_NOSCALE  = 0x80, // Disable proportional downscaling of coil actuation when exceeding threshold
    //! [flags]
};

// DeviceFlags, used to specify a certain panel for active BMX and flashing
enum class Device : uint8_t {
    //! [device]
    NONE = 0x00,
    SP1 = 0x01,
    SP2 = 0x02,
    SP3 = 0x04,
    SP4 = 0x08,
    MP0 = 0x10,
    TP0 = 0x20,
    FLASH_DONE = 0x40, // Set by MP to indicate SP flashing is complete.
    //! [device]
};

struct CoilConfig {
    //! [coilconfig]
    /**
     * coil_x_fwd              0000 0001         0x01
     * coil_x_aft              0000 0010         0x02
     * coil_y_starboard        0000 0100         0x04
     * coil_y_port             0000 1000         0x08
     * coil_z_deck             0001 0000         0x10
     * coil_z_overhead         0010 0000         0x20
     */
    uint8_t activeCoils = 0x2A; // Activte one panel per axis (X-,Y-,Z-)
    // Maximum power per coil (unit 2mA)
    uint8_t maxPower = 50;
    //! [coilconfig]
    uint8_t generation = 1;
} __attribute__((packed));

struct SensorConfig {
    // Selection of active BMX sensor
    Device activeBmx = Device::TP0;
    uint8_t generation = 1;
} __attribute__((packed));

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

struct QuaternionData {
    float r;
    float i;
    float j;
    float k;
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
 * MAINPANEL DATA
 * Transferred from Mainpanel to CDH
 */
struct MainpanelStatus {
    MainpanelMode mode;         // Mainpanel mode
    MainpanelFlags flags;       // Mainpanel flags
    Device flash;               // Mainpanel flash status
    CoilConfig coilConfig;      // Coil configuration
    uint32_t time_update_start; // Current time at start of loop (in ms/10 - csec)
    uint32_t ts_update_start;   // Timestamp at start of loop
    uint32_t ts_sensor_start;   // Timestamp at start of sensor data gathering
    uint32_t ts_sensor_done;    // Timestamp after sensor data gathering
    uint32_t ts_mode1_done;     // Timestamp after first mode specific function
    uint32_t ts_mode2_done;     // Timestamp after second mode specific function (identical to mode1 if only one mode is executed)
    uint32_t ts_before_sleep;   // Timestamp before going to sleep for fixed refresh rate
    uint32_t error;             // Bitmask indicating error states on the mainpanel //TODO: Implement on mainpanel
    char version[8] = FW_GIT_COMMIT_HASH; // Git SHA1 of mainpanel
} __attribute__((packed));

struct MainpanelCalc {
    QuaternionData att_svd;
    QuaternionData att_ekf;
    VectorData est_angular_velocity;
    VectorData mctr;
} __attribute__((packed));

struct GPSData {
    float lat; // Latitude in degrees.
    float lon; // Longitude in degrees.
    float alt; // Altitude in meters.
    float ut;  // Time (seconds since Unix epoch)

    uint8_t generation;
} __attribute__((packed));

struct MainpanelData {
    StartStruct start;

    SensorValues panelData;
    SensorValues sideXPlusData;
    SensorValues sideXMinusData;
    SensorValues sideYPlusData;
    SensorValues sideYMinusData;
    SensorValues topData;

    MainpanelStatus status;
    MainpanelCalc calc;

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

/**
 * MAINPANEL CONTROL
 * Transferred from CDH to Mainpanel
 * Every element contains a generation counter that is incremented to indicate changes
 */
struct ModeData {
    MainpanelMode mode = MainpanelMode::SLEEPM;
    MainpanelFlags flags = MainpanelFlags::NONE;
    Device flash = Device::NONE;
    uint8_t generation = 1;
} __attribute__((packed));

struct TimeData {
    uint16_t year;
    uint32_t csec;
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

struct SensorCalibration {
    PanelSensorCalibration main;
    PanelSensorCalibration sideXPlus;
    PanelSensorCalibration sideXMinus;
    PanelSensorCalibration sideYPlus;
    PanelSensorCalibration sideYMinus;
    PanelSensorCalibration top;
    uint8_t generation;
} __attribute__((packed));

struct ControllerGainsData {
    uint32_t bdot = 10000;

    float sun_2axes[3][5] = {{-0.0000,  0.7606, -92.7963,  -0.0000,   0.0000},
                             {-0.7606,  0.0000,  -0.0000, -74.7403,  -0.0335},
                             { 0.0067,  0.0000,  -0.0000,   0.0590,  -0.3803}};

    float sun_3axes[3][6] = {{ 1.0000,  0.0000,   0.0000,   1.0000,   0.0000,   0.0000},
                             { 0.0000,  1.0000,   0.0000,   0.0000,   1.0000,   0.0000},
                             { 0.0000,  0.0000,   1.0000,   0.0000,   0.0000,   1.0000}};
    float sun_spin_rate = 0;

    float svd_mag_weight = 1000;
    float svd_sun_weight = 1000;
    float svd_cov_threshold = 1;
    float mctr_offset[3] = {0.0, 0.0, 0.0};
    uint8_t generation = 1;
} __attribute__((packed));

struct TleData {
    // First line of Tle string
    uint16_t year;
    float epoch;
    float mean_motion_1st;
    float mean_motion_2nd;
    int16_t mean_motion_2nd_exp;
    float bstar;
    int16_t bstar_exp;

    // Second line of Tle string
    float inclination;
    float raan;
    float eccentricity;
    float argument_pedigree;
    float mean_anomaly;
    float mean_motion;

    uint8_t generation;
} __attribute__((packed));

struct MainpanelControl {
    StartStruct start;

    ModeData mode;
    TimeData time;
    CoilConfig coilConfig = {};
    SensorConfig sensorConfig;
    SensorCalibration sensorCalibration;
    ControllerGainsData controllerGains;
    TleData tle;
    GPSData gpsData;

    VerifyStruct end;
} __attribute__((packed));

/**
 * DAEMON EXCLUSIVE STRUCTURES
 */
enum class LogLevel : uint8_t {
    //! [loglevel]
    VERBOSE = 0x00,
    DEBUG   = 0x01,
    INFO    = 0x02,
    WARNING = 0x04,
    ERROR   = 0x08
    //! [loglevel]
};

union Spidata {
    MainpanelControl control;
    MainpanelData data;
    uint8_t byte[MAX(sizeof(MainpanelControl), sizeof(MainpanelData)) + 2];

    Spidata()
    {
        //TODO: Maybe provide more useful default constructor
        for(unsigned int i = 0; i < sizeof(byte); i++) {
            byte[i] = 0;
        }
    }
    Spidata(const Spidata& c)
    {
        for(unsigned int i = 0; i < sizeof(byte); i++) {
            byte[i] = c.byte[i];
        }
    }
};

#endif
