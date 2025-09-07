# Voting Machine using PIC16F877A + I2C EEPROM + LCD

This project implements a simple **electronic voting machine** using the **PIC16F877A microcontroller**, **16x2 LCD**, and **I2C EEPROM** for storing votes permanently.  

Even after a power reset, the votes are preserved in EEPROM and can be retrieved on startup.  

---

## 🚀 Features
- **Candidate Selection**: Press buttons to cast votes for Candidate 1 or Candidate 2.  
- **Result Display**: Shows total votes for both candidates on the LCD.  
- **EEPROM Storage**: Votes are saved in an external EEPROM using the I2C protocol.  
- **Reset Option**: Clear all votes and reset EEPROM to zero.  
- **Persistence**: Votes remain stored even after a power cycle.  

---

## 🛠️ Hardware Requirements
- PIC16F877A Microcontroller  
- 16 MHz Crystal Oscillator  
- 16x2 LCD Display  
- Push Buttons for inputs  
- External EEPROM (24C02 / 24C04 / 24C08 etc.)  
- Pull-up resistors for I2C lines (SCL, SDA)  
- Breadboard / PCB and wires  

---

## ⚙️ Pin Configuration
- **PORTB** → Input buttons for voting and actions  
- **PORTC & PORTD** → LCD data & control lines  
- **SCL, SDA** → I2C communication with EEPROM  
- **Crystal** → 16 MHz (for stable I2C operation)  

---

## 📋 Button Mapping
- `0xE0` → Vote for **Candidate 1**  
- `0xD0` → Vote for **Candidate 2**  
- `0xB0` → Show **Results**  
- `0x70` → **Reset** all votes  

---

## 🖥️ Software Requirements
- MPLAB X IDE  
- XC8 Compiler  
- Proteus (for simulation, optional)  

---

## 📂 File Structure
┣ 📜 main.c # Main program file
┣ 📜 README.md # Project documentation


---

## 🔧 How It Works
1. On startup, votes for each candidate are loaded from EEPROM.  
2. When a vote button is pressed, the counter is incremented and saved back into EEPROM.  
3. LCD provides feedback for each action (vote, results, reset).  
4. Press "Results" button to display the total votes of both candidates.  
5. Votes are never lost unless explicitly reset.  

---

## 🖼️ Demo Flow
1. Power ON → LCD shows *"Voting Machine"*  
2. Press Candidate 1 button → LCD shows *"Voted to C1"*  
3. Press Candidate 2 button → LCD shows *"Voted to C2"*  
4. Press Show Results → LCD shows vote counts  
5. Press Reset → LCD shows *"Votes Reset"*  

---

## 📸 Simulation / Hardware Setup
(Add Proteus simulation screenshot or hardware circuit photo here)  

---

## 📜 License
This project is open-source. Feel free to use and modify it for educational purposes.  

---

## 🙌 Acknowledgments
- Microchip MPLAB & XC8 Compiler  
- Standard I2C + LCD interfacing techniques  
- EEPROM (24Cxx series) documentation  

---
