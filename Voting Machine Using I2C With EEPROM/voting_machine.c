// PIC16F877A Configuration Bit Settings
#pragma config FOSC = HS        // Use HS crystal (for I2C stability, 16 MHz recommended)
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config BOREN = OFF      
#pragma config LVP = OFF        
#pragma config CPD = OFF        
#pragma config WRT = OFF        
#pragma config CP = OFF         

#include <xc.h>
#define _XTAL_FREQ 6000000     // 6 MHz Oscillator

// EEPROM I2C Addresses
#define EEPROM_Address_R  0xA1
#define EEPROM_Address_W  0xA0

// LCD Functions
void init(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);
void lcd_number(unsigned int);

// I2C + EEPROM Functions
void I2C_Master_Init(const unsigned long baud);
void I2C_Master_Wait();
void I2C_Master_Start();
void I2C_Master_RepeatedStart();
void I2C_Master_Stop();
unsigned char I2C_Master_Write(unsigned char data);
unsigned char I2C_Read_Byte(void);
void I2C_NACK(void);
void EEPROM_Write(unsigned int add, unsigned char data);
unsigned char EEPROM_Read(unsigned int add);

// Voting Variables
unsigned char value;
unsigned int Candidate_1 = 0, Candidate_2 = 0;

void main() 
{
    init();

    // Load votes from EEPROM at startup
    Candidate_1 = EEPROM_Read(0x0000);
    Candidate_2 = EEPROM_Read(0x0001);

    lcd_command(0x80);
    char array[] = {"Voting Machine"};
    for(int x=0; array[x] != '\0'; x++) 
    {
        lcd_data(array[x]);
    }
    __delay_ms(1000);
    lcd_command(0x01);

    while (1) 
    {
        value = PORTB;
        switch(value) 
        {
            case 0xE0:   // Candidate 1 Vote
                Candidate_1++;
                EEPROM_Write(0x0000, Candidate_1);   // Save in EEPROM
                char c1[] = {"Voted to C1"};
                for(int x=0; c1[x] != '\0'; x++)
                {
                    lcd_data(c1[x]);
                }
                __delay_ms(200);
                lcd_command(0x01);
                break;

            case 0xD0:   // Candidate 2 Vote
                Candidate_2++;
                EEPROM_Write(0x0001, Candidate_2);   // Save in EEPROM
                char c2[] = {"Voted to C2"};
                for(int x=0; c2[x] != '\0'; x++)
                {
                    lcd_data(c2[x]);
                }
                __delay_ms(200);
                lcd_command(0x01);
                break;

            case 0xB0:   // Show Results
                lcd_command(0x80);
                char a[] = {"Total Votes"};
                for(int x=0; a[x] != '\0'; x++)
                {
                    lcd_data(a[x]);
                }
                lcd_command(0xC0);
                lcd_data('C');
                lcd_data('1');
                lcd_data(':');
                lcd_number(Candidate_1);
                lcd_command(0xC6);
                lcd_data('C');
                lcd_data('2');
                lcd_data(':');
                lcd_number(Candidate_2);
                __delay_ms(1000);
                lcd_command(0x01);
                break;

            case 0x70:   // Reset Votes
                Candidate_1 = 0;
                Candidate_2 = 0;
                EEPROM_Write(0x0000, Candidate_1);
                EEPROM_Write(0x0001, Candidate_2);
                lcd_command(0x80);
                char reset[] = {"Votes Reset"};
                for(int x=0; reset[x] != '\0'; x++)
                {
                    lcd_data(reset[x]);
                }
                __delay_ms(1000);
                lcd_command(0x01);
                break;
                
            default:
                break;
        }
    }
}

void init() 
{
    TRISC=0x00;
    PORTC=0x00;
    TRISD=0x00;
    PORTD=0x00;
    TRISB=0xF0;
    PORTB=0x00;
    OPTION_REG &=0x7F;

    // LCD Init
    lcd_command(0x30);
    __delay_ms(10);
    lcd_command(0x30);
    __delay_ms(10);
    lcd_command(0x30);
    __delay_ms(10);
    lcd_command(0x38);
    __delay_ms(10);
    lcd_command(0x06);
    __delay_ms(10);
    lcd_command(0x0C);
    __delay_ms(10);
    lcd_command(0x01);
    __delay_ms(10);

    // I2C Init
    I2C_Master_Init(100000);
}

// ================= LCD FUNCTIONS =================
void lcd_command(unsigned char i) 
{
    PORTC &= ~0x02;  
    PORTD = i;
    PORTC |= 0x01;
    __delay_ms(10);
    PORTC &= ~0x01;
    __delay_ms(10);
}

void lcd_data(unsigned char j) 
{
    PORTC |= 0x02;  // RS connected to RC1
    PORTD = j;
    PORTC |= 0x01;
    __delay_ms(10);
    PORTC &= ~0x01;
    __delay_ms(10);
}

void lcd_number(unsigned int num) 
{
    char digits[5];
    int i = 0;
    if(num == 0)
    { 
        lcd_data('0');
        return;
    }
    while(num > 0) 
    {
        digits[i++] = (num % 10) + '0';
        num /= 10;
    }
    for(int j = i-1; j >= 0; j--)
    {
        lcd_data(digits[j]);
    }
}

// ================= I2C + EEPROM FUNCTIONS =================
void I2C_Master_Init(const unsigned long baud) 
{
    SSPCON = 0x28;
    SSPCON2 = 0x00;
    SSPADD = (_XTAL_FREQ/(4*baud))-1;
    SSPSTAT = 0x00;
    TRISC = 0x18; 
}

void I2C_Master_Wait()
{ 
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start() 
{
    I2C_Master_Wait();
    SEN = 1;
}

void I2C_Master_RepeatedStart() 
{
    I2C_Master_Wait();
    RSEN = 1;
}

void I2C_Master_Stop()
{
    I2C_Master_Wait();
    PEN = 1;
}

unsigned char I2C_Master_Write(unsigned char data) 
{
    I2C_Master_Wait();
    SSPBUF = data;
    while(!SSPIF);
    SSPIF=0;
    return ACKSTAT;
}

unsigned char I2C_Read_Byte(void)
{
    I2C_Master_Wait();
    RCEN=1;
    while(!SSPIF);
    SSPIF=0;
    I2C_Master_Wait();
    return SSPBUF;
}

void I2C_NACK(void)
{
    ACKDT=1;
    I2C_Master_Wait();
    ACKEN=1;
}

void EEPROM_Write(unsigned int add, unsigned char data) 
{
    I2C_Master_Start();
    while (I2C_Master_Write(EEPROM_Address_W))
    {
        I2C_Master_RepeatedStart();
    }
    I2C_Master_Write(add>>8);
    I2C_Master_Write((unsigned char)add);
    I2C_Master_Write(data);
    I2C_Master_Stop();
    __delay_ms(10);
}

unsigned char EEPROM_Read(unsigned int add)
{
    unsigned char Data;
    I2C_Master_Start();
    while (I2C_Master_Write(EEPROM_Address_W))
    {
        I2C_Master_RepeatedStart();
    }
    I2C_Master_Write(add>>8);
    I2C_Master_Write((unsigned char)add);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(EEPROM_Address_R);
    Data = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    return Data;
}
