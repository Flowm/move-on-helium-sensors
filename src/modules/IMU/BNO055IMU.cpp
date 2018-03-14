#include "BNO055IMU.hpp"

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

    printf("BNO055 CHIP:0x%02x, ACC:0x%02x, MAG:0x%02x, GYR:0x%02x, SW:0x%04x, BL:0x%02x\r\n",
        id_info.chip_id, id_info.acc_id, id_info.mag_id,
        id_info.gyr_id, id_info.sw_rev_id, id_info.bootldr_rev_id);

    return true;
}

void BNO055IMU::update() {

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
    //storage->data->imu.quaternion.w = quaternion.w;
    //storage->data->imu.quaternion.x = quaternion.x;
    //storage->data->imu.quaternion.y = quaternion.y;
    //storage->data->imu.quaternion.z = quaternion.z;
    storage->data->imu.temp_accel = temp.acc_chip;
    storage->data->imu.temp_gyro = temp.gyr_chip;
    // TODO: gravity?
    // TODO: lin_accel?
    storage->data->imu.resets_temps = reset_counter_temperatures;
    storage->data->imu.resets_zeroes = reset_counter_zeroes;
    storage->unlock();

    if (temp.acc_chip == 104 && temp.gyr_chip == 104) {
        reset_counter_temperatures++;
        impl.reset();
    }

    if (all_values_zero()) {
        reset_counter_zeroes++;
        impl.reset();
    }
}

bool BNO055IMU::all_values_zero() {
    if (accel.x == 0 &&
        accel.y == 0 &&
        accel.z == 0 &&
        gyro.x == 0 &&
        gyro.y == 0 &&
        gyro.z == 0 &&
        mag.x == 0 &&
        mag.y == 0 &&
        mag.z == 0 &&
        angles.h == 0 &&
        angles.p == 0 &&
        angles.r == 0 &&
        quaternion.w == 0 &&
        quaternion.x == 0 &&
        quaternion.y == 0 &&
        quaternion.z == 0 &&
        temp.acc_chip == 0 &&
        temp.gyr_chip == 0) {
            return true;
        }
    return false;
}
