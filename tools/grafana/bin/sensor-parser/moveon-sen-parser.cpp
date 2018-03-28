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
    SensorDataNoGps* data = (SensorDataNoGps*) buf;

    //printf("GPS "
    //       "LAT=%.6f,LON=%.6f,TIME=%u,SPEED=%.4f,COURSE=%.4f,VAR=%.4f"
    //       "\r\n",
    //       data->gps.lat, data->gps.lon, data->gps.timestamp,
    //       data->gps.groundSpeed, data->gps.course, data->gps.magVar);

    for (int i = 0; i < MAX_ENV_SENSORS; i++) {
        printf("ENV%d "
               "TEMP=%.4f,HUM=%.4f,PRES=%.4f,GAS=%.4f"
               "\r\n",
               i,
               data->env[i].temperature,
               data->env[i].humidity,
               data->env[i].pressure,
               data->env[i].gasresistance);
    }

    printf("IMU "
           "ACC_X=%.4f,ACC_Y=%.4f,ACC_Z=%.4f,"
           "GYRO_X=%.4f,GYRO_Y=%.4f,GYRO_Z=%.4f,"
           "MAG_X=%.4f,MAG_Y=%.4f,MAG_Z=%.4f,"
           //"QUAT_W=%.4f,QUAT_X=%.4f,QUAT_Y=%.4f,QUAT_Z=%.4f,"
           "ANG_X=%.4f,ANG_Y=%.4f,ANG_Z=%.4f,"
           "TEMP_ACC=%d,"//TEMP_GYRO=%d,"
           "RSTS=%u"
           "\r\n",
           data->imu.accel.x, data->imu.accel.y, data->imu.accel.z,
           data->imu.gyro.x, data->imu.gyro.y, data->imu.gyro.z,
           data->imu.mag.x, data->imu.mag.y, data->imu.mag.z,
           //data->imu.quaternion.w, data->imu.quaternion.x, data->imu.quaternion.y, data->imu.quaternion.z,
           data->imu.orientation.x, data->imu.orientation.y, data->imu.orientation.z,
           data->imu.temp_accel,//data->imu.temp_gyro,
           data->imu.resets);

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

    printf("TMP OW%d=%.4f", 0, data->temp.temp[0]);
    for(int i = 1; i < MAX_TEMP_SENSORS; i++) {
        printf(",OW%d=%.4f", i, data->temp.temp[i]);
    }
    printf("\r\n");

    printf("SYS "
           "LOG_CNT=%hu,"
           "RTC_S=%hu,"
           "LOG_MS=%hu,"
           "LOCK_US=%u"
           "\r\n",
           data->system.log_cnt,
           data->system.rtc_s,
           data->system.log_ms,
           data->system.lock_wait_us);

    return 0;
}
