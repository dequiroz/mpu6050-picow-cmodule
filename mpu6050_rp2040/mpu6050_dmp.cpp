#include "mpu6050_dmp.h"
#include "MPU6050_6Axis_MotionApps_V6_12.h"

extern "C" {
    #include "py/obj.h"
    #include "py/runtime.h"
    #include "py/stream.h"  // <-- needed for mp_printf and mp_plat_print
}

// Global instance
static MPU6050 mpu;
Quaternion q;
VectorFloat gravity;
float ypr[3];
uint8_t fifoBuffer[64]; // FIFO storage buffer

bool imu_dmp_init(uint8_t i2c_id, uint sda, uint scl) {
    mp_printf(&mp_plat_print, "Setting up i2c pins: SDA=%d, SCL=%d on I2C%d\n", sda, scl, i2c_id);

    i2c_inst_t* i2c = (i2c_id == 1) ? i2c1 : i2c0;
    i2c_init(i2c, 400*1000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

    //sleep_ms(100); // Wait a bit before initializing

    mp_printf(&mp_plat_print, "Initializing MPU\n");
    mpu.initialize();

    // You MUST disable sleep mode
    //mpu.setSleepEnabled(false);
    //sleep_ms(100); // Give time to wake up

    mp_printf(&mp_plat_print, "Testing device connections...\n");
    if (mpu.testConnection()) {
        mp_printf(&mp_plat_print, "MPU6050 connection successful\n");
    } else {
        mp_printf(&mp_plat_print, "MPU6050 connection failed\n");
        return false;
    }

    //mp_printf(&mp_plat_print, "Calling dmpInitialize\n");
    //sleep_ms(100); // allow MPU6050 to stabilize

    //uint8_t whoami = mpu.getDeviceID();
    //mp_printf(&mp_plat_print, "WHOAMI: 0x%x\n", whoami);

    uint8_t devStatus = mpu.dmpInitialize();

    //mp_printf(&mp_plat_print, "DMP Initialization (code %d)\n", devStatus);

    if (devStatus == 0) {
        mp_printf(&mp_plat_print, "MPU DMP Initialized OK\n");

        mpu.setDMPEnabled(true);
        mp_printf(&mp_plat_print, "DMP Enabled\n");

        return true;
    } else {
        mp_printf(&mp_plat_print, "DMP Initialization failed (code %d)\n", devStatus);
        return false;
    }
}

bool imu_dmp_read(float* yaw, float* pitch, float* roll) {
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        *yaw = ypr[0] * 180/M_PI;
        *pitch = ypr[1] * 180/M_PI;
        *roll = ypr[2] * 180/M_PI;
        return true;
    }
    return false;
}

