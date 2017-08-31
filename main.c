#define _XTAL_FREQ 8000000
#include <xc.h>
#include <pic16f1829.h>
#include "euart.h"
#include "i2c.h"
#include "adc.h"

// BEGIN CONFIG
#pragma config FOSC = INTOSC 
#pragma config WDTE = OFF 
#pragma config PWRTE = OFF
#pragma config BOREN = OFF 
#pragma config LVP = OFF 
#pragma config CPD = OFF 
#pragma config WRT = OFF 
#pragma config CP = ON
//END CONFIG

// MCLR Disabled, Code Protect on, Watchdog disabled, Brownout off, Internal Osc
//__CONFIG(MCLRE_OFF & CP_ON & WDTE_OFF & BOREN_OFF & FOSC_INTOSC);
#define I2C_SLAVE 0x2a

void main()
{
  //nRBPU = 0;    //Enables PORTB Internal Pull Up Resistors
  TRISCbits.TRISC1 = 1;
  TRISB = 0xFF;                 //PORTB as input
  EUSART_Initialize();
  //I2C_Master_Init(500000);      //Initialize I2C Master with 100KHz clock
  //i2c_Init();	
  ADC_Init();
  __delay_ms(500);
  EUSART_Write('S');
  EUSART_Write('T');
  EUSART_Write('A');
  EUSART_Write('R');
  EUSART_Write('T');
  EUSART_Write('\n');
  int ack = 0;
  int add = 0;
  do
  {
#if 0      
    i2c_Start();                        // send Start
    i2c_Address(I2C_SLAVE, I2C_WRITE);  // Send slave address - write operation
    i2c_Write(0x00);	                // Set register for servo 0
    i2c_Restart();                      // Restart
    i2c_Address(I2C_SLAVE, I2C_READ);   // Send slave address - read operation	
    ack = i2c_Read(0);
    EUSART_Write('n');
    ack = (ack << 8) + i2c_Read(1);
    EUSART_Write('m');
    i2c_Stop();
#endif
#if 0      
      I2C_Master_Start();         //Start condition
      I2C_Master_Write((0x2a << 1) | 0x0);
      I2C_Master_Write((add)&0xF);
     
      //I2C_Master_RepeatedStart();
      I2C_Master_Start();         //Start condition
      I2C_Master_Write((0x2a << 1) | 0x1);     //7 bit address + Read
      //I2C_Master_Write(0x02);
      
      EUSART_Write('n');
      //I2C_Master_Start();         //Start condition
      ack = I2C_Master_Read(0); //Read + Acknowledge
      EUSART_Write('A');
     
     //ack = (ack << 8) + I2C_Master_Read(0);
     EUSART_Write('C');
     ack = (ack << 8) + I2C_Master_Read(1); //Read + Not  Acknowledge
     EUSART_Write('K');
     EUSART_Write(' ');
     I2C_Master_Stop();          //Stop condition
     
    __delay_ms(2);
#endif       
    ack = ADC_Read(8);
    EUSART_Write(0x30 + ((ack>>12)&0xF));
    EUSART_Write(0x30 + ((ack>>8)&0xF));
    
    EUSART_Write(0x30 + ((ack>>4)&0xF));
    EUSART_Write(0x30 + ((ack>>0)&0xF));
      
      EUSART_Write('\r');
   // UART_Write(PORTB);
   
    __delay_ms(10);
  }while(1);
}

//TRISB = 0b01011111;                 //PORTB as input
//PORTBbits.RB5 = 0; 
//PORTBbits.RB7 = 0;
