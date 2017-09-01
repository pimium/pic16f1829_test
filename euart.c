/**
  Section: Included Files
 */
#include "euart.h"

/**
  Section: EUSART APIs
 */

void EUSART_Initialize(void)
{
  // Set the EUSART module to the options selected in the user interface.

  // ABDEN disabled; WUE disabled; RCIDL idle; ABDOVF no_overflow; SCKP
  // async_noninverted_sync_fallingedge; BRG16 16bit_generator;
  // BAUDCON = 0x48;
  BAUDCON = 0xA;
  // APFCON0bits.RXDTSEL = 0x1; //RC5 as RX
  APFCON0bits.RXDTSEL = 0x0;  // RB5 as RX
  APFCON0bits.T1GSEL  = 0x0;
  APFCON0bits.TXCKSEL = 0x0;

  // APFCON0 = 0x80;

  // ADDEN disabled; RX9 8-bit; RX9D 0x0; FERR no_error; CREN disabled; SPEN
  // enabled; SREN disabled; OERR no_error;
  RCSTAbits.SPEN = 1;
  RCSTAbits.CREN = 1;

  // RCSTA = 0x90;
  // CSRC slave_mode; TRMT TSR_empty; TXEN enabled; BRGH hi_speed; SYNC
  // asynchronous; SENDB sync_break_complete; TX9D 0x0; TX9 8-bit;
  TXSTAbits.CSRC  = 0;
  TXSTAbits.TX9   = 0;
  TXSTAbits.TXEN  = 1;
  TXSTAbits.SYNC  = 0;
  TXSTAbits.SENDB = 0;
  TXSTAbits.BRGH  = 1;
  TXSTAbits.TRMT  = 1;

  // Baud Rate = 9600; SP1BRGL 207;
  SPBRGL = 12;  // 0xCF;

  // Baud Rate = 9600; SP1BRGH 0;
  SPBRGH = 0x00;

  TRISBbits.TRISB5   = 1;
  ANSELBbits.ANSB5   = 0;
  ANSELBbits.ANSELB  = 0;
  INLVLBbits.INLVLB5 = 1;

  TRISCbits.TRISC5 = 1;
  TRISBbits.TRISB7 = 1;

  PIE1bits.RCIE   = 1;
  INTCONbits.PEIE = 1;
  INTCONbits.GIE  = 1;
}

uint8_t EUSART_Read(void)
{
  while (!PIR1bits.RCIF) {
  }

  if (1 == RCSTAbits.OERR) {
    // EUSART error - restart

    RCSTAbits.CREN = 0;
    RCSTAbits.CREN = 1;
  }

  return RCREG;
}

void EUSART_Write(uint8_t txData)
{
  while (0 == PIR1bits.TXIF) {
  }

  TXREG = txData;  // Write the data byte to the USART.
}

char getch(void) { return EUSART_Read(); }

void putch(char txData) { EUSART_Write(txData); }
/**
  End of File
 */
