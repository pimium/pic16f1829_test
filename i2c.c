
// PIC16F877A Configuration Bit Settings
// 'C' source line config statements
#include "i2c.h"
#include <stdint.h>
#include "euart.h"

#define _XTAL_FREQ 8000000


// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

void I2C_Master_Init(const unsigned long c)
{
  //SSP1CON1 = 0b00101000;
  SSP1CON1 = 0x28;
  SSP1CON2 = 0;
  SSP1CON3 = 0;
  //SSP1CON3 = 0b01100000;
  SSP1STAT = 0x80;
  //SSP1ADD = (_XTAL_FREQ/(4*c))-1;
  SSP1ADD = 20;
  TRISBbits.TRISB6 = 1;        //Setting as input as given in datasheet
  TRISBbits.TRISB4 = 1;        //Setting as input as given in datasheet
  
  //SSP1ADD = 0x3;
  //SSP1STAT = 0x80;
}

void I2C_Master_Wait(void)
{
  while ( ( SSP1CON2 & 0x1F ) || ( SSP1STAT & 0x04 ) )
  {
      EUSART_Write(0x30 + (SSP1CON2 & 0xF));
      EUSART_Write(0x30 + ((SSP1CON2 & 0x10)>>4));
      EUSART_Write(0x30 + (SSP1STAT & 0x04));
      EUSART_Write(' ');
  }
}

void I2C_Master_Start(void)
{
  I2C_Master_Wait();
  SSP1CON2bits.SEN = 1;
  PIR1bits.SSP1IF = 0;
}

void I2C_Master_RepeatedStart(void)
{
  I2C_Master_Wait();
  SSP1CON2bits.RSEN = 1;
}

void I2C_Master_Stop()
{
  I2C_Master_Wait();
  SSP1CON2bits.PEN = 1;
}

void I2C_Master_Write(unsigned d)
{
  I2C_Master_Wait();
  SSP1BUF = d;
  while(!PIR1bits.SSP1IF); //wait for the slave to put the interupt flag to go 
  //high to show transmission is complete this waits for the 9th clock and then sets high.
  PIR1bits.SSP1IF = 0;
  while(SSP1STATbits.BF);       /* wait till complete data is sent from buffer */
  I2C_Master_Wait();
}

unsigned short I2C_Master_Read(unsigned short a)
{
  unsigned short temp;
  I2C_Master_Wait();
  SSP1CON2bits.RCEN = 1;
  I2C_Master_Wait();
  temp = SSP1BUF;
  I2C_Master_Wait();
  PIR1bits.SSP1IF = 0;  
  SSP1CON2bits.ACKDT = (a)?0:1;
  SSP1CON2bits.ACKEN = 1;
  PIR1bits.SSP1IF = 0; 
  while(SSP1CON2bits.ACKEN)    /* wait for ack data to send on bus */
  {
      //EUSART_Write(0x30 + ((SSP1CON2 & 0x10)>>4));
     // EUSART_Write('e');
  }
  return temp;
}

// Initialise MSSP port. (12F1822 - other devices may differ)
void i2c_Init(void){

   // Initialise I2C MSSP
   // Master 100KHz
   TRISB4=1;                    // set SCL and SDA pins as inputs
   TRISB6=1;

   SSPCON1 = 0b00101000; 	// I2C enabled, Master mode
   SSPCON2 = 0x00;
   // I2C Master mode, clock = FOSC/(4 * (SSPADD + 1)) 
   SSPADD = 19;    		// 100Khz @ 16Mhz Fosc

   SSPSTAT = 0b11000000; 	// Slew rate disabled

}

// i2c_Wait - wait for I2C transfer to finish
void i2c_Wait(void){
    while ( ( SSP1CON2 & 0x1F ) || ( SSPSTAT & 0x04 ) ){
        EUSART_Write('e');
    }
}

// i2c_Start - Start I2C communication
void i2c_Start(void)
{
    i2c_Wait();
    SSP1CON2bits.SEN=1;
}

// i2c_Restart - Re-Start I2C communication
void i2c_Restart(void){
    i2c_Wait();
    SSP1CON2bits.RSEN=1;
}

// i2c_Stop - Stop I2C communication
void i2c_Stop(void)
{
    i2c_Wait();
    SSP1CON2bits.PEN=1;
}

// i2c_Write - Sends one byte of data
void i2c_Write(unsigned char data)
{
    i2c_Wait();
    SSP1BUF = data;
}

// i2c_Address - Sends Slave Address and Read/Write mode
// mode is either I2C_WRITE or I2C_READ
void i2c_Address(unsigned char address, unsigned char mode)
{
    unsigned char l_address;

    l_address=address<<1;
    l_address+=mode;
    i2c_Wait();
    SSP1BUF = l_address;
}

// i2c_Read - Reads a byte from Slave device
unsigned char i2c_Read(unsigned char ack)
{
    // Read data from slave
    // ack should be 1 if there is going to be more data read
    // ack should be 0 if this is the last byte of data read
    unsigned char i2cReadData;

    i2c_Wait();
    SSP1CON2bits.RCEN=1;
    i2c_Wait();
    i2cReadData = SSPBUF;
    i2c_Wait();
    if ( ack ) SSP1CON2bits.ACKDT=0;	        // Ack
    else       SSP1CON2bits.ACKDT=1;	        // NAck
    SSP1CON2bits.ACKEN=1;                    // send acknowledge sequence

    return( i2cReadData );
}