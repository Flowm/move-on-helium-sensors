#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <string>
#include "../../../../src/include/protocol_sensors.h"

int main(int argc, char **argv) {
    int c;
    int s = 0;
    int invalid = 0;
    char buf[1000];

    // Read binary data from stdin
    while((c = getchar()) != EOF) {
        buf[s++] = c;
    }
    printf("DBG SIZE=%d EXP=%d\n", s, sizeof(SensorDataNoGps));

    if (argc == 2) {
        invalid = std::stoi(argv[1]);
    }
    printf("DBG CHK=%d\n", invalid);

    // Invalid byte to packet size
    int invalid_range_start[8] = {};
    for (int i=0; i<8; i++) {
        if (invalid & (1 << i)) {
            invalid_range_start[i] = 30 + 32 * i;
            printf("DBG Invalid Chunk %d from byte %d\n", i, invalid_range_start[i]);
        }
    }

#if 0
    if (invalid) {
        return 1;
    }

    printf("ENV: %d, IMU: %d, temp: %d, adcs: %d, toss: %d, system: %d\n",
            offsetof(SensorDataNoGps, env),
            offsetof(SensorDataNoGps, imu),
            offsetof(SensorDataNoGps, temp),
            offsetof(SensorDataNoGps, adcs),
            offsetof(SensorDataNoGps, toss),
            offsetof(SensorDataNoGps, system)
          );
#endif

    // Cast to datastructure and print
    SensorDataNoGps* data = (SensorDataNoGps*) buf;

    if (!(invalid & 0x01)) {
    for (int i = 0; i < MAX_ENV_SENSORS; i++) {
        printf("ENV%d "
               "TEMP=%.2f,HUM=%.2f,PRES=%.2f,GAS=%.4f"
               "\r\n",
               i,
               data->env[i].temperature,
               data->env[i].humidity,
               data->env[i].pressure,
               data->env[i].gasresistance);
    }
    }

    if (!(invalid & 0x03)) {
    printf("IMU "
           "ACC_X=%.4f,ACC_Y=%.4f,ACC_Z=%.4f,"
           "GYRO_X=%.4f,GYRO_Y=%.4f,GYRO_Z=%.4f,"
           "MAG_X=%.2f,MAG_Y=%.2f,MAG_Z=%.2f,"
           "ANG_X=%.4f,ANG_Y=%.4f,ANG_Z=%.4f,"
           //"QUAT_W=%.4f,QUAT_X=%.4f,QUAT_Y=%.4f,QUAT_Z=%.4f,"
           "TEMP_ACC=%d,"
           "RSTS=%u"
           "\r\n",
           data->imu.accel.x, data->imu.accel.y, data->imu.accel.z,
           data->imu.gyro.x, data->imu.gyro.y, data->imu.gyro.z,
           data->imu.mag.x, data->imu.mag.y, data->imu.mag.z,
           data->imu.orientation.x, data->imu.orientation.y, data->imu.orientation.z,
           //data->imu.quaternion.w, data->imu.quaternion.x, data->imu.quaternion.y, data->imu.quaternion.z,
           data->imu.temp_accel, //data->imu.temp_gyro,
           data->imu.resets);
    }

    if (!(invalid & 0x0C)) {
    printf("ADCS "
           //"ACC_X=%.4f,ACC_Y=%.4f,ACC_Z=%.4f,"
           "GYRO_X=%.4f,GYRO_Y=%.4f,GYRO_Z=%.4f,"
           "MAG_X=%.4f,MAG_Y=%.4f,MAG_Z=%.4f,"
           "SUN_X=%.4f,SUN_Y=%.4f,SUN_Z=%.4f,"
           "TEMP_OW1=%.4f,TEMP_OW2=%.4f,TEMP_OW3=%.4f,"
           "SUN_RAW1=%hu,SUN_RAW2=%hu,SUN_RAW3=%hu,SUN_RAW4=%hu"
           "\r\n",
           //data->adcs.accel.x, data->adcs.accel.y, data->adcs.accel.z,
           data->adcs.gyro.x, data->adcs.gyro.y, data->adcs.gyro.z,
           data->adcs.mag.x, data->adcs.mag.y, data->adcs.mag.z,
           data->adcs.sun.x, data->adcs.sun.y, data->adcs.sun.z,
           data->adcs.temp[0], data->adcs.temp[1], data->adcs.temp[2],
           data->adcs.raw_sun[0], data->adcs.raw_sun[1], data->adcs.raw_sun[2], data->adcs.raw_sun[3]);
    }

    if (!(invalid & 0x06)) {
    printf("OWT OW%d=%hd", 0, data->temp.temp[0]);
    for(int i = 1; i < MAX_TEMP_SENSORS; i++) {
        printf(",OW%d=%hd", i, data->temp.temp[i]);
    }
    printf("\r\n");
    }

    if (!(invalid & 0x18)) {
    printf("TOSS "
           "RTC=%u",
           data->toss.timestamp);
    for (int i = 0; i < 4; i++) {
        printf(",TMP%d=%hd", i+1, data->toss.temp[i]);
    }
    for (int i = 0; i < 9; i++) {
        printf(",PH%d=%hu", i+1, data->toss.photodiode[i]);
    }
    printf("\r\n");
    }

    if (!(invalid & 0x30)) {
    printf("SYS "
           "LOG_CNT=%hu,"
           "RTC_S=%hu,"
           "RTC_MS=%hu,"
           "LOG_MS=%hu,"
           "LOCK_US=%u,"
           "STATUS=%hu"
           "\r\n",
           data->system.log_cnt,
           data->system.rtc_s,
           data->system.rtc_ms,
           data->system.log_ms,
           data->system.lock_wait_us,
           data->system.sensor_status);
    }

    return 0;
}
