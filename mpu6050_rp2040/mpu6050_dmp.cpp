#include "mpu6050_dmp.h"
#include "MPU6050_6Axis_MotionApps_V20.h"

extern "C" {
    #include "py/obj.h"
    #include "py/runtime.h"
    #include "py/stream.h"  // <-- needed for mp_printf and mp_plat_print
}

// Global instance
static MPU6050 mpu;
Quaternion q;
VectorFloat gravity;
float current_ypr[3];  // yaw, pitch, roll
static bool dmpReady = false;
static uint16_t packetSize = 0;
uint8_t fifoBuffer[64]; // FIFO storage buffer

bool imu_dmp_init(uint8_t i2c_id, uint sda, uint scl) {
    mp_printf(&mp_plat_print, "Setting up i2c pins: SDA=%d, SCL=%d on I2C%d\n", sda, scl, i2c_id);

    i2c_inst_t* i2c = (i2c_id == 1) ? i2c1 : i2c0;
    i2c_init(i2c, 400*1000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

    mpu.initialize();

    if (!mpu.testConnection()) {
        mp_printf(&mp_plat_print, "MPU6050 connection failed\n");
        return false;
    }

    uint8_t devStatus = mpu.dmpInitialize();

    if (devStatus != 0) {
        mp_printf(&mp_plat_print, "DMP Initialization failed (code %d)\n", devStatus);
        return false;
    }
    mpu.setDMPEnabled(true);
    packetSize = mpu.dmpGetFIFOPacketSize();
    dmpReady = true;
    return true;
}

// Functions to set accelerometer offset parameters
void imu_set_accel_offset_x(int16_t x) {
    mpu.setXAccelOffset(x);
}
void imu_set_accel_offset_y(int16_t y) {
    mpu.setYAccelOffset(y);
}

void imu_set_accel_offset_z(int16_t z) {
    mpu.setZAccelOffset(z);
}
// Functions to set gyroscope offset parameters
void imu_set_gyro_offset_x(int16_t x) {
    mpu.setXGyroOffset(x);
}
void imu_set_gyro_offset_y(int16_t y) {
    mpu.setYGyroOffset(y);
}
void imu_set_gyro_offset_z(int16_t z) {
    mpu.setZGyroOffset(z);
}

bool imu_check_and_read() {
    if (!dmpReady) return false;

    uint16_t fifoCount = mpu.getFIFOCount();
    uint8_t mpuIntStatus = mpu.getIntStatus();

    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
        mp_printf(&mp_plat_print, "FIFO overflow!\n");
        return false;
    } else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);

        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(current_ypr, &q, &gravity);

        return true;
    }
    return false;
}

void imu_get_ypr(float *dest) {
    dest[0] = current_ypr[0] * 180/M_PI;
    dest[1] = current_ypr[1] * 180/M_PI;
    dest[2] = current_ypr[2] * 180/M_PI;
}

bool imu_dmp_read(float* yaw, float* pitch, float* roll) {
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(current_ypr, &q, &gravity);
        *yaw = current_ypr[0] * 180/M_PI;
        *pitch = current_ypr[1] * 180/M_PI;
        *roll = current_ypr[2] * 180/M_PI;
        return true;
    }
    return false;
}

