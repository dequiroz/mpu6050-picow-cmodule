# mpu6050-picow-cmodule

MicroPython C module for using the MPU6050 (with DMP support) on Raspberry Pi Pico W (RP2040).
Based on the RP2040 driver from [Jeff Rowberg’s i2cdevlib](https://github.com/jrowberg/i2cdevlib).

---

## 📦 Overview

This repository wraps the C++ driver code for the MPU6050 IMU sensor and its Digital Motion Processor (DMP), making it accessible as a MicroPython C module on the Raspberry Pi Pico W.

It is intended for robotics, motion tracking, and educational use, where performance and accuracy are important but where full C development is impractical.

---

## 🔧 Features

- ✅ MPU6050 initialization with DMP configuration to enable interruptions when data is available
- ✅ Offset configuration for gyroscope and accelerometer
- ✅ Efficient C-level access from MicroPythonfor Yaw Pitch Roll

---

## 🚀 Getting Started

### 1. Install Prerequisites to build Micropython
```bash
sudo apt update
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential
```
### 2. Clone this repo
```bash
git clone https://github.com/dequiroz/mpu6050-picow-cmodule.git
```

### 3. Install necesary submodules
```bash
cd mpu6050-picow-cmodule
git submodule update --init micropython
cd micropython/ports/rp2
make BOARD=RPI_PICO_W submodules
```

### 4. Build MicroPython with the module
```bash
cd micropython/ports/rp2
make BOARD=RPI_PICO_W USER_C_MODULES=../../../mpu6050_rp2040/micropython.cmake -j100
```

---

## 🧪 Example MicroPython Usage

```python
import imu

imu.init()

while True:
    ax, ay, az = mpu6050.get_accel()
    print("Accel:", ax, ay, az)
```

