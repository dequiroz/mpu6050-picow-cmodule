#include <stdint.h>  // For uint8_t
#include "pico/types.h"    // <-- For uint

#ifdef __cplusplus
extern "C" {
#endif

bool imu_dmp_init(uint8_t i2c_id, uint sda, uint scl);
bool imu_dmp_read(float* yaw, float* pitch, float* roll);

#ifdef __cplusplus
}
#endif

