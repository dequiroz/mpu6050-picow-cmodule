add_compile_options(
    -Wno-unused-variable
    -Wno-maybe-uninitialized
    -Wno-unused-but-set-variable
)

# Replace this with your actual absolute path
set(PICO_SDK_PATH /home/dirago/MPU_DMP_Test/micropython/lib/pico-sdk)

# Create an INTERFACE library for our C module.
add_library(usermod_imu INTERFACE)

# Add our source files to the lib
target_sources(usermod_imu INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/mpu6050_dmp.cpp
    ${CMAKE_CURRENT_LIST_DIR}/modmpu6050.c
    ${CMAKE_CURRENT_LIST_DIR}/I2Cdev.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MPU6050.cpp
)

# Add the current directory as an include directory.
target_include_directories(usermod_imu INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${PICO_SDK_PATH}/src/rp2_common/pico_double/include
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_imu)
#target_link_libraries(usermod_imu INTERFACE pico_stdlib)
