#include <stdint.h>  // For uint8_t
#include "pico/types.h"    // <-- For uint

#ifdef __cplusplus
extern "C" {
#endif

bool imu_dmp_init(uint8_t i2c_id, uint sda, uint scl);
bool imu_dmp_read(float* yaw, float* pitch, float* roll);
bool imu_check_and_read();
void imu_get_ypr(float *dest);
void imu_set_accel_offset_x(int16_t x);
void imu_set_accel_offset_y(int16_t y);
void imu_set_accel_offset_z(int16_t z);
void imu_set_gyro_offset_x(int16_t x);
void imu_set_gyro_offset_y(int16_t y);
void imu_set_gyro_offset_z(int16_t z);

#ifdef __cplusplus
}
#endif

