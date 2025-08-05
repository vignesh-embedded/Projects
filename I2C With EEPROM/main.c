#pragma config FOSC = HS        // Configures the oscillator selection bits to use High-Speed oscillator
#pragma config WDTE = OFF       // Disables the Watchdog Timer (WDT)
#pragma config PWRTE = OFF      // Disables the Power-up Timer (PWRT)
#pragma config BOREN = OFF      // Disables Brown-out Reset
#pragma config LVP = OFF        // Disables Low-Voltage In-Circuit Serial Programming (LVP)
#pragma config CPD = OFF        // Disables Data EEPROM Memory Code Protection
#pragma config WRT = OFF        // Disables Write protection for Flash Program Memory
#pragma config CP = OFF         // Disables Flash Program Memory Code Protection

#include <xc.h>                
#define _XTAL_FREQ 16000000     // 16 MHz Oscillator Frequency
#define EEPROM_Address_R  0xA1  // EEPROM Address for Read Operation
#define EEPROM_Address_W  0xA0  // EEPROM Address for Write Operation

// I2C function prototypes
void I2C_Master_Init(const unsigned long baud);  // Initializes the I2C master with a given baud rate
void I2C_Master_Wait();                          // Waits for the I2C operation to complete
void I2C_Master_Start();                         // Sends an I2C start condition
void I2C_Master_RepeatedStart();                 // Sends an I2C repeated start condition
void I2C_Master_Stop();                          // Sends an I2C stop condition
void I2C_NACK();                                 // Sends a NACK signal
unsigned char I2C_Master_Write(unsigned char data); // Writes a byte over I2C and returns the ACK status
unsigned char I2C_Read_Byte(void);               // Reads a byte from the I2C bus

// EEPROM function prototypes
void EEPROM_Write(unsigned int add, unsigned char data); // Writes data to EEPROM at a specific address
unsigned char EEPROM_Read(unsigned int add);             // Reads data from EEPROM at a specific address


void main(void)                      // Main Function
{
    I2C_Master_Init(100000);         // Initializes the I2C  with 100 kHz baud rate
    
    unsigned int Address1 = 0x0023;  
    unsigned char Data1 = 0x41;      // Write Data 'A' in ASCII
    unsigned int Address2 = 0x0028; 
    unsigned char Data2 = 0x42;      // Write Data 'B' in ASCII
    unsigned int Address3 = 0x0036; 
    unsigned char Data3 = 0x43;      // Write Data 'C' in ASCII
    
    // Write data to EEPROM at specified addresses
    EEPROM_Write(Address1, Data1); 
    __delay_ms(10);                 // Waits 10 ms for the write to complete
    EEPROM_Write(Address2, Data2); 
    __delay_ms(10); 
    EEPROM_Write(Address3, Data3); 
    __delay_ms(10); 
    TRISD = 0x00;                   // Configures PORTD as output
    
    // Read data from EEPROM and output it to PORTD
    Address1 = 0x0023; 
    PORTD = EEPROM_Read(Address1);  // Reads data from address 0x0023 and outputs to PORTD
    __delay_ms(2000);               // Delays 2 seconds
    Address2 = 0x0028; 
    PORTD = EEPROM_Read(Address2);  // Reads data from address 0x0028 and outputs to PORTD
    __delay_ms(2000);               // Delays 2 seconds
    Address3 = 0x0036; 
    PORTD = EEPROM_Read(Address3);  // Reads data from address 0x0036 and outputs to PORTD
    while(1)
    {
        // Infinite loop
    }
    return;
}

void I2C_Master_Init(const unsigned long baud)
{
    SSPCON = 0x28;                      // Configures the SSP module as I2C master mode
        SSPCON2 = 0x00;                 // Clears the SSPCON2 register
    SSPADD = (_XTAL_FREQ/(4*baud))-1;   // Sets the I2C clock frequency
    SSPSTAT = 0x00;                     // Clears the SSPSTAT register
    TRISC = 0x18;                       // Configures RC3 (SCL) and RC4 (SDA) as inputs
}

void I2C_Master_Wait() 
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));       // Waits until the I2C module is idle for communication
}

void I2C_Master_Start() 
{
    I2C_Master_Wait();             // Waits for the module to be idle
    SSPCON2 = 0x01;                // Initiates the start condition (SEN = 1)
}

void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();             // Waits for the module to be idle
    SSPCON2 = 0x02;                // Initiates the repeated start condition (RSEN = 1)
}

void I2C_Master_Stop() {
    I2C_Master_Wait();             // Waits for the module to be idle
    SSPCON2 = 0x04;                // Initiates the stop condition (PEN = 1)
}

unsigned char I2C_Master_Write(unsigned char data) 
{
    I2C_Master_Wait();             // Waits for the module to be idle
    SSPBUF = data;                 // Loads data into SSPBUF for transmission
    while (!SSPIF);                // Waits for the transmission to complete
    SSPIF = 0;                     // Clears the interrupt flag
    return ACKSTAT;                // Returns the ACK status
}

unsigned char I2C_Read_Byte(void) 
{
    I2C_Master_Wait();             // Waits for the module to be idle
    SSPCON2 = 0x08;                // Enables reception (RCEN = 1)
    while (!SSPIF);                // Waits for the reception to complete
    SSPIF = 0;                     // Clears the interrupt flag
    I2C_Master_Wait();             // Waits for the module to be idle
    return SSPBUF;                 // Returns the received byte from SSPBUF
}

void I2C_NACK(void) 
{
    ACKDT = 1;                     // Sets ACKDT = 1 for NACK
    I2C_Master_Wait();             // Waits for the module to be idle
    ACKEN = 1;                     // Initiates the NACK signal
}


void EEPROM_Write(unsigned int add, unsigned char data) 
{
    I2C_Master_Start();                          // Sends start condition
    while (I2C_Master_Write(EEPROM_Address_W))   // Waits until the EEPROM is ready
        I2C_Master_RepeatedStart();
    while (I2C_Master_Write(add >> 8))           // Sends the address high byte
        I2C_Master_RepeatedStart();    
    while (I2C_Master_Write((unsigned char)add)) // Sends the address low byte
        I2C_Master_RepeatedStart();
    while (I2C_Master_Write(data))               // Sends the data byte to be written
        I2C_Master_RepeatedStart();
    I2C_Master_Stop();                           // Sends stop condition
}

unsigned char EEPROM_Read(unsigned int add) 
{
    unsigned char Data;
    I2C_Master_Start();                           // Sends start condition
    while (I2C_Master_Write(EEPROM_Address_W))    // Waits until the EEPROM is ready
        I2C_Master_RepeatedStart();             
    I2C_Master_Write(add >> 8);                   // Sends the address high byte   
    I2C_Master_Write((unsigned char)add);         // Sends the address low byte 
    I2C_Master_RepeatedStart();                   // Sends repeated start condition
    I2C_Master_Write(EEPROM_Address_R);           // Switches to read mode
    Data = I2C_Read_Byte();                       // Reads a byte of data
    I2C_NACK();                                   // Sends NACK after reading the byte
    I2C_Master_Stop();                            // Sends stop condition
    return Data;                                  // Returns the read data
}