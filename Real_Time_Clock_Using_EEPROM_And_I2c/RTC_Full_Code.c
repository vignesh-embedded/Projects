#pragma config FOSC = HS        // Configures the oscillator selection bits to use High-Speed oscillator
#pragma config WDTE = OFF       // Disables the Watchdog Timer (WDT)
#pragma config PWRTE = OFF      // Disables the Power-up Timer (PWRT)
#pragma config BOREN = OFF      // Disables Brown-out Reset (BOR)
#pragma config LVP = OFF        // Disables Low-Voltage In-Circuit Serial Programming (LVP)
#pragma config CPD = OFF        // Disables Data EEPROM Memory Code Protection
#pragma config WRT = OFF        // Disables Write protection for Flash Program Memory
#pragma config CP = OFF         // Disables Flash Program Memory Code Protection

#include <xc.h>                // Include the PIC18's XC8 header file for built-in functions
#define _XTAL_FREQ 6000000     // Defines the system clock frequency as 6 MHz
#define RS RC2                 // Defines the LCD Register Select pin at RC2
#define EN RC1                 // Defines the LCD Enable pin at RC1
// Function Prototypes
void init(void);                // Function to initialize the microcontroller and peripherals
void i2c_init(const unsigned long);  // Function to initialize I2C communication
void i2c_write(unsigned char);      // Function to write data over I2C
int i2c_read(int);                   // Function to read data from I2C
void i2c_start();                    // Function to generate an I2C start condition
void i2c_wait();                     // Function to wait for I2C communication to complete
void i2c_stop();                     // Function to generate an I2C stop condition
void lcd_cmd(unsigned char);        // Function to send a command to the LCD
void lcd_data(unsigned char);       // Function to send data to the LCD
int bcd_2_dec(int);                  // Function to convert Binary Coded Decimal (BCD) to decimal
int dec_2_bcd(int);                  // Function to convert decimal to BCD
void settime(void);                  // Function to set the time on the RTC (Real Time Clock)
void update(void);                   // Function to update the time from the RTC
// Global variables
char msg1[5] = {"TIME:"};            // Message to display "TIME:" on the LCD
char msg2[5] = {"DATE:"};            // Message to display "DATE:" on the LCD
int sec = 58, min = 59, hour = 23;   // Initial values for seconds, minutes, and hours
int date = 10, month = 10, year = 25;  // Initial values for date, month, and year
char sec1, sec2, min1, min2, hour1, hour2, date1, date2, month1, month2, year1, year2; // Variables for displaying time and date

void main()
{
    init();               // Initialize the system
    i2c_init(100000);     // Initialize I2C with a clock frequency of 100 kHz
    settime();            // Set the initial time on the RTC
    while(1)              // Infinite loop to continuously update the display
    {
        update();         // Update the time from the RTC
        // Break down the time into individual digits for display
        sec1 = sec / 10;  
        sec2 = sec % 10;
        min1 = min / 10;
        min2 = min % 10;
        hour1 = hour / 10;
        hour2 = hour % 10;
        date1 = date / 10;
        date2 = date % 10;
        month1 = month / 10;
        month2 = month % 10;
        year1 = year / 10;
        year2 = year % 10;  
        // Display the time on the first row of the LCD
        lcd_cmd(0x80);               // Set the cursor to the beginning of the first line
        for(int i = 0; i < 5; i++)       // Display "TIME:" on the LCD
            lcd_data(msg1[i]);
        lcd_data(hour1 + '0');       // Display hour (first digit)
        lcd_data(hour2 + '0');       // Display hour (second digit)
        lcd_data(0x2D);              // Display a dash '-'
        lcd_data(min1 + '0');        // Display minute (first digit)
        lcd_data(min2 + '0');        // Display minute (second digit)
        lcd_data(0x2D);              // Display a dash '-'
        lcd_data(sec1 + '0');        // Display second (first digit)
        lcd_data(sec2 + '0');        // Display second (second digit)
        // Display the date on the second row of the LCD
        lcd_cmd(0xC0);               // Set the cursor to the beginning of the second line
        for(int i = 0; i < 5; i++)       // Display "DATE:" on the LCD
            lcd_data(msg2[i]);
        lcd_data(date1 + '0');       // Display date (first digit)
        lcd_data(date2 + '0');       // Display date (second digit)
        lcd_data(0x2D);              // Display a dash '-'
        lcd_data(month1 + '0');      // Display month (first digit)
        lcd_data(month2 + '0');      // Display month (second digit)
        lcd_data(0x2D);              // Display a dash '-'
        lcd_data(year1 + '0');       // Display year (first digit)
        lcd_data(year2 + '0');       // Display year (second digit)
        __delay_ms(500);             // Wait for 500 milliseconds before updating the display again
    }
}
void settime(void)
{
    i2c_start();                    // Generate start condition for I2C
    i2c_write(0xD0);                // Send the RTC address with write command
    i2c_write(0x00);                // Set the RTC register address to 0 (seconds)
    i2c_write(dec_2_bcd(sec));      // Set the seconds on the RTC
    i2c_write(dec_2_bcd(min));      // Set the minutes on the RTC
    i2c_write(dec_2_bcd(hour));     // Set the hours on the RTC
    i2c_write(1);                   // Set the day of the week (Monday)
    i2c_write(dec_2_bcd(date));     // Set the date on the RTC
    i2c_write(dec_2_bcd(month));    // Set the month on the RTC
    i2c_write(dec_2_bcd(year));     // Set the year on the RTC
    i2c_stop();                     // Generate stop condition for I2C
}
int dec_2_bcd(int temp)
{
    return ((temp / 10) << 4) + (temp % 10);   // Convert decimal to Binary Coded Decimal (BCD)
}
int bcd_2_dec(int temp)
{
    return ((temp >> 4) * 10) + (temp & 0x0F);  // Convert BCD to decimal
}
void i2c_stop()
{
    i2c_wait();        // Wait for any ongoing I2C communication to complete
    PEN = 1;           // Generate I2C stop condition
}
void i2c_wait()
{
    while(SSPCON2 & 0x1F || SSPSTAT & 0x04);  // Wait for the I2C module to be ready for communication
}
void i2c_start()
{
    i2c_wait();        // Wait for any ongoing I2C communication to complete
    SEN = 1;           // Generate I2C start condition
}
void i2c_init(const unsigned long baud)
{
    SSPCON = 0x28;             // Configure the I2C master mode, enable serial port
    SSPSTAT = 0x00;            // Set all I2C status bits to 0
    SSPCON2 = 0x00;            // Set all I2C control bits to 0
    SSPADD = (_XTAL_FREQ / (4 * baud)) - 1;  // Set the I2C clock frequency
}
void init(void)
{
    TRISD = 0x00;      // Set PORTD as output (for LCD data)
    TRISC = 0x018;
    PORTD = 0x00;      // Initialize PORTD to 0 (clear display)
    lcd_cmd(0x30);     // Initialize the LCD in 8-bit mode
    __delay_ms(5);     // Wait for the LCD to initialize
    lcd_cmd(0x30);     // Repeat the initialization
    __delay_ms(5);     // Wait for the LCD to initialize
    lcd_cmd(0x38);     // Repeat the initialization
    __delay_ms(5);     // Wait for the LCD to initialize
    lcd_cmd(0x38);     // Final initialization step
    __delay_ms(5);     // Wait for the LCD to initialize
    lcd_cmd(0x0C);     // Turn on the LCD display
    __delay_ms(5);     // Wait for the LCD to process
    lcd_cmd(0x06);     // Set the LCD cursor to move right
    __delay_ms(5);     // Wait for the LCD to process
    lcd_cmd(0x01);     // Clear the LCD screen
    __delay_ms(5);     // Wait for the LCD to process
}
void i2c_write(unsigned char temp)
{
    i2c_wait();        // Wait for the I2C module to be ready
    SSPBUF = temp;     // Write the byte to the I2C data buffer
}
int i2c_read(int ack)
{
    int value;
    i2c_wait();        // Wait for the I2C module to be ready
    RCEN = 1;          // Enable the I2C receive mode
    i2c_wait();        // Wait for the data to be received
    value = SSPBUF;    // Read the received data from the buffer
    i2c_wait();        // Wait for the next operation to complete
    ACKDT = (ack) ? 0 : 1;   // Send ACK or NACK based on the 'ack' parameter
    ACKEN = 1;         // Enable the acknowledgment
    return value;      // Return the received value
}
void update(void)
{
    i2c_start();                    // Start I2C communication
    i2c_write(0xD0);                // Write RTC address with write command
    i2c_write(0);                   // Set the RTC register address to 0 (seconds)
    i2c_stop();                     // Stop I2C communication
    i2c_start();                    // Start I2C communication again
    i2c_write(0xD1);                // Write RTC address with read command
    sec = bcd_2_dec(i2c_read(1));   // Read and convert seconds from BCD to decimal
    min = bcd_2_dec(i2c_read(1));   // Read and convert minutes from BCD to decimal
    hour = bcd_2_dec(i2c_read(1));  // Read and convert hours from BCD to decimal
    (i2c_read(1));                  // Read and discard unused day of the week data
    date = bcd_2_dec(i2c_read(1));  // Read and convert date from BCD to decimal
    month = bcd_2_dec(i2c_read(1)); // Read and convert month from BCD to decimal
    year = bcd_2_dec(i2c_read(0));  // Read and convert year from BCD to decimal
    i2c_stop();                     // Stop I2C communication
}
void lcd_cmd(unsigned char i)
{
    RS = 0;              // Set RS to 0 for command mode
    PORTD = i;           // Send the command to the LCD
    EN = 1;              // Enable the LCD to latch the command
    EN = 0;              // Disable the LCD to complete the command
    __delay_ms(5);       // Wait for the LCD to process the command
}
void lcd_data(unsigned char i)
{
    RS = 1;              // Set RS to 1 for data mode
    PORTD = i;           // Send the data to the LCD
    EN = 1;              // Enable the LCD to latch the data
    EN = 0;              // Disable the LCD to complete the data operation
    __delay_ms(5);       // Wait for the LCD to process the data
}