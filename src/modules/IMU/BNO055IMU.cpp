#include "BNO055IMU.hpp"

BNO055_ID_INF_TypeDef bno055_id_inf;
BNO055_TEMPERATURE_TypeDef temp;

bool BNO055IMU::setup() {

    BNO055_ID_INF_TypeDef id_info;

    impl.read_id_inf(&id_info);
    if (_chip_id == id_info.chip_id) {
        printf("BNO055 CHIP_ID: 0x%02x valid\r\n", id_info.chip_id);
    } else {
        printf("BNO055 CHIP_ID: 0x%02x invalid\r\n", id_info.chip_id);
        return false;
    }


    if (impl.chip_ready() == 0)
    {
        printf("BNO055 is NOT available!!\r\n");
        return false;
    }

    printf(
        "BNO055 CHIP:0x%02x, ACC:0x%02x, MAG:0x%02x, GYR:0x%02x, SW:0x%04x, BL:0x%02x\r\n",
        id_info.chip_id, id_info.acc_id, id_info.mag_id,
        id_info.gyr_id, id_info.sw_rev_id, id_info.bootldr_rev_id);

    impl.get_chip_temperature(&temp);
    printf("BNO055 TEMP acc_chip: %d, gyr_chip: %d\r\n", temp.acc_chip, temp.gyr_chip);
}

void BNO055IMU::update() {

    BNO055_VECTOR_TypeDef accel;
    BNO055_VECTOR_TypeDef mag;
    BNO055_VECTOR_TypeDef gyro;

    BNO055_VECTOR_TypeDef gravity;
    BNO055_VECTOR_TypeDef lin_accel;
    BNO055_QUATERNION_TypeDef quaternion;
    BNO055_EULER_TypeDef angles;

    impl.get_accel(&accel);
    impl.get_mag(&mag);
    impl.get_gyro(&gyro);

    impl.get_gravity(&gravity);
    impl.get_linear_accel(&lin_accel);
    impl.get_quaternion(&quaternion);
    impl.get_euler_angles(&angles);

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
    storage->data->imu.orientation.y = angles.r;
    storage->data->imu.orientation.z = angles.p;
    storage->data->imu.quaternion.r = quaternion.w;
    storage->data->imu.quaternion.i = quaternion.x;
    storage->data->imu.quaternion.j = quaternion.y;
    storage->data->imu.quaternion.k = quaternion.z;
    // TODO: gravity?
    // TODO: lin_accel?
    storage->unlock();

    // printf(
    //     "BNO055"
    //     " GRA_x=%4.2f, GRA_y=%4.2f, GRA_z=%4.2f "
    //     " LIN_ACC_x:%4.2f, LIN_ACC_y:%4.2f, LIN_ACC_z:%4.2f "
    //     "\r\n",
    //     gravity.x, gravity.y, gravity.z,
    //     lin_accel.x, lin_accel.y, lin_accel.z
    // );
}
