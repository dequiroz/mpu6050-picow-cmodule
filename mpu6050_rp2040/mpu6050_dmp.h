//#pragma once
//#include <stdbool.h>

//bool imu_dmp_init();
//bool imu_dmp_read(float* yaw, float* pitch, float* roll);

#ifdef __cplusplus
extern "C" {
#endif

bool imu_dmp_init();
bool imu_dmp_read(float* yaw, float* pitch, float* roll);

#ifdef __cplusplus
}
#endif

