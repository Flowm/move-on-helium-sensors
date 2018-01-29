#include "BNO055IMU.hpp"

BNO055_ID_INF_TypeDef bno055_id_inf;

bool BNO055IMU::setup() {

    BNO055_ID_INF_TypeDef id_info;

    impl.read_id_inf(&id_info);
    if (_chip_id == id_info.chip_id) {
        printf("BNO055 CHIP_ID: 0x%02x valid\r\n", id_info.chip_id);
    } else {
        printf("BNO055 CHIP_ID: 0x%02x invalid\r\n", id_info.chip_id);
        return false;
    }


    if (impl.chip_ready() == 0) {
        printf("BNO055 is NOT available!\r\n");
        return false;
    }

    printf(
        "BNO055 CHIP:0x%02x, ACC:0x%02x, MAG:0x%02x, GYR:0x%02x, SW:0x%04x, BL:0x%02x\r\n",
        id_info.chip_id, id_info.acc_id, id_info.mag_id,
        id_info.gyr_id, id_info.sw_rev_id, id_info.bootldr_rev_id);

    return true;
}

void BNO055IMU::update() {

    BNO055_VECTOR_TypeDef accel;
    BNO055_VECTOR_TypeDef mag;
    BNO055_VECTOR_TypeDef gyro;

    BNO055_VECTOR_TypeDef gravity;
    BNO055_VECTOR_TypeDef lin_accel;
    BNO055_QUATERNION_TypeDef quaternion;
    BNO055_EULER_TypeDef angles;

    BNO055_TEMPERATURE_TypeDef temp;

    impl.get_accel(&accel);
    impl.get_mag(&mag);
    impl.get_gyro(&gyro);

    impl.get_gravity(&gravity);
    impl.get_linear_accel(&lin_accel);
    impl.get_quaternion(&quaternion);
    impl.get_euler_angles(&angles);

    impl.get_chip_temperature(&temp);

    storage->lock();
    storage->data->imu.accel.x = accel.x;
    storage->data->imu.accel.y = accel.y;
    storage->data->imu.accel.z = accel.z;
    storage->data->imu.gyro.x = gyro.x;
    storage->data->imu.gyro.y = gyro.y;
    storage->data->imu.gyro.z = gyro.z;
    storage->data->imu.mag.x = mag.x;
    storage->data->imu.mag.y = mag.y;
    storage->data->imu.mag.z = mag.z;
    storage->data->imu.orientation.x = angles.h;
    storage->data->imu.orientation.y = angles.p;
    storage->data->imu.orientation.z = angles.r;
    storage->data->imu.quaternion.w = quaternion.w;
    storage->data->imu.quaternion.x = quaternion.x;
    storage->data->imu.quaternion.y = quaternion.y;
    storage->data->imu.quaternion.z = quaternion.z;
    storage->data->imu.temp_accel = temp.acc_chip;
    storage->data->imu.temp_gyro = temp.gyr_chip;
    // TODO: gravity?
    // TODO: lin_accel?
    storage->unlock();

    // TODO: Remove this after adding these values to central data storage
    // printf(
    //     "BNO055 "
    //     "GRA_X=%4.2f,GRA_Y=%4.2f,GRA_Z=%4.2f,"
    //     "LIN_ACC_X:%4.2f,LIN_ACC_Y:%4.2f,LIN_ACC_Z:%4.2f,"
    //     "\r\n",
    //     gravity.x, gravity.y, gravity.z,
    //     lin_accel.x, lin_accel.y, lin_accel.z
    // );
}
