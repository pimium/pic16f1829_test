
#include "adc.h"

void ADC_Init()
{
  ADCON0 = 0x01;               //Turn ON ADC and Clock Selection
  ADCON1 = 0x90;               //All pins as Analog Input and setting Reference Voltages
  TRISCbits.TRISC6 = 1;
}

unsigned int ADC_Read(unsigned int channel)
{
  ADCON0 &= ~0x7C;              //Clearing channel selection bits
  ADCON0 |= (channel & 0x1F) <<2;        //Setting channel selection bits
  //ADCON0 = 0x21;
  __delay_ms(2);               //Acquisition time to charge hold capacitor
  GO_nDONE = 1;                //Initializes A/D conversion
  while(GO_nDONE);             //Waiting for conversion to complete
  unsigned int adc_value = ADRESH;
  adc_value = (adc_value<<8) + ADRESL;
  return adc_value; //Return result
}
