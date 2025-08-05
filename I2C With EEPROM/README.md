# 📘 PIC16F877A Interfacing with External EEPROM via I2C

This project demonstrates how to interface a PIC16F877A microcontroller with an external EEPROM (e.g., 24Cxx series) using the I²C communication protocol. The code is written in Embedded C using the XC8 compiler** and MPLAB X IDE.

---

## 🛠️ Features

- Initializes PIC as an I²C Master at 100 kHz
- Writes data (`'A'`, `'B'`, `'C'`) to 3 different memory addresses of the EEPROM
- Reads the data back from EEPROM and displays it on PORTD
- Demonstrates key I²C operations: Start, Stop, Repeated Start, Write, Read, NACK

---

## ⚙️ Configuration

- Oscillator Frequency: 16 MHz (HS mode)
- I²C EEPROM Address: 
  - Write: `0xA0` 
  - Read: `0xA1`
- Delay: 10 ms after each EEPROM write to allow write cycle to complete
- PORTD: Used to display read data for testing purposes

---

## 📸 Demo Behavior

1. Initializes I²C and sets up PORTD.
2. Writes `'A'`, `'B'`, `'C'` (ASCII 0x41, 0x42, 0x43) to EEPROM addresses `0x0023`, `0x0028`, `0x0036`.
3. Reads the values back with 2-second delays and outputs them to PORTD.

---

## 🔌 Hardware Requirements

- PIC16F877A
- 24C64 EEPROM
- I²C Pull-up Resistors
- 16 MHz Crystal Oscillator
- LEDs or logic analyzer for output testing (optional)

---

## 📚 Concepts Used

- I²C protocol: Multi-byte communication
- EEPROM memory addressing (16-bit)
- Embedded C with bit-level register control
- Microcontroller-to-peripheral communication

---
