# PIC16F877A RTC with DS1307 and LCD16x2

This project demonstrates how to interface a **PIC16F877A microcontroller** with the **DS1307 Real-Time Clock (RTC)** using the **I2C protocol**.  
The system displays the current **time** and **date** on a **16x2 LCD**.  
The project was developed using **MPLAB IDE with XC8 compiler** and tested in **Proteus simulation software**.

---

## 📌 Features
- Displays **Time** in `HH:MM:SS` format.
- Displays **Date** in `DD:MM:YY` format.
- Uses **I2C communication** between PIC16F877A and DS1307 RTC.
- LCD16x2 interface in **8-bit mode**.
- Written in **embedded C (XC8)** with modular, easy-to-read code.
- Fully **simulated in Proteus** before hardware implementation.

---

## ⚙️ Hardware Requirements
- PIC16F877A Microcontroller
- DS1307 RTC (Real-Time Clock IC)
- 32.768 kHz Crystal for DS1307
- CR2032 Battery (for RTC backup) (Used For Hardware)
- LCD16x2 Display
- Pull-up Resistors (10 kΩ for SDA & SCL lines)
- Power Supply (5V regulated)

---

## 🖥️ Software Requirements
- **MPLAB X IDE** (with XC8 Compiler)
- **Proteus Design Suite** (for simulation)
- GitHub (for version control and sharing)

---

## 🔗 Pin Connections

| Component       | Pin on PIC16F877A | Notes                        |
|-----------------|------------------|------------------------------|
| DS1307 SDA      | RC4 (SDA)        | I2C Data line (with pull-up) |
| DS1307 SCL      | RC3 (SCL)        | I2C Clock line (with pull-up)|
| LCD RS          | RD0              | Control pin                  |
| LCD EN          | RD1              | Enable pin                   |
| LCD D4 - D7     | RD2 - RD5        | Data pins (4-bit mode)       |
| VCC, GND        | +5V, GND         | Power supply                 |

---

## 📜 Code Overview
- **main.c**  
  Initializes peripherals, reads time/date from DS1307, and displays them on LCD.  
- **i2c.c / i2c.h**  
  Implements I2C protocol functions (`I2C_Init`, `I2C_Start`, `I2C_Write`, `I2C_Read`, etc.).  
- **lcd.c / lcd.h**  
  Provides functions to initialize and control the LCD in 4-bit mode.  
- **ds1307.c / ds1307.h**  
  Provides RTC read/write functions for time and date.

---

## 🔧 How It Works
1. **Initialization**  
   - I2C is initialized at 100 kHz.  
   - LCD is initialized in 4-bit mode.  

2. **RTC Communication**  
   - DS1307 continuously keeps track of time and date.  
   - PIC16F877A communicates with DS1307 over I2C to fetch updated time/date values.  

3. **Display**  
   - The fetched data is converted into ASCII format.  
   - Time is displayed on the **first line of the LCD**.  
   - Date is displayed on the **second line of the LCD**.  

---

## 📊 Block Diagram
+-------------+ I2C +-------+
| | <------------> | |
| PIC16F877A | | DS1307|
| | | RTC |
+-------------+ +-------+
|
| 4-bit Data + Control
v
+---------+
| LCD16x2|
+---------+
---

## ▶️ Simulation in Proteus
- The Proteus file (`rtc_lcd.pdsprj`) simulates the circuit.  
- LCD will show live **time** and **date**.  
- You can adjust DS1307 registers to test different times/dates.  

*(Insert Screenshot or GIF here)*

---

## 📂 Repository Structure

![Uploading Screenshot from 2025-09-12 18-58-49.png…]()


