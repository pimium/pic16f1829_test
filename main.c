#define _XTAL_FREQ 8000000
#include <pic16f1829.h>
#include <xc.h>
#include "adc.h"
#include "euart.h"
#include "i2c.h"

// BEGIN CONFIG
#pragma config FOSC  = INTOSC
#pragma config WDTE  = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP   = OFF
#pragma config CPD   = OFF
#pragma config WRT   = OFF
#pragma config CP    = ON
// END CONFIG

// MCLR Disabled, Code Protect on, Watchdog disabled, Brownout off, Internal Osc
//__CONFIG(MCLRE_OFF & CP_ON & WDTE_OFF & BOREN_OFF & FOSC_INTOSC);
#define I2C_SLAVE 0x2a

//#pragma interrupt InterruptHandler 1
void interrupt InterruptHandler(void)  // interrupt 1
{
  if (PIE1bits.RCIE && PIR1bits.RCIF) {
    uint8_t data  = RCREG;
    PIR1bits.RCIF = 0;
    EUSART_Write(data);

    if (1 == RCSTAbits.OERR) {
      // EUSART error - restart
      RCSTAbits.CREN = 0;
      RCSTAbits.CREN = 1;
    }
  }
}

void main()
{
  // nRBPU = 0;    //Enables PORTB Internal Pull Up Resistors
  TRISCbits.TRISC1 = 1;  // PORTB as input
  EUSART_Initialize();
  // I2C_Master_Init(500000);      //Initialize I2C Master with 100KHz clock
  // i2c_Init();

  ADC_Init();
  __delay_ms(500);
  EUSART_Write('S');
  EUSART_Write('T');
  EUSART_Write('A');
  EUSART_Write('R');
  EUSART_Write('T');
  EUSART_Write('\n');
  __delay_ms(10);
  int ack = 0;
  do {
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
      I2C_Master_Write((ack)&0xF);
     
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
       
    ack = ADC_Read(8);
    EUSART_Write(0x30 + ((ack>>12)&0xF));
    EUSART_Write(0x30 + ((ack>>8)&0xF));
    
    EUSART_Write(0x30 + ((ack>>4)&0xF));
    EUSART_Write(0x30 + ((ack>>0)&0xF));
#endif
    EUSART_Write('r');
    EUSART_Write('\r');

    // uint8_t data = EUSART_Read();
    // EUSART_Write(data);

    __delay_ms(5);
  } while (1);
}

// TRISB = 0b01011111;                 //PORTB as input
// PORTBbits.RB5 = 0;
// PORTBbits.RB7 = 0;
