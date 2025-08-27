# ATmega32A + MPU6050 IMU Data Fusion

This project demonstrates how to interface the **MPU6050 (3-axis accelerometer + 3-axis gyroscope)** with an **ATmega32A microcontroller** over **I²C**, and perform **sensor fusion** using a complementary filter.  
It outputs real-time orientation (angles) over **USART** at 9600 baud.

---

## ✨ Features

- **MPU6050 Initialization**
  - Configures power management, sample rate, and gyro settings.
  - I²C communication implemented with bit-banged protocol on `PORTC`.

- **Gyroscope & Accelerometer Data**
  - Reads raw 16-bit values for X, Y, Z axes.
  - Converts readings into degrees using scaling factors.

- **Complementary Filter**
  - Combines accelerometer (long-term stable) and gyroscope (short-term accurate) data.
  - Outputs fused angles for roll, pitch, and yaw.

- **Calibration**
  - Gyroscope offset calibration routine.
  - Averages multiple samples for baseline correction.

- **USART Debug Output**
  - Sends sensor data as ASCII text at **9600 baud**.
  - Includes helper functions:
    - `my_putstr()` → print string
    - `my_put_int()` → print integer
    - `my_put_float()` → print float with precision

- **Timer Interrupt**
  - Uses **Timer0 overflow interrupt** for integration timing.
  - Ensures consistent sampling interval.

---

## 🛠 Requirements

### Hardware
- ATmega32A microcontroller (8 MHz).
- MPU6050 IMU module.
- Serial adapter (USB-UART, e.g., CH340/FTDI).
- Optional: PC with terminal software (PuTTY, RealTerm, Arduino Serial Monitor).

### Software
- **CodeVisionAVR**

---


---

## 🚀 How It Works

1. **Initialization**
   - USART configured at 9600 baud.
   - Bit-banged I²C initialized on `PORTC`.
   - MPU6050 configured for active mode.
   - Calibration routine collects gyro offsets.

2. **Main Loop**
   - Reads raw accelerometer and gyroscope data.
   - Applies complementary filter:
     ```
     fused_angle = α * (previous_angle + gyro_rate * dt) + β * accel_angle
     ```
     with `α ≈ 0.98`, `β ≈ 0.02`.
   - Normalizes angles to `[0–360°]`.

3. **Output**
   - Angles printed as ASCII values via USART.
   - Example output:
     ```
     123    45    302
     ```

---

## ▶️ Usage

1. **Wire the MPU6050 to ATmega32A**
   - `SDA` → `PORTC.1`
   - `SCL` → `PORTC.0`
   - `VCC` → 5V (or 3.3V depending on module)
   - `GND` → GND

2. **Flash the firmware**  
   Using `avr-gcc` + `avrdude`:
   ```bash
   avr-gcc -mmcu=atmega32 -Os -o imu.elf mpu6050_project.c
   avr-objcopy -O ihex imu.elf imu.hex
   avrdude -c usbasp -p m32 -U flash:w:imu.hex





