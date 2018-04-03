#include "BNO055IMU.hpp"

bool BNO055IMU::setup() {
    BNO055_ID_INF_TypeDef id_info;

    impl.read_id_inf(&id_info);
    if (_chip_id == id_info.chip_id) {
        logger->lock();
        logger->printf("BNO055 CHIP_ID: 0x%02x valid\r\n", id_info.chip_id);
        logger->unlock();
    } else {
        logger->lock();
        logger->printf("BNO055 CHIP_ID: 0x%02x invalid\r\n", id_info.chip_id);
        logger->unlock();
        return false;
    }

    if (impl.chip_ready() == 0) {
        logger->lock();
        logger->printf("BNO055 is NOT available!\r\n");
        logger->unlock();
        return false;
    }

    logger->lock();
    logger->printf("BNO055 CHIP:0x%02x, ACC:0x%02x, MAG:0x%02x, GYR:0x%02x, SW:0x%04x, BL:0x%02x\r\n",
        id_info.chip_id, id_info.acc_id, id_info.mag_id,
        id_info.gyr_id, id_info.sw_rev_id, id_info.bootldr_rev_id);
    logger->unlock();

    set_update_rate(200);

    return true;
}

void BNO055IMU::update() {
    // Retrieve sensor data
    impl.get_accel(&accel);
    impl.get_mag(&mag);
    impl.get_gyro(&gyro);
    //impl.get_gravity(&gravity);
    //impl.get_linear_accel(&lin_accel);
    //impl.get_quaternion(&quaternion);
    impl.get_euler_angles(&angles);
    impl.get_chip_temperature(&temp);
    last_data = storage->get_ts();

    data.accel.x = accel.x;
    data.accel.y = accel.y;
    data.accel.z = accel.z;
    data.mag.x = mag.x;
    data.mag.y = mag.y;
    data.mag.z = mag.z;
    data.gyro.x = gyro.x;
    data.gyro.y = gyro.y;
    data.gyro.z = gyro.z;
    data.orientation.x = angles.h;
    data.orientation.y = angles.p;
    data.orientation.z = angles.r;
    data.temp_accel = temp.acc_chip;
    data.resets = assemble_combined_resets();

    storage->lock();
    storage->data->imu = data;
    storage->unlock();

    if (temp.acc_chip == 104 && temp.gyr_chip == 104) {
        reset_counter_temperatures++;
        impl.reset();
    }

    if (all_values_zero()) {
        last_data = 0;
        reset_counter_zeroes++;
        impl.reset();
    }
}

bool BNO055IMU::all_values_zero() {
    if (accel.x == 0 &&
        accel.y == 0 &&
        accel.z == 0 &&
        mag.x == 0 &&
        mag.y == 0 &&
        mag.z == 0 &&
        gyro.x == 0 &&
        gyro.y == 0 &&
        gyro.z == 0 &&
        angles.h == 0 &&
        angles.p == 0 &&
        angles.r == 0 &&
        temp.acc_chip == 0 &&
        temp.gyr_chip == 0) {
            return true;
        }
    return false;
}

uint8_t BNO055IMU::assemble_combined_resets() {
    return (reset_counter_temperatures & 0xF) << 4 | (reset_counter_zeroes & 0xF);
}

void BNO055IMU::print() {
    // Only print valid data
    if (!last_data) {
        return;
    }
    logger->lock();
    logger->printf("%s T=%lu,"
                   "ACC_X=%.4f,ACC_Y=%.4f,ACC_Z=%.4f,"
                   "MAG_X=%.2f,MAG_Y=%.2f,MAG_Z=%.2f,"
                   "GYRO_X=%.4f,GYRO_Y=%.4f,GYRO_Z=%.4f,"
                   "ANG_X=%.4f,ANG_Y=%.4f,ANG_Z=%.4f,"
                   "TEMP_ACC=%d,"
                   "RSTS=%u"
                   "\r\n",
                   _name, last_data,
                   data.accel.x, data.accel.y, data.accel.z,
                   data.mag.x, data.mag.y, data.mag.z,
                   data.gyro.x, data.gyro.y, data.gyro.z,
                   data.orientation.x, data.orientation.y, data.orientation.z,
                   data.temp_accel,
                   data.resets);
    logger->unlock();
}
