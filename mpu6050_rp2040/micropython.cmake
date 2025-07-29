add_compile_options(
    -Wno-unused-variable
    -Wno-maybe-uninitialized
    -Wno-unused-but-set-variable
)

# Links SDK PATH to micropython folder
set(PICO_SDK_PATH ${MICROPY_DIR}/lib/pico-sdk)
message(STATUS "Using Pico SDK from: ${PICO_SDK_PATH}")

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
